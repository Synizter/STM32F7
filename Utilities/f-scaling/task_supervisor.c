/* Private include -----------------------------------------------------------*/
#include "task_supervisor.h"

//TEST
#include "stm32f7xx_ll_gpio.h"

/* Private functions ---------------------------------------------------------*/
uint16_t SYS_TASK_DEADLINE = 0;

void System_SetTaskDeadline(uint16_t dl_time)
{
  SYS_TASK_DEADLINE = dl_time;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint16_t System_GetTaskDeadline()
{
  return SYS_TASK_DEADLINE;
}

float System_GetTaskEXETime()
{
  return  ((float)1/(SystemCoreClock/1000) * System_GetCounter());
}


