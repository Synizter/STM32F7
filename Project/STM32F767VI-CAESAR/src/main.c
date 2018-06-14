/**
  ******************************************************************************
  * @file   main.c 
  * @author  GORAGOD P
  * @version V1.0.0
  * @date    30-December-2016
  * @brief   Main program body through the LL API
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define T1_F              60
#define T1_DL             2

TaskHandle_t pxPrevTCB;

System_TaskSupervisor SystemTask1_Instance;
System_TaskSupervisor SystemTask2_Instance;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);

//DO NOT TOUCH THIS SHIT
//static void CPU_CACHE_Enable(void);

static void SysTask_1(void* parameter);
static void SysTask_2(void* parameter);
void vApplicationIdleHook( void );

/* Private functions ---------------------------------------------------------*/
__STATIC_INLINE BaseType_t System_TaskCreate(TaskFunction_t pxTaskCodeconst, 
                                                        char * const pcName,
							const uint16_t usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
                                                        System_TaskSupervisor * pvValueCreatedTask,
                                                        uint16_t xTaskDL,
                                                        uint8_t xTaskCR)
{
  BaseType_t xReturn;
  
    xReturn = xTaskCreate( pxTaskCodeconst,
               pcName,
               usStackDepth,
               pvParameters,
               uxPriority,
               pvValueCreatedTask->task_tcb);
  
  /* Set parameter field for task supervisor  YOU may add your own field by modifying @System_TaskSupervisor struct*/
  pvValueCreatedTask->task_deadline = xTaskDL;
  pvValueCreatedTask->task_opf = xTaskCR;
  pvValueCreatedTask->isDeadlineMiss = 0;
  
  /* Attach params to task stack */
   vTaskSetThreadLocalStoragePointer(pvValueCreatedTask->task_tcb, 0, (void*)pvValueCreatedTask);
  return xReturn;
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

int main(void)
{
  /* Configure the MPU attributes as Write Through */
  MPU_Config();
  /* Disable the CPU Cache */

  /* Configure the system clock to 216 MHz */
  SystemClock_Config();
  MeasurementSystem_Init();
  System_SuperviosrInit();
  
  /* Add your application code here */  
  System_TaskCreate(SysTask_1, 
                    "System Task 1", 
                    configMINIMAL_STACK_SIZE * 2, 
                    NULL, 
                    tskIDLE_PRIORITY + 1,
                    &SystemTask1_Instance,
                    100,
                    60);
    System_TaskCreate(SysTask_2, 
                    "System Task 2", 
                    configMINIMAL_STACK_SIZE * 2, 
                    NULL, 
                    tskIDLE_PRIORITY + 1,
                    &SystemTask2_Instance,
                    100,
                    200);
  
  
  /* Start scheduler */
  vTaskStartScheduler();
  /* Infinite loop */
  
  while (1)
  {
//    if (ubDmaTransferStatus == 2)
//    {
//      /* Update status variable of DMA transfer */
//      ubDmaTransferStatus = 0;
//    }
//    LL_ADC_REG_StartConversionSWStart(ADC1);

  }
}
/* System Task ----------------------------------------------------------------*/
uint32_t s;

void SysTask_1(void* parameter) 
{
  (void) parameter;
  //Task_SetTaskOpClockRate(&SystemTask1_Instance, SystemTask1_Instance.task_opf);
  for(;;) 
  {  
    while(s--);
    s = 0x0FFFFFF;
     vTaskDelay(10);

  }
}

void SysTask_2(void* parameter) 
{
  (void) parameter;

  for(;;) 
  {    
     vTaskDelay(100);
  }
}
                    
void vApplicationIdleHook(void) 
{      
  while(1)
  {
    __NOP();
  }
}

void vApplicationTickHook(void)
{
  
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
   
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1);
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady()!= 1);
  
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_7);

  /* Enable PWR clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Activation OverDrive Mode */
  LL_PWR_EnableOverDriveMode();
  while(LL_PWR_IsActiveFlag_OD() != 1)
  {
  };

  /* Activation OverDrive Switching */
  LL_PWR_EnableOverDriveSwitching();
  while(LL_PWR_IsActiveFlag_ODSW() != 1)
  {
  };
  

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 432, LL_RCC_PLLP_DIV_2);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  
  /* Set systick to 1ms */
  //SysTick_Config(216000000 / 1000);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(216000000);
}

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  /* Disable MPU */
  LL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER0, 0x00, 0x20020000UL, 
         LL_MPU_REGION_SIZE_256KB | LL_MPU_REGION_FULL_ACCESS | LL_MPU_ACCESS_NOT_BUFFERABLE |
         LL_MPU_ACCESS_NOT_CACHEABLE | LL_MPU_ACCESS_SHAREABLE | LL_MPU_TEX_LEVEL0 |
         LL_MPU_INSTRUCTION_ACCESS_ENABLE);

  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
//DO NOT TOUCH THIS SHIT
//static void CPU_CACHE_Enable(void)
//{
//  /* Enable I-Cache */
//  SCB_EnableICache();
//
//  /* Enable D-Cache */
//  SCB_EnableDCache();
//}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
