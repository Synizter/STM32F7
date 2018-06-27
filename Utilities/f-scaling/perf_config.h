#ifndef __PERF_CONFIG_H__
#define __PERF_CONFIG_H__

#include "stm32f7xx.h"
#include "task_supervisor.h"
#include "stm32f7xx_ll_utils.h"

#define LOWER_BND_LOW_CLK_RATE			60
#define UPPER_BND_LOW_CLK_RATE			144

#define MID_CLK_RATE                            168

#define LOWER_BND_HIGH_CLK_RATE			180
#define UPPER_BND_HIGH_CLK_RATE			216

#define TASK_MAX_PERF                           UPPER_BND_HIGH_CLK_RATE
#define MID_CLK_RANGE				(UPPER_BND_MID_CLK_RATE - LOWER_BND_MID_CLK_RATE) + 1 
#define HIGH_CLK_RANGE				(UPPER_BND_HIGH_CLK_RATE - LOWER_BND_HIGH_CLK_RATE) + 1

typedef struct PLLParamerter_Type
{
  uint32_t clock_src;
  uint32_t PLLM;
  uint32_t PLLN;
  uint32_t PLLP;
  uint32_t AHBDiv;
  uint32_t APB1Div;
  uint32_t APB2Div;
}PLLParamCon_TypeDef;

void ClockRateSwitch(uint16_t);
uint8_t TaskPerf_isOnStandby();
void Task_SetTaskOpClockRate(System_TaskSupervisor*);



#endif