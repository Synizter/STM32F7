/* Private include -----------------------------------------------------------*/
#include "task_supervisor.h"
#include "perf_config.h"

/* Private functions ---------------------------------------------------------*/

void System_SetTaskDeadline(System_TaskSupervisor* instance, uint16_t dl_time)
{
  instance->task_deadline = dl_time;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint16_t System_GetTaskDeadline(System_TaskSupervisor* instance)
{
  return instance->task_deadline;
}

uint16_t System_GetTaskEXETime(System_TaskSupervisor* instance)
{
  return  ((float)1/(SystemCoreClock/1000) * System_GetElapseTime(instance)) * 1000;
}

void System_StartCounter(System_TaskSupervisor* instance)
{
  instance->task_timestamp = System_GetCounter();
}

uint32_t System_GetElapseTime(System_TaskSupervisor* instance)
{
  return System_GetCounter() - instance->task_timestamp;
}

