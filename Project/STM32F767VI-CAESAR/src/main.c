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
#include "semphr.h"

/**BENCHMARKING**/
#include "bb_sort.h"
#include "md5.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

TaskHandle_t pxPrevTCB;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
//DO NOT TOUCH THIS SHIT
//static void CPU_CACHE_Enable(void);



/* Private functions ---------------------------------------------------------*/
//static void BB_SORT_TASK(void* param);
//static void MD5_TASK(void* param);

/*TEST case*/
static void DATA_ACQUISTION(void* param);
static void PRE_PROCESS(void* param);
static void CHECK_SUM(void* param);
System_TaskSupervisor DATA_ACQUISTION_INSTANCE;
System_TaskSupervisor PRE_PROCESS_INSTANCE;
System_TaskSupervisor CHECK_SUM_INSTANCE;

SemaphoreHandle_t data_acq_sem;
SemaphoreHandle_t pre_proc_sem;
SemaphoreHandle_t check_sum_sem;
uint8_t buffer[MAX_ARRAY_LEN] = {0};


/*IMPLEMENT Case based on CAESAR BOT*/
//static void ON_CMD_RECV_WAKE(void* param);
//static void CMD_PROCESS(void* param);
//static void uPYTHON_EXE(void* param);

void TEST_CASE(void);
void vApplicationIdleHook( void );

/*Stat Variable --------------------------------------------------------------*/

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
  
  TEST_CASE();
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_14, LL_GPIO_PULL_NO);
  
  LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_14);
    /*TEST**/
  Task_SetTaskOpClockRate(&PRE_PROCESS_INSTANCE);
  /* Start scheduler */

  vTaskStartScheduler();
  
  /* Infinite loop */
  while (1);
}

/* System Task ----------------------------------------------------------------*/
void DATA_ACQUISTION(void* param) 
{
  xSemaphoreGive(data_acq_sem);
  (void) param;
  
  for(;;) 
  {                                                                                                                            	
    if(xSemaphoreTake(data_acq_sem, portMAX_DELAY) == pdTRUE)
    {
      Sensor_Read(buffer);
      xSemaphoreGive(pre_proc_sem);
    }
    else
    {}
  }
}

uint32_t cycle_start[123];
uint32_t cycle_end[123];
uint8_t idx = 0;
uint8_t f[123];

void gen_f()
{
  uint8_t in = 0;
  uint8_t i = 0;
  for(i = 216; i >= 60; --i)
  {
    if(i == 179)
      i = 168;
    else if(i == 167)
      i = 144;
    f[in++] = i; 
  }
}
void TEST_CASE(void)
{
  /* Add your application code here */  
//  DATA_ACQUISTION_INSTANCE.isOptimize = 0;
//  System_TaskCreate(DATA_ACQUISTION, 
//                    "Pre-process Task", 
//                    configMINIMAL_STACK_SIZE * 2, 
//                    NULL, 
//                    tskIDLE_PRIORITY + 2,
//                    &DATA_ACQUISTION_INSTANCE,
//                    300,
//                    TASK_MAX_PERF);
//    PRE_PROCESS_INSTANCE.isOptimize = 1;
    System_TaskCreate(PRE_PROCESS, 
                    "Pre-process Task", 
                    configMINIMAL_STACK_SIZE * 2, 
                    NULL, 
                    tskIDLE_PRIORITY + 2,
                    &PRE_PROCESS_INSTANCE,
                    300,
                    180);
//    CHECK_SUM_INSTANCE.isOptimize = 1;
//    System_TaskCreate(CHECK_SUM, 
//                    "Check-sum Task", 
//                    configMINIMAL_STACK_SIZE * 2, 
//                    NULL, 
//                    tskIDLE_PRIORITY + 3,
//                    &CHECK_SUM_INSTANCE,
//                    200,
//                    TASK_MAX_PERF);
//    
//    /*Semaphore Create*/
//    data_acq_sem = xSemaphoreCreateBinary();
//    pre_proc_sem = xSemaphoreCreateBinary();
//    check_sum_sem = xSemaphoreCreateBinary();

}
void PRE_PROCESS(void* param)
{
  Sensor_Read(buffer);
  (void) param;
  for(;;)
  {
    
    LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_14);
////    if(xSemaphoreTake(pre_proc_sem, portMAX_DELAY) == pdTRUE)
////    {
      
      BubbleSort(buffer);
      
//      xSemaphoreGive(check_sum_sem);
//    }
//    else
////    {}
      LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_14);
  }
}

UL_MD5Context md5_context;

uint8_t md5_ch1[UL_MD5LENGTH];
uint8_t md5_ch2[UL_MD5LENGTH];
uint8_t md5_ch3[UL_MD5LENGTH];
uint8_t md5_ch4[UL_MD5LENGTH];
uint8_t temp[64];

void CHECK_SUM(void* param)
{
  (void) param;
  uint16_t chunk;
  uint8_t i;
  
  for(;;)
  {
//    if(xSemaphoreTake(check_sum_sem, portMAX_DELAY) == pdTRUE)
//    {
      
      ul_MD5Init(&md5_context);
      /* encrype MD5 */
      for(chunk = 0; chunk < MAX_ARRAY_LEN/64; ++chunk)
      {
        switch (chunk)
        {
        case 1:
          /*Copy data into buffer*/
          for(i = 0; i < 64; ++i)
          {
            temp[i] = buffer[i];
          }
          ul_MD5Update(&md5_context, temp, 64);
          ul_MD5Final(md5_ch1, &md5_context);
          break;
        case 2:
          /*Copy data into buffer*/
          for(i = 64; i < (64 * 2); ++i)
          {
            temp[i - 64] = buffer[i];
          }
          ul_MD5Update(&md5_context, temp, 64);
          ul_MD5Final(md5_ch2, &md5_context);
          break;
        case 3:
          /*Copy data into buffer*/
          for(i = 128; i < (64 * 3); ++i)
          {
            temp[i - 128] = buffer[i];
          }
          ul_MD5Update(&md5_context, temp, 64);
          ul_MD5Final(md5_ch3, &md5_context);
          break;
        case 4:
          /*Copy data into buffer*/
          for(i = 192; i < (64 * 4); ++i)
          {
            temp[i - 192] = buffer[i];
          }
          ul_MD5Update(&md5_context, temp, 64);
          ul_MD5Final(md5_ch4, &md5_context);
          break;
        }
      }
//      xSemaphoreGive(data_acq_sem);
//    }
//    else
//    {}
  }
}
                    
void vApplicationIdleHook(void) 
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
