#ifndef __PERF_CONFIG_H__
#define __PERF_CONFIG_H__

#include "stm32f7xx.h"
#include "cmsis_os.h"

#define LOWER_BND_LOW_CLK_RATE			60
#define UPPER_BND_LOW_CLK_RATE			144

#define MID_CLK_RATE                            168

#define LOWER_BND_HIGH_CLK_RATE			180
#define UPPER_BND_HIGH_CLK_RATE			216


#define MID_CLK_RANGE				(UPPER_BND_MID_CLK_RATE - LOWER_BND_MID_CLK_RATE) + 1 
#define HIGH_CLK_RANGE				(UPPER_BND_HIGH_CLK_RATE - LOWER_BND_HIGH_CLK_RATE) + 1

typedef struct PLLParamerter_Type
{
  uint32_t clock_src;
  uint32_t pll_m;
  uint32_t pll_n;
  uint32_t pll_p;
}PLLParamCon_TypeDef;

typedef struct TaskPreference_Type
{
  TaskHandle_t Task_Instance;
  PLLParamCon_TypeDef Task_OptFreq;
  float Task_OnChipWL;
  float Task_OffChipWL; 
  uint16_t Task_Deadline;
}TaskPerfConf_t;

void TaskPerf_ClockRateSwitch(TaskPerfConf_t*, uint16_t);
void TEST_FUNC_TaskPerf_ClockRateSwitch(uint16_t);
void TaskPerf_Evauate(TaskPerfConf_t*);

#endif