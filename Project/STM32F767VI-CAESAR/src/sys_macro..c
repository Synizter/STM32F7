#include "perf_config.h"
#include "task_supervisor.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

extern TaskHandle_t pxPrevTCB;
static TaskHandle_t pxCurrentTCB;
static System_TaskSupervisor currentTaskInstance;

void CONTEXT_SWITCH_OUT()
{
  pxCurrentTCB = xTaskGetCurrentTaskHandle();
  pxPrevTCB = pxCurrentTCB;
}

void CONTEXT_SWITCH_IN()
{
    if( pxCurrentTCB != pxPrevTCB )                                      	                        
    {                                                                              	                
      currentTaskInstance.task_tcb = xTaskGetCurrentTaskHandle();                                           
      currentTaskInstance = (System_TaskSupervisor*)pvTaskGetThreadLocalStoragePointer(currentTaskInstance.task_tcb,0);  
      System_StartCounter(&currentTaskInstance);                                                        
      /* Set Clockrate of Task */                                                                       
      Task_SetTaskOpClockRate(&currentTaskInstance);							
      if(currentTaskInstance.isDeadlineMiss == 1)                                                       
      {                                                                                                 
        currentTaskInstance.isDeadlineMiss = 0;                                                         
        currentTaskInstance.task_opf += 1;                                                              
      }                                                                                                 
    }                                                                                                   
      
}