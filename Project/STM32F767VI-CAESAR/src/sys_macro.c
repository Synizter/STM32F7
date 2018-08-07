/*
  To do list
  [x] Count cycle of task
  [ ] Changing of clock rate at every execution
  [ ] Compare each cycle of task due to different clock rate in order to
      identify a verge of exeution time chaning
*/


#include "perf_config.h"
#include "task_supervisor.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

extern TaskHandle_t pxPrevTCB;
extern TaskHandle_t pxCurrentTCB;
static System_TaskSupervisor* currentTaskInstance;
uint32_t cycle_tmp = 0;

void CONTEXT_SWITCH_OUT()
{
  /*Task done execution, get counter*/
  cycle_tmp = System_GetCounter();
  if(currentTaskInstance->task_timestamp > cycle_tmp)
    currentTaskInstance->task_cycle = cycle_tmp - (0xFFFFFFFF - currentTaskInstance->task_timestamp);
  else
    currentTaskInstance->task_cycle = cycle_tmp - (currentTaskInstance->task_timestamp); 
  pxPrevTCB = pxCurrentTCB;
}

void CONTEXT_SWITCH_IN()
{
  if( pxCurrentTCB != pxPrevTCB )                                      	                        
  {  
    /* Retrieve Data of current running task */
    currentTaskInstance->task_tcb = xTaskGetCurrentTaskHandle();                                           
    currentTaskInstance = (System_TaskSupervisor*) pvTaskGetThreadLocalStoragePointer(currentTaskInstance->task_tcb,0);  
    
    /*Optimize of task*/
    if(currentTaskInstance->isOptimize == 1)
    {
      currentTaskInstance->task_prv_cycle = currentTaskInstance->task_cycle;
    }
    System_StartCounter(currentTaskInstance);                                                        
    /* Set Clockrate of Task */                                                                       
    Task_SetTaskOpClockRate(currentTaskInstance);							                                                                                          
  }                                                                                                   
}