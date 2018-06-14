#ifndef __SYS_MACRO_H__
#define __SYS_MACRO_H__

#include "FreeRTOS.h"
#include "task_supervisor.h"
#include "perf_config.h"

extern TaskHandle_t pxPrevTask;
static System_TaskSupervisor currentTaskInstance;

#define traceTASK_SWITCH_OUT()         pxPrevTask = pxCurrentTCB
#define traceTASK_SWITCHED_IN()                                                    	                        \
    if( pxCurrentTCB != pxPrevTask )                                      	                                \
    {                                                                              	                        \
      currentTaskInstance.task_tcb = xTaskGetCurrentTaskHandle();                                                   \
      currentTaskInstance = (System_TaskSupervisor*)pvTaskGetThreadLocalStoragePointer(currentTaskInstance.tcb,0);  \
      System_StartCounter(&currentTaskInstance);                                                                \
      /* Set Clockrate of Task */                                                                               \
      Task_SetTaskOpClockRate(&currentTaskInstance);							        \
      if(currentTaskInstance.isDeadlineMiss == 1)                                                               \
      {                                                                                                         \
        currentTaskInstance.isDeadlineMiss = 0;                                                                 \
        currentTaskInstance.task_opf += 1;                                                                      \
      }                                                                                                         \
    }                                                                                                           \
      
#endif /* __SYS_MACRO_H__ */