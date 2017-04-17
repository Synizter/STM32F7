/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "lwip/init.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "stm32f7xx_eth_bsp.h"
#include "stm32f7xx_eth_netconf.h"
#include "ethernetif.h"


/* Scheduler includes */
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LINK_CHECK_INTERVAL                     5000

#define NETCONF_TASK_STACK_SIZE		        ( configMINIMAL_STACK_SIZE * 2 )
#define NETCONF_TASK_PRIORITY		        ( osPriorityRealtime )
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static struct netif netif;
static __IO uint8_t LastLinkStatus = 0;
static __IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
static uint32_t LinkCheckTimer = 0;
DHCP_State_TypeDef DHCP_state = DHCP_START;
static void (*NetCallback)(NetCB_Event_TypeDef);

osThreadId NetConf_ID;

#ifdef USE_DHCP
uint32_t DHCPfineTimer = 0;
uint32_t DHCPcoarseTimer = 0;
#endif

/* Private function prototypes -----------------------------------------------*/
static void LwIP_LinkCallback(struct netif *netif);


#ifdef USE_DHCP
static void LwIP_NetConfTask(void const *argument);
#endif

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void LwIP_Init(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  
  /* Initialize the LwIP stack */
  lwip_init();

#ifdef USE_DHCP
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;
#else
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))
    
   Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface.*/
  netif_set_default(&netif);
  
  if( ETH_BSP_PhyGetLinkStatus() )
  {
    LastLinkStatus = 1;
    
    /* Set Ethernet link flag */
    netif.flags |= NETIF_FLAG_LINK_UP;
    
    /*  When the netif is fully configured this function must be called.*/
    netif_set_up(&netif);
    
#ifdef USE_DHCP
    DHCP_state = DHCP_START;
#else
    DHCP_state = STATIC_IP;
#endif /* USE_DHCP */
  }
  else
  {
    LastLinkStatus = 0;
    
    /*  When the netif link is down this function must be called.*/
    netif_set_down(&netif);
#ifdef USE_DHCP
    DHCP_state = DHCP_LINK_DOWN;
#endif /* USE_DHCP */
  }
  
  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&netif, LwIP_LinkCallback);
  
  /* Set user net callback to null */
  NetCallback = 0;
  
  /* Task Definition */
  osThreadDef(Network_Config, LwIP_NetConfTask, NETCONF_TASK_PRIORITY, NULL, NETCONF_TASK_STACK_SIZE);
  NetConf_ID = osThreadCreate(osThread(Network_Config), NULL);
}

static void LwIP_NetConfTask(void const *argument) {
  (void) argument;\
  
#ifdef USE_DHCP
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
    struct dhcp *dhcp = netif_dhcp_data(&netif);
#endif
   
  LinkCheckTimer = 0;
  
  for(;;) {

#ifdef USE_DHCP
    switch (DHCP_state)
    {
    case DHCP_START:
      {
        dhcp_start(&netif);
        DHCP_state = DHCP_WAIT_ADDRESS;
      }
      break;
    case DHCP_WAIT_ADDRESS:
      {
        /* Check the new IP address is assigned */
        if ( netif.ip_addr.addr != 0 ) 
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;	
          
          /* Stop DHCP */
          dhcp_stop(&netif);
          /* DHCP Get Address Callback */
          if(NetCallback)
          {
            NetCallback(NET_EVENT_DHCP_GET_ADDRESS);
          }
        }
        else
        {
          /* DHCP timeout */
          if (dhcp->tries > MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;
            /* Stop DHCP */
            dhcp_stop(&netif);
            /* Static address used */
            IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
            IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
            IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
            netif_set_addr(&netif, &ipaddr , &netmask, &gw);
            /* DHCP Timeout Callback */
            if(NetCallback)
            {
              NetCallback(NET_EVENT_DHCP_TIMEOUT);
            }
          }
        }
      }
      break;
    default: break;
    }
#endif
    
    if( LinkCheckTimer >= LINK_CHECK_INTERVAL )
    {
      /* Check the link status */
      if( ETH_BSP_PhyGetLinkStatus() )
      {
        if( !LastLinkStatus )
        {
          LastLinkStatus = 1;
          netif_set_link_up(&netif);
        }
      }
      else
      {
        if( LastLinkStatus )
        {
          LastLinkStatus = 0;
          netif_set_link_down(&netif);
        }
      }
      
      LinkCheckTimer = 0;
    }
    
    /* wait 250 ms */
    osDelay(DHCP_FINE_TIMER_MSECS / 2);
    LinkCheckTimer += (DHCP_FINE_TIMER_MSECS / 2);
  }
}



/**
  * @brief  LwIP_NetSetCallback
  * @param  callback: DHCP callback function.
  * @retval None
  */
void LwIP_NetSetCallback(void (*Callback)(NetCB_Event_TypeDef))
{
  NetCallback = Callback;
}


/**
  * @brief  Link callback function, this function is called on change of link status.
  * @param  The network interface
  * @retval None
  */
static void LwIP_LinkCallback(struct netif *netif)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

  if(netif_is_link_up(netif))
  {
    ETH_BSP_PhyRestart();

#ifdef USE_DHCP
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;

    DHCP_state = DHCP_START;
#else
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif /* USE_DHCP */

    netif_set_addr(netif, &ipaddr , &netmask, &gw);
    
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(netif);    
    
    /* Net Link up Callback */
    if(NetCallback)
    {
      NetCallback(NET_EVENT_LINK_UP);
    }
  }
  else
  {
    ETH_BSP_PhyStop();
#ifdef USE_DHCP
    DHCP_state = DHCP_LINK_DOWN;
    dhcp_stop(netif);
#endif /* USE_DHCP */

    /*  When the netif link is down this function must be called.*/
    netif_set_down(netif);
    
    /* Net Link down Callback */
    if(NetCallback)
    {
      NetCallback(NET_EVENT_LINK_DOWN);
    }
  }
}

/**
  * @brief  Returns the current time in milliseconds
  *         when LWIP_TIMERS == 1 and NO_SYS == 1
  * @param  None
  * @retval Current Time value
  */
u32_t sys_now(void)
{
  return LocalTime;
}

/**
  * @brief  Update the current time value in milliseconds.
  * @param  inc_ms : Time increase value in milliseconds
  * @retval None
  */
void LwIP_TimerUpdate(uint32_t inc_ms)
{
  LocalTime += inc_ms;
}

/**
  * @brief  Get ip address
  * @param  iptext : Buffer to contrain ip address in text.
  * @retval None
  */
void LwIP_GetIPAddr( uint8_t * iptext )
{
  uint8_t iptab[4];
  uint32_t IPaddress;
  
  IPaddress = netif.ip_addr.addr;
  
  iptab[0] = (uint8_t)(IPaddress >> 24);
  iptab[1] = (uint8_t)(IPaddress >> 16);
  iptab[2] = (uint8_t)(IPaddress >> 8);
  iptab[3] = (uint8_t)(IPaddress);

  sprintf((char*)iptext, "%d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]);
}