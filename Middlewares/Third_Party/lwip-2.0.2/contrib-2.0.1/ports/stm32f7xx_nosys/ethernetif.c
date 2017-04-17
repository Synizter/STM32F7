/**
 * @file
 * Ethernet Interface for standalone applications (without RTOS) - works only for 
 * ethernet polling mode (polling for ethernet frame reception)
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/mld6.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include "ethernetif.h"
#include "stm32f7xx_eth.h"
#include <string.h>

/* Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'


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

//ETH_TxBufferTypeDef ETH_TxBuffer[ETH_TXBUFNB];
//ETH_RxBufferTypeDef ETH_RxBuffer[ETH_RXBUFNB];

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
#ifdef CHECKSUM_BY_HARDWARE
  int i; 
  ETH_DMADESCTypeDef *DMADesc;
#endif
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] =  MAC_ADDR0;
  netif->hwaddr[1] =  MAC_ADDR1;
  netif->hwaddr[2] =  MAC_ADDR2;
  netif->hwaddr[3] =  MAC_ADDR3;
  netif->hwaddr[4] =  MAC_ADDR4;
  netif->hwaddr[5] =  MAC_ADDR5;
  
  /* initialize MAC address in ethernet MAC */ 
  ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr); 

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
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
  
  /* Initialize Tx Descriptors list: Chain Mode */
  ETH_DMATxDescChainInit(DMATxDscrTab, ETH_TxBuffer, ETH_TXBUFNB);
  /* Initialize Rx Descriptors list: Chain Mode  */
  ETH_DMARxDescChainInit(DMARxDscrTab, ETH_RxBuffer, ETH_RXBUFNB);

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

   /* Note: TCP, UDP, ICMP checksum checking for received frame are enabled in DMA config */

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
  err_t errval;
  struct pbuf *q;
  uint8_t *buffer =  (uint8_t *)(DMATxDescToSet->Buffer1Addr);
  __IO ETH_DMADESCTypeDef *DmaTxDesc;
  __IO ETH_DMADESCTypeDef *FirstDmaTxDesc;
  uint32_t count = 0;
  uint32_t framelength = 0;
  uint32_t bufferoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t payloadoffset = 0;
  
  DmaTxDesc = DMATxDescToSet;
  FirstDmaTxDesc = DMATxDescToSet;
  bufferoffset = 0;
  count++;
  
#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
  
  SCB_InvalidateDCache_by_Addr((uint32_t*)DmaTxDesc, sizeof(ETH_DMADESCTypeDef));
  
  /* copy frame from pbufs to driver buffers */
  for(q = p; q != NULL; q = q->next)
  {
    /* Is this buffer available? If not, goto error */
    if((DmaTxDesc->Status & ETH_DMATxDesc_OWN) != (uint32_t)RESET)
    {
      errval = ERR_BUF;
      goto error;
    }

    /* Get bytes in current lwIP buffer */
    byteslefttocopy = q->len;
    payloadoffset = 0;
    
    /* Check if the length of data to copy is bigger than Tx buffer size*/
    while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
    {
      /* Copy data to Tx buffer*/
      memcpy( (u8_t*)((u8_t*)buffer + bufferoffset), (u8_t*)((u8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );
      
      SCB_CleanDCache_by_Addr((uint32_t*)(DmaTxDesc->Buffer1Addr), ETH_TX_BUF_SIZE);
      
      /* Point to next descriptor */
      DmaTxDesc = (ETH_DMADESCTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);
      count++;
      
      SCB_InvalidateDCache_by_Addr((uint32_t*)DmaTxDesc, sizeof(ETH_DMADESCTypeDef));
      
      /* Check if the buffer is available */
      if((DmaTxDesc->Status & ETH_DMATxDesc_OWN) != (uint32_t)RESET)
      {
        errval = ERR_USE;
        goto error;
      }

      buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);

      byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
      payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
      framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
      bufferoffset = 0;
    }

    /* Copy the remaining bytes */
    memcpy( (u8_t*)((u8_t*)buffer + bufferoffset), (u8_t*)((u8_t*)q->payload + payloadoffset), byteslefttocopy);
    bufferoffset = bufferoffset + byteslefttocopy;
    framelength = framelength + byteslefttocopy;
  }
  
  SCB_CleanDCache_by_Addr((uint32_t*)(DmaTxDesc->Buffer1Addr), byteslefttocopy);
  
  /* Note: padding and CRC for transmitted frame 
     are automatically inserted by DMA */
  
  /* Prepare transmit descriptors to give to DMA*/ 
  ETH_Prepare_Transmit_Descriptors(framelength);
  
  /* Clean desc cache */
  while(count)
  {
    SCB_CleanDCache_by_Addr((uint32_t*)FirstDmaTxDesc, sizeof(ETH_DMADESCTypeDef));
    FirstDmaTxDesc = ETH_DMADESC_NEXT(FirstDmaTxDesc);
    count--;
  }
  
  /* Resume transmission */
  ETH_ClearTransmitBufferUnavailable();
 
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
  
  errval = ERR_OK;

error:
  
  /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
  if ((ETH->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
  {
    /* Clear TUS ETHERNET DMA flag */
    ETH->DMASR = ETH_DMASR_TUS;

    /* Resume DMA transmission*/
    ETH->DMATPDR = 0;
  }
  return errval;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
   */
static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p, *q;
  u16_t len;
  FrameTypeDef frame;
  uint8_t *buffer;
  __IO ETH_DMADESCTypeDef *DMARxDesc;
  uint32_t bufferoffset = 0;
  uint32_t payloadoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t i=0;  
  
  p = NULL;
  
  /* get received frame */
  frame = ETH_Get_Received_Frame();
  
  /* Obtain the size of the packet and put it into the "len" variable. */
  len = frame.length;
  buffer = (uint8_t *)frame.buffer;
  
#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif
  
  /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
  /* Clean and Invalidate data cache */
  //SCB_CleanInvalidateDCache ();
  
  if (p != NULL)
  {
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    
    DMARxDesc = frame.descriptor;
    
    SCB_InvalidateDCache_by_Addr((uint32_t*)DMARxDesc, sizeof(ETH_DMADESCTypeDef));
    
    if(frame.length > ETH_RX_BUF_SIZE)
      SCB_InvalidateDCache_by_Addr((uint32_t*)DMARxDesc->Buffer1Addr, ETH_RX_BUF_SIZE);
    else
      SCB_InvalidateDCache_by_Addr((uint32_t*)DMARxDesc->Buffer1Addr, frame.length);
    
    bufferoffset = 0;
    for(q = p; q != NULL; q = q->next)
    {
      byteslefttocopy = q->len;
      payloadoffset = 0;
      
      /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size*/
      while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
      {
        /* Copy data to pbuf*/
        memcpy( (u8_t*)((u8_t*)q->payload + payloadoffset), (u8_t*)((u8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));
        
        /* Point to next descriptor */
        DMARxDesc = (ETH_DMADESCTypeDef *)(DMARxDesc->Buffer2NextDescAddr);
        
        SCB_InvalidateDCache_by_Addr((uint32_t*)DMARxDesc, sizeof(ETH_DMADESCTypeDef));
        if(frame.length > ETH_RX_BUF_SIZE)
        {
          SCB_InvalidateDCache_by_Addr((uint32_t*)DMARxDesc->Buffer1Addr, ETH_RX_BUF_SIZE);
          frame.length -= ETH_RX_BUF_SIZE;
        }
        else
          SCB_InvalidateDCache_by_Addr((uint32_t*)DMARxDesc->Buffer1Addr, frame.length);
        
        buffer = (unsigned char *)(DMARxDesc->Buffer1Addr);
        
        byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }
      /* Copy remaining data in pbuf */
      memcpy( (u8_t*)((u8_t*)q->payload + payloadoffset), (u8_t*)((u8_t*)buffer + bufferoffset), byteslefttocopy);
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
  DMARxDesc =frame.descriptor;

  /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
  for (i=0; i<DMA_RX_FRAME_infos->Seg_Count; i++)
  {  
    DMARxDesc->Status = ETH_DMARxDesc_OWN;
    SCB_CleanDCache_by_Addr((uint32_t*)DMARxDesc, sizeof(ETH_DMADESCTypeDef));
    DMARxDesc = (ETH_DMADESCTypeDef *)(DMARxDesc->Buffer2NextDescAddr);
  }
  
  /* Clear Segment_Count */
  DMA_RX_FRAME_infos->Seg_Count =0;
  
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
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
err_t ethernetif_input(struct netif *netif)
{
  err_t err;
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);

  /* no packet could be read, silently ignore this */
  if (p == NULL) return ERR_MEM;

  /* entry point to the LwIP stack */
  err = netif->input(p, netif);
  
  if (err != ERR_OK)
  {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
    pbuf_free(p);
    p = NULL;
  }
  return err;
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