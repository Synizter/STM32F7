#ifndef __TASK_SUPERVISOR_H__
#define __TASK_SUPERVISOR_H__

/* Private Include ------------------------------------------------------------*/
#include "stm32f7xx_ll_tim.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx.h"
#include "cmsis_os.h"
/* Private definition --------------------------------------------------------*/
#define MAX_SYS_TASK                            10

/* Private Typedef -----------------------------------------------------------*/
typedef struct
{
  
  uint32_t task_timestamp;
  uint32_t task_current_tick;
  
  uint16_t task_deadline;
  
  TaskHandle_t task_tcb;
  
}System_TaskSupervisor;

/* Prototpye ------------------------------------------------------------------*/
uint16_t System_GetTaskDeadline();
void System_SetTaskDeadline(System_TaskSupervisor* instance, uint16_t dl_time);
float System_GetTaskEXETime();
void System_StartCounter();
void System_StopCounter();
uint32_t System_GetElapseTime();

/* Inline Function ------------------------------------------------------------*/
__STATIC_INLINE void System_SuperviosrInit() 
{
  /* Enable Cycle Counter for DVFS*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->LAR = 0xC5ACCE55;  //Unlock register access 
  DWT->CYCCNT = 0;
}

__STATIC_INLINE uint64_t System_GetCounter()
{
  return DWT->CYCCNT;
}

__STATIC_INLINE void System_ResetCounter()
{
  /*Disable Counter*/
  DWT->CTRL &= ~(1 << 0);
  DWT->CYCCNT = 0;
}

__STATIC_INLINE uint8_t System_isCounterENA()
{
  return (uint8_t)0x0 | DWT->CTRL;
}




#endif