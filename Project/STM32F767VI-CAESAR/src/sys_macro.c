#include "perf_config.h"
#include "task_supervisor.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

extern TaskHandle_t pxPrevTCB;
extern TaskHandle_t pxCurrentTCB;
static System_TaskSupervisor* currentTaskInstance;

void CONTEXT_SWITCH_OUT()
{
  currentTaskInstance->task_tcb = xTaskGetCurrentTaskHandle();                                           
  currentTaskInstance = (System_TaskSupervisor*) pvTaskGetThreadLocalStoragePointer(currentTaskInstance->task_tcb,0);  
  currentTaskInstance->task_exe_time = System_GetTaskEXETime(currentTaskInstance);
  
  if(currentTaskInstance->isDeadlineMiss == 0 && currentTaskInstance->swing_cnt < 5)                                                       
  {                                                                                                 
     currentTaskInstance->isDeadlineMiss = 1;
     currentTaskInstance->task_prv_opf = currentTaskInstance->task_opf;
     currentTaskInstance->task_opf -= 1;                                                              
  }
  else if(currentTaskInstance->isDeadlineMiss == 1 && currentTaskInstance->swing_cnt < 5)
  {
     currentTaskInstance->isDeadlineMiss = 0;
     currentTaskInstance->task_opf += 1;    
     currentTaskInstance->swing_cnt += 1;
  }
  else
  {
    currentTaskInstance->task_prv_opf = currentTaskInstance->task_opf;
  }
  
  
  pxPrevTCB = pxCurrentTCB;
}

void CONTEXT_SWITCH_IN()
{
    if( pxCurrentTCB != pxPrevTCB )                                      	                        
    {                                                                              	                
      currentTaskInstance->task_tcb = xTaskGetCurrentTaskHandle();                                           
      currentTaskInstance = (System_TaskSupervisor*) pvTaskGetThreadLocalStoragePointer(currentTaskInstance->task_tcb,0);  
      System_StartCounter(currentTaskInstance);                                                        
      /* Set Clockrate of Task */                                                                       
      Task_SetTaskOpClockRate(currentTaskInstance);							
                                                                                            
    }                                                                                                   
      
}