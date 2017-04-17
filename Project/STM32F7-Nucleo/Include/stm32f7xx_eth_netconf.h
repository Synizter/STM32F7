/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F7X6_ETH_NETCONF_H
#define __STM32F7X6_ETH_NETCONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"

/* Exported types ------------------------------------------------------------*/
typedef enum 
{ 
  DHCP_START=0,
  DHCP_WAIT_ADDRESS,
  DHCP_ADDRESS_ASSIGNED,
  DHCP_TIMEOUT,
  DHCP_LINK_DOWN,
  STATIC_IP
} DHCP_State_TypeDef;

typedef enum 
{ 
  NET_EVENT_DHCP_GET_ADDRESS = 0,
  NET_EVENT_DHCP_TIMEOUT,
  NET_EVENT_LINK_DOWN,
  NET_EVENT_LINK_UP
} NetCB_Event_TypeDef;

/* Exported constants --------------------------------------------------------*/
#define USE_DHCP       /* enable DHCP, if disabled static address is used*/
#define MAX_DHCP_TRIES        4

/* MAC ADDRESS*/
//#define MAC_ADDR0   02
//#define MAC_ADDR1   00
//#define MAC_ADDR2   00
//#define MAC_ADDR3   00
//#define MAC_ADDR4   00
//#define MAC_ADDR5   00
 
/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   10
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LwIP_Init(void);
void LwIP_Process(void);
void LwIP_GetIPAddr( uint8_t * iptext );
void LwIP_TimerUpdate(uint32_t inc_ms);
void LwIP_NetSetCallback(void (*Callback)(NetCB_Event_TypeDef));

//EDITED
u32_t sys_now(void);



#ifdef __cplusplus
}
#endif

#endif /* __STM32F7X6_ETH_NETCONF_H */
