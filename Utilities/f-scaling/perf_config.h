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

void TaskPerf_ClockRateSwitch(uint16_t);
uint8_t TaskPerf_isOnStandby();

#endif