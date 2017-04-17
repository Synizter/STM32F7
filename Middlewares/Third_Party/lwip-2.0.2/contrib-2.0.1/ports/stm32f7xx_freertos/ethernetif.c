
/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/mld6.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include "ethernetif.h"
#include "stm32f7xx_eth.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

#define NETIF_TASK_STACK_SIZE			( 350 )
#define NETIF_TASK_PRIORITY			( configMAX_PRIORITIES - 2 )
#define NETIF_GUARD_BLOCK_TIME			( 250 )

/* The time to block waiting for input. */
#define NETIF_TIME_WAITING_FOR_INPUT		( ( portTickType ) 100 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Semaphore to signal incoming packets */
static SemaphoreHandle_t s_xSemaphore = NULL;
static SemaphoreHandle_t xTxSemaphore = NULL;


#if defined ( __ICCARM__ ) /*!< IAR Compiler */

//#pragma location=0x20022000
#pragma data_alignment=32
__no_init ETH_DMADescBufTypeDef  DMARxDscrTab[ETH_RXBUFNB];/* Ethernet Rx MA Descriptor */

//#pragma location=0x20022100
#pragma data_alignment=32
__no_init ETH_DMADescBufTypeDef  DMATxDscrTab[ETH_TXBUFNB];/* Ethernet Tx DMA Descriptor */

//#pragma location=0x20022200
#pragma data_alignment=32
__no_init ETH_RxBufferTypeDef ETH_RxBuffer[ETH_RXBUFNB];

//#pragma location=0x20023FC4
#pragma data_alignment=32
__no_init ETH_TxBufferTypeDef ETH_TxBuffer[ETH_TXBUFNB];

#elif defined ( __CC_ARM   )

ETH_DMADescBufTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__((at(0x20002000)));/* Ethernet Rx MA Descriptor */
ETH_DMADescBufTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__((at(0x20002100)));/* Ethernet Tx DMA Descriptor */
uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__((at(0x20002200)));  /* Ethernet Receive Buffer */
uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE]  __attribute__((at(0x20003FC4))); /* Ethernet Transmit Buffer */

#elif defined ( __GNUC__   )

ETH_DMADescBufTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__((section(".RxDescripSection")));/* Ethernet Rx MA Descriptor */
ETH_DMADescBufTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__((section(".TxDescripSection")));/* Ethernet Tx DMA Descriptor */
uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__((section(".RxBUF")));/* Ethernet Receive Buffer */
uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__((section(".TxBUF")));/* Ethernet Transmit Buffer */

#endif

/* Global pointers to track current transmit and receive descriptors */
extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;

/* Global pointer for last received frame infos */
extern ETH_DMA_Rx_Frame_infos *DMA_RX_FRAME_infos;

/* Private function prototypes -----------------------------------------------*/
static void ethernetif_input( void * argument );

/* Private functions ---------------------------------------------------------*/

/**
* In this function, the hardware should be initialized.
* Called from ethernetif_init().
*
* @param netif the already initialized lwip network interface structure
*        for this ethernetif
*/
static void low_level_init(struct netif *netif)
{
  uint32_t i;
  ETH_DMADESCTypeDef *DMADesc;
  
  /* set netif MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set netif MAC hardware address */
  netif->hwaddr[0] =  MAC_ADDR0;
  netif->hwaddr[1] =  MAC_ADDR1;
  netif->hwaddr[2] =  MAC_ADDR2;
  netif->hwaddr[3] =  MAC_ADDR3;
  netif->hwaddr[4] =  MAC_ADDR4;
  netif->hwaddr[5] =  MAC_ADDR5;

  /* set netif maximum transfer unit */
  netif->mtu = 1500;

  /* Accept broadcast address and ARP traffic */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

#if LWIP_IPV6 && LWIP_IPV6_MLD
  /*
   * For hardware/netifs that implement MAC filtering.
   * All-nodes link-local is handled by default, so we must let the hardware know
   * to allow multicast packets in.
   * Should set mld_mac_filter previously. */
  if (netif->mld_mac_filter != NULL) {
    ip6_addr_t ip6_allnodes_ll;
    ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
    netif->mld_mac_filter(netif, &ip6_allnodes_ll, MLD6_ADD_MAC_FILTER);
  }
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */
  
  /* create binary semaphore used for informing ethernetif of frame reception */
  s_xSemaphore = xSemaphoreCreateBinary();
  xSemaphoreGive( s_xSemaphore );
  
  /* create binary semaphore used for Transmit ethernetif of frame reception */
  xTxSemaphore = xSemaphoreCreateBinary();
  xSemaphoreGive( xTxSemaphore );
  
  /* initialize MAC address in ethernet MAC */ 
  ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr); 

  /* Initialize Tx Descriptors list: Chain Mode */
  ETH_DMATxDescChainInit(DMATxDscrTab, ETH_TxBuffer, ETH_TXBUFNB);
  
  /* Initialize Rx Descriptors list: Chain Mode  */
  ETH_DMARxDescChainInit(DMARxDscrTab, ETH_RxBuffer, ETH_RXBUFNB);

  /* Enable Ethernet Rx interrrupt */
  {
    DMADesc = DMARxDescToGet;
    
    for( i = 0; i < ETH_RXBUFNB; i++ )
    {
      ETH_DMARxDescReceiveITConfig(DMADesc, ENABLE);
      
      SCB_InvalidateDCache_by_Addr((uint32_t*)DMADesc, sizeof(ETH_DMADESCTypeDef));
    }
  }

#ifdef CHECKSUM_BY_HARDWARE
  /* Enable the TCP, UDP and ICMP checksum insertion for the Tx frames */
  DMADesc = DMATxDescToSet;
  
  for(i=0; i<ETH_TXBUFNB; i++)
  {
    ETH_DMATxDescChecksumInsertionConfig(DMADesc, ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
    
    SCB_InvalidateDCache_by_Addr((uint32_t*)DMADesc, sizeof(ETH_DMADESCTypeDef));
    
    DMADesc = ETH_DMADESC_NEXT(DMADesc);
  }
#endif

  /* create the task that handles the ETH_MAC */
  xTaskCreate( ethernetif_input, (char*) "netif", NETIF_TASK_STACK_SIZE, (void*)netif, NETIF_TASK_PRIORITY, NULL );

  /* Enable MAC and DMA transmission and reception */
  ETH_Start();   
}

/**
* This function should do the actual transmission of the packet. The packet is
* contained in the pbuf that is passed to the function. This pbuf
* might be chained.
*
* @param netif the lwip network interface structure for this ethernetif
* @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
* @return ERR_OK if the packet could be sent
*         an err_t value if the packet couldn't be sent
*
* @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
*       strange results. You might consider waiting for space in the DMA queue
*       to become availale since the stack doesn't retry to send a packet
*       dropped because of memory failure (except for the TCP timers).
*/

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  struct pbuf *q;
  u8_t *buffer ;
  __IO ETH_DMADESCTypeDef *DmaTxDesc;
  uint32_t framelength = 0;
  uint32_t bufferoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t payloadoffset = 0;
  
  if ( xSemaphoreTake( xTxSemaphore, NETIF_GUARD_BLOCK_TIME ) == pdTRUE )
  {
    DmaTxDesc = DMATxDescToSet;
    buffer = (u8_t *)(DmaTxDesc->Buffer1Addr);
    bufferoffset = 0;

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    
    for(q = p; q != NULL; q = q->next) 
    {
      if(( DmaTxDesc->Status & ETH_DMATxDesc_OWN ) != (u32_t)RESET )
      {
        goto error;
      }

      /* Get bytes in current lwIP buffer  */
      byteslefttocopy = q->len;
      payloadoffset = 0;

      /* Check if the length of data to copy is bigger than Tx buffer size*/
      while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
      {
        /* Copy data to Tx buffer*/
        memcpy( (u8_t*)((u8_t*)buffer + bufferoffset), (u8_t*)((u8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );

        /* Point to next descriptor */
        DmaTxDesc = (ETH_DMADESCTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);

        /* Check if the buffer is available */
        if((DmaTxDesc->Status & ETH_DMATxDesc_OWN) != (u32_t)RESET)
        {
          goto error;
        }

        buffer = (u8_t *)(DmaTxDesc->Buffer1Addr);

        byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
        framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }

      /* Copy the remaining bytes */
      memcpy( (u8_t*)((u8_t*)buffer + bufferoffset), (u8_t*)((u8_t*)q->payload + payloadoffset), byteslefttocopy );
      bufferoffset = bufferoffset + byteslefttocopy;
      framelength = framelength + byteslefttocopy;
    }

    /* Prepare transmit descriptors to give to DMA*/
    ETH_Prepare_Transmit_Descriptors(framelength);

    MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
    if (((u8_t*)p->payload)[0] & 1) {
      /* broadcast or multicast packet*/
      MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
    } else {
      /* unicast packet */
      MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
    }
    /* increase ifoutdiscards or ifouterrors on error */

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
    
    LINK_STATS_INC(link.xmit);
    
    /* Give semaphore and exit */
  error:
    xSemaphoreGive(xTxSemaphore);
  }
  
  return ERR_OK;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p = NULL, *q = NULL;
  uint16_t len = 0;
  uint8_t *buffer;
  FrameTypeDef frame;
  __IO ETH_DMADESCTypeDef *DMARxDesc;
  uint32_t bufferoffset = 0;
  uint32_t payloadoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t i = 0;
  
  /* get received frame */
  frame = ETH_Get_Received_Frame_interrupt();
  
  /* Obtain the size of the packet and put it into the "len" variable. */
  len = frame.length;
  buffer = (u8_t *)frame.buffer;
  
  if ( len > 0 )
  {
#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif
    
    /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  }
  
  if ( p != NULL )
  {
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    
    DMARxDesc = frame.descriptor;
    bufferoffset = 0;
    
    for( q = p; q != NULL; q = q->next )
    {
      byteslefttocopy = q->len;
      payloadoffset = 0;
      
      /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size */
      while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
      {
        /* Copy data to pbuf */
        memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));
        
        /* Point to next descriptor */
        DMARxDesc = (ETH_DMADESCTypeDef *)(DMARxDesc->Buffer2NextDescAddr);
        buffer = (uint8_t *)(DMARxDesc->Buffer1Addr);
        
        byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }
      
      /* Copy remaining data in pbuf */
      memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
      bufferoffset = bufferoffset + byteslefttocopy;
    }
    
    MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
    if (((u8_t*)p->payload)[0] & 1) {
      /* broadcast or multicast packet*/
      MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
    } else {
      /* unicast packet*/
      MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
    }
#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
    
    LINK_STATS_INC(link.recv);
  } else {
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
    MIB2_STATS_NETIF_INC(netif, ifindiscards);
  }
    
  /* Release descriptors to DMA */
  /* Point to first descriptor */
  DMARxDesc = frame.descriptor;
  
  /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
  for ( i = 0; i < DMA_RX_FRAME_infos->Seg_Count; i++ )
  {  
    DMARxDesc->Status |= ETH_DMARxDesc_OWN;
    DMARxDesc = (ETH_DMADESCTypeDef *)(DMARxDesc->Buffer2NextDescAddr);
  }
  
  /* Clear Segment_Count */
  DMA_RX_FRAME_infos->Seg_Count = 0;
  
  /* When Rx Buffer unavailable flag is set: clear it and resume reception */
  if ((ETH->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
  {
    /* Clear RBUS ETHERNET DMA flag */
    ETH->DMASR = ETH_DMASR_RBUS;
    /* Resume DMA reception */
    ETH->DMARPDR = 0;
  }
  return p;
}

/**
  * @brief This function is the ethernetif_input task, it is processed when a packet 
  * is ready to be read from the interface. It uses the function low_level_input() 
  * that should handle the actual reception of bytes from the network
  * interface. Then the type of the received packet is determined and
  * the appropriate input function is called.
  *
  * @param netif the lwip network interface structure for this ethernetif
  */
static void ethernetif_input( void * argument )
{
  struct pbuf *p;
  struct netif *netif = (struct netif *) argument;
  
  for( ;; )
  {
    if ( xSemaphoreTake( s_xSemaphore, NETIF_TIME_WAITING_FOR_INPUT ) == pdTRUE )
    {
      do
      {
        p = low_level_input( netif );
        if ( p != NULL )
        {
          if (netif->input( p, netif) != ERR_OK )
          {
            pbuf_free(p);
          }
        }
      }while( p != NULL );
    }
  }
}

/**
  * @brief  Ethernet IRQ Handler when Receive complete
  * @param  None
  * @retval None
  */
void ethernetif_recv_irq( void )
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
  /* Give the semaphore to wakeup NetIF task */
  xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );
  
  /* Switch tasks if necessary. */	
  if( xHigherPriorityTaskWoken != pdFALSE )
  {
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
}

/**
* Should be called at the beginning of the program to set up the
* network interface. It calls the function low_level_init() to do the
* actual setup of the hardware.
*
* This function should be passed as a parameter to netif_add().
*
* @param netif the lwip network interface structure for this ethernetif
* @return ERR_OK if the loopif is initialized
*         ERR_MEM if private data couldn't be allocated
*         any other err_t on error
*/
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));
  
#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);
  
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
#if LWIP_IPV6
  netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
  netif->linkoutput = low_level_output;
  
  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}