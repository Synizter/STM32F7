/**
  ******************************************************************************
  * @file    stm32f4x7_eth_bsp.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2013
  * @brief   STM32F4x7 Ethernet hardware configuration.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"

#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_eth.h"
#include "stm32f7xx_eth_bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ETH_InitTypeDef ETH_InitStructure;
__IO uint8_t  EthInitStatus = 0;

/* Private function prototypes -----------------------------------------------*/
static void ETH_GPIO_Config(void);
static void ETH_NVIC_Config(void);
static void ETH_MACDMA_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
void ETH_BSP_Config(void)
{
  /* Configure the GPIO ports for ethernet pins */
  ETH_GPIO_Config();
  
  /* Config NVIC for Ethernet */
  //ETH_NVIC_Config();

  /* Configure the Ethernet MAC/DMA */
  ETH_MACDMA_Config();

  if( !EthInitStatus )
  {
    while(1);
  }
}

/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
static void ETH_MACDMA_Config(void)
{
  /* Enable ETHERNET clock  */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ETHMAC | LL_AHB1_GRP1_PERIPH_ETHMACTX |
                           LL_AHB1_GRP1_PERIPH_ETHMACRX);

  /* Reset ETHERNET on AHB Bus */
  ETH_DeInit();

  /* Software reset */
  ETH_SoftwareReset();

  /* Wait for software reset */
  while (ETH_GetSoftwareResetStatus() == SET);

  /* ETHERNET Configuration --------------------------------------------------*/
  /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
  ETH_StructInit(&ETH_InitStructure);

  /* Fill ETH_InitStructure parametrs */
  /*------------------------   MAC   -----------------------------------*/
  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
  //ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable; 
  ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;   

  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
  ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

  /*------------------------   DMA   -----------------------------------*/  
  
  /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
  the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
  if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;         
  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;     
 
  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;       
  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;   
  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;      
  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;                
  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;          
  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
  //ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_1_1;
  /* Configure Ethernet */
  EthInitStatus = ETH_Init(&ETH_InitStructure, LAN8742A_PHY_ADDRESS);

  /* Enable the Ethernet Rx Interrupt */
  ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void ETH_GPIO_Config(void)
{
  volatile uint32_t i;
  LL_GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIOs clocks */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA | LL_AHB1_GRP1_PERIPH_GPIOB | LL_AHB1_GRP1_PERIPH_GPIOC | LL_AHB1_GRP1_PERIPH_GPIOG);
  
  /* Enable SYSCFG clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  
  /* MII/RMII Media interface selection --------------------------------------*/
  LL_SYSCFG_SetPHYInterface(LL_SYSCFG_PMC_ETHRMII);

/* Ethernet pins configuration ************************************************/
   /*
        ETH_MDIO --------------> PA2
        ETH_MDC ---------------> PC1
    
        ETH_RMII_REF_CLK-------> PA1

        ETH_RMII_CRS_DV -------> PA7
        ETH_RMII_RXER   -------> PG2
        ETH_RMII_RXD0   -------> PC4
        ETH_RMII_RXD1   -------> PC5
        ETH_RMII_TX_EN  -------> PG11
        ETH_RMII_TXD0   -------> PG13
        ETH_RMII_TXD1   -------> PB13

        ETH_RST_PIN     -------> NRST
   */

  /* Configure PA1,PA2 and PA7 */
  GPIO_InitStructure.Pin   = LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_7;
  GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Mode  = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStructure.Pull  = LL_GPIO_PULL_NO;
  GPIO_InitStructure.Alternate = LL_GPIO_AF_11;
  LL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PG2, PG11,PG13 */
  GPIO_InitStructure.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_11 | LL_GPIO_PIN_13;
  LL_GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  /* Configure PB13 */
  GPIO_InitStructure.Pin = LL_GPIO_PIN_13;
  LL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure PC1, PC4 and PC5 */
  GPIO_InitStructure.Pin = LL_GPIO_PIN_1 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
  LL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  Configures and enable the Ethernet global interrupt.
  * @param  None
  * @retval None
  */
static void ETH_NVIC_Config(void)
{
  NVIC_SetPriority(ETH_IRQn, 7);  
  NVIC_EnableIRQ(ETH_IRQn);
}

/**
  * @brief  Stop the Ethernet MAC Interface
  * @param  None
  * @retval None
  */
void ETH_BSP_PhyStop(void)
{
  ETH_Stop();
}

/**
  * @brief  Reconfigures the Ethernet MAC Interface
  * @param  None
  * @retval None
  */
void ETH_BSP_PhyRestart(void)
{
  __IO uint32_t delay = 0;
  __IO uint32_t timeout = 0;
  uint32_t tmpreg,RegValue;
  
  /* Restart the autonegotiation */
  if(ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
  {
    /* Reset Timeout counter */
    timeout = 0;

    /* Enable Auto-Negotiation */
    ETH_WritePHYRegister(LAN8742A_PHY_ADDRESS, PHY_BCR, PHY_AutoNegotiation);

    /* Wait until the auto-negotiation will be completed */
    do
    {
      timeout++;
    } while (!(ETH_ReadPHYRegister(LAN8742A_PHY_ADDRESS, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO));

    /* Reset Timeout counter */
    timeout = 0;

    /* Read the result of the auto-negotiation */
    RegValue = ETH_ReadPHYRegister(LAN8742A_PHY_ADDRESS, PHY_SR);
    
    /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
    if((RegValue & PHY_DUPLEX_STATUS) != (uint32_t)RESET)
    {
      /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
      ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
    }
    else
    {
      /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
      ETH_InitStructure.ETH_Mode = ETH_Mode_HalfDuplex;
    }
    /* Configure the MAC with the speed fixed by the auto-negotiation process */
    if(RegValue & PHY_SPEED_STATUS)
    {
      /* Set Ethernet speed to 10M following the auto-negotiation */
      ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
    }
    else
    {
      /* Set Ethernet speed to 100M following the auto-negotiation */
      ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
    }
    
    /*------------------------ ETHERNET MACCR Re-Configuration --------------------*/
    /* Get the ETHERNET MACCR value */  
    tmpreg = ETH->MACCR;

    /* Set the FES bit according to ETH_Speed value */ 
    /* Set the DM bit according to ETH_Mode value */ 
    tmpreg |= (uint32_t)(ETH_InitStructure.ETH_Speed | ETH_InitStructure.ETH_Mode);

    /* Write to ETHERNET MACCR */
    ETH->MACCR = (uint32_t)tmpreg;
    
    /* Delay */
    delay = ETH_REG_WRITE_DELAY;
    while( delay-- );
    
    tmpreg = ETH->MACCR;
    ETH->MACCR = tmpreg;
  }
  
  /* Restart MAC interface */
  ETH_Start();
}

/**
  * @brief  Get Link status
  * @param  None
  * @retval None
  */
uint8_t ETH_BSP_PhyGetLinkStatus(void)
{
  /* Get Ethernet link status*/
  if( ETH_ReadPHYRegister(LAN8742A_PHY_ADDRESS, PHY_BSR) & PHY_Linked_Status )
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Check is frame received.
  * @param  None
  * @retval None
  */
uint32_t ETH_BSP_ReceivedCheck(void)
{
  return ETH_CheckFrameReceived();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
