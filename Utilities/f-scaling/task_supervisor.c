/* Private include -----------------------------------------------------------*/
#include "task_supervisor.h"
#include "perf_config.h"

/* Private Varirable*/
static uint32_t s;
static uint32_t e;
static uint32_t t;

/* Private functions ---------------------------------------------------------*/

uint16_t System_GetTaskDeadline(System_TaskSupervisor* instance)
{
  return instance->task_deadline;
}

uint16_t System_GetTaskEXETime(System_TaskSupervisor* instance)
{
  return (System_GetElapseTime(instance) -(instance->task_timestamp)) * (1.0/SystemCoreClock) * 1000;
}

void System_StartCounter(System_TaskSupervisor* instance)
{
  instance->task_timestamp = System_GetCounter();
  t = instance->task_timestamp;
  s = t;
}

uint32_t System_GetElapseTime(System_TaskSupervisor* instance)
{
  uint16_t elapse = 0;
  if(System_GetCounter() > s)
  {
    e = System_GetCounter();
    elapse = e - s;
    s = System_GetCounter();
  }
  else
  {
    e = System_GetCounter();
    elapse = e - s;
    s = System_GetCounter();
  }
  t += elapse;
  return t;
}

