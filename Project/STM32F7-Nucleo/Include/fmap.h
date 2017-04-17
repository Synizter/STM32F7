#ifndef __FMAP_H__
#define __FMAP_H__

#include "stm32f7xx.h"

#define CLOCK_RATE_RANGE (216 - 24) + 1

typedef struct {
  	uint32_t clk_src;
	uint32_t m;
	uint32_t n;
	uint32_t p;
}PLLParam_TypeDef;

extern const PLLParam_TypeDef ClockRate_MHz[CLOCK_RATE_RANGE];
void ClockRate_Config(PLLParam_TypeDef*, uint32_t);
void TaskClockRateMHz_Config(uint32_t clk_rate);

#endif