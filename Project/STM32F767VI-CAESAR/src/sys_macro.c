#include "perf_config.h"
#include "task_supervisor.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

extern TaskHandle_t pxPrevTCB;
extern TaskHandle_t pxCurrentTCB;
static System_TaskSupervisor* currentTaskInstance;

void CONTEXT_SWITCH_OUT()
{
  pxPrevTCB = pxCurrentTCB;
}

void CONTEXT_SWITCH_IN()
{
    if( pxCurrentTCB != pxPrevTCB )                                      	                        
    {       
//      if(currentTaskInstance->isOptimize == 1)
//      {
//        currentTaskInstance->task_tcb = xTaskGetCurrentTaskHandle();                                           
//        currentTaskInstance = (System_TaskSupervisor*) pvTaskGetThreadLocalStoragePointer(currentTaskInstance->task_tcb,0);  
//        if(currentTaskInstance->isDeadlineMiss == 0 && currentTaskInstance->prevDeadlineMiss == 0)
//        {
//          currentTaskInstance->prevDeadlineMiss = 0;
//          currentTaskInstance->task_prv_opf = currentTaskInstance->task_opf;
//          currentTaskInstance->task_opf -= 1;
//        }
//        else if(currentTaskInstance->prevDeadlineMiss == 1)
//        {
//        
//        }
//        else
//        {
//          currentTaskInstance->prevDeadlineMiss = 1;
//          currentTaskInstance->isDeadlineMiss = 1;
//          //currentTaskInstance->task_opf = currentTaskInstance->task_prv_opf += 3;
//          //TEST
//          currentTaskInstance->task_opf += 5;
//        }
//      System_StartCounter(currentTaskInstance);                                                        
//      /* Set Clockrate of Task */                                                                       
//      Task_SetTaskOpClockRate(currentTaskInstance);							                                                                                          
//      }                                                                                                   
  }
}