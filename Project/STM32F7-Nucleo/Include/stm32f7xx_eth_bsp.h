
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F7x6_ETH_BSP_H
#define __STM32F7x6_ETH_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"

#include "lwip/netif.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define DP83848_PHY_ADDRESS       0x01 /* Relative to STM324xG-EVAL Board */
#define LAN8720_PHY_ADDRESS       0x00 /* Relative to STM324xG-EVAL Board */
#define LAN8742A_PHY_ADDRESS      0x00 /* Relative to STM32F746GDISCOVERY Board */
   
/* MII and RMII mode selection, for STM324xG-EVAL Board(MB786) RevB ***********/
#define RMII_MODE  // User have to provide the 50 MHz clock by soldering a 50 MHz
                     // oscillator (ref SM7745HEV-50.0M or equivalent) on the U3
                     // footprint located under CN3 and also removing jumper on JP5. 
                     // This oscillator is not provided with the board. 
                     // For more details, please refer to STM3240G-EVAL evaluation
                     // board User manual (UM1461).


//#define MII_MODE

/* Uncomment the define below to clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef 	MII_MODE
 #define PHY_CLOCK_MCO
#endif

/* Static IP - Address */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   10

/* Static IP - Netmask */
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/* Static IP - Gateway Address */
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1
   
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void    ETH_BSP_Config(void);
void    ETH_BSP_PhyStop(void);
void    ETH_BSP_PhyRestart(void);
uint8_t ETH_BSP_PhyGetLinkStatus(void);
uint32_t ETH_BSP_ReceivedCheck(void);
#ifdef __cplusplus
}
#endif

#endif /* __STM32F7x6_ETH_BSP_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
