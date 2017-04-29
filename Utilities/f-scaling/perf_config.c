#include "perf_config.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_rcc.h"

/* Private Prototype ******************************************************************/
/* Private Variable *******************************************************************/
const PLLParamCon_TypeDef ClockRateScale_Low[] = { 
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 120, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 122, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 124, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 126, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 128, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 130, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 132, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 134, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 136, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 138, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 140, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 142, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 144, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 146, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 148, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 150, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 152, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 154, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 156, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 158, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 160, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 162, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 164, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 166, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 170, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 172, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 174, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 176, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 178, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 180, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 182, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 184, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 186, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 188, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 190, LL_RCC_PLLP_DIV_4},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 96, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 97, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 98, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 99, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 100, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 101, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 102, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 103, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 104, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 105, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 106, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 107, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 108, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 109, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 110, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 111, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 112, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 113, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 114, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 115, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 116, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 117, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 118, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 119, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 120, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 121, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 122, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 123, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 124, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 125, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 126, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 127, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 128, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 129, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 130, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 131, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 132, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 133, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 134, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 135, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 136, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 137, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 138, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 139, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 140, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 141, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 142, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 143, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 144, LL_RCC_PLLP_DIV_2}
};
const PLLParamCon_TypeDef ClockRateScale_High[] = { 
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 180, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 181, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 182, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 183, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 184, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 185, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 186, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 187, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 188, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 189, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 190, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 191, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 192, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 193, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 194, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 195, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 196, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 197, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 198, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 199, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 200, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 201, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 202, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 203, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 204, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 205, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 206, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 207, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 208, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 209, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 210, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 211, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 212, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 213, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 214, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 215, LL_RCC_PLLP_DIV_2},
{LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 216, LL_RCC_PLLP_DIV_2}
};
const PLLParamCon_TypeDef ClockRateScale_Medium = {LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2};


void TaskPerf_ClockRateSwitch(TaskPerfConf_t* instance, uint16_t target_f)
{
	/*Low Range Frequency*/
	if (target_f >= LOWER_BND_LOW_CLK_RATE && target_f <= UPPER_BND_LOW_CLK_RATE)
	{
		/* Disable Overdrive Mode */
		LL_PWR_DisableOverDriveMode();
		/* Disable Overdrice Switching */
		LL_PWR_DisableOverDriveSwitching();
		/* Using Scale 3 Mode (Low) */
		LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);

		/* Copy map pll configuration to temp var */
		instance->Task_OptFreq = ClockRateScale_Low[target_f - LOWER_BND_LOW_CLK_RATE];
	}
	/*High Range Frequency*/
	else if (target_f >= LOWER_BND_HIGH_CLK_RATE && target_f <= UPPER_BND_HIGH_CLK_RATE)
	{
		/* Enable Overdrive Mode */
		LL_PWR_EnableOverDriveMode();
		/* Enable Overdrice Switching */
		LL_PWR_EnableOverDriveSwitching();
		/* Using Scale 1 Mode (High) */
		LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

		/* Copy map pll configuration to temp var */
		instance->Task_OptFreq = ClockRateScale_High[target_f - LOWER_BND_HIGH_CLK_RATE];
	}
	/*Medium Range Frequency*/
	else if (target_f == MID_CLK_RANGE)
	{
		/* Using Scale 1 Mode (High) */
		LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
		/* Copy map pll configuration to temp var */
		instance->Task_OptFreq = ClockRateScale_Medium;
	}

	else 
	{
		while(1);
	}

	LL_RCC_PLL_ConfigDomain_SYS(instance->Task_OptFreq.clock_src, 
								instance->Task_OptFreq.pll_m, 
								instance->Task_OptFreq.pll_n, 
								instance->Task_OptFreq.pll_p);
	
	SysTick_Config((target_f * 1000000) / 1000);
	SystemCoreClock = target_f * 1000000;
	SystemCoreClockUpdate();
}

PLLParamCon_TypeDef temp;
void TEST_FUNC_TaskPerf_ClockRateSwitch(uint16_t target_f) 
{
  /*Low Range Frequency -------------------------------------------------------*/
  if (target_f >= LOWER_BND_LOW_CLK_RATE && target_f <= UPPER_BND_LOW_CLK_RATE)
  {
    /* Disable Overdrive Mode */
    LL_PWR_DisableOverDriveMode();
    /* Disable Overdrice Switching */
    LL_PWR_DisableOverDriveSwitching();
    /* Using Scale 3 Mode (Low) */
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
    
    /* Copy map pll configuration to temp var */
    temp = ClockRateScale_Low[target_f - LOWER_BND_LOW_CLK_RATE];
  }
  /*Medium Range Frequency ----------------------------------------------------*/
  else if (target_f >= LOWER_BND_MID_CLK_RATE && target_f <= UPPER_BND_MID_CLK_RATE)
  {

    /* Check over-drive neccessary */
    if (target_f >= OVDR_MID_CLK_RATE) 
    {
      /* Enable Overdrive Mode */
      LL_PWR_EnableOverDriveMode();
      while(LL_PWR_IsActiveFlag_OD() != 1);
      /* Enable Overdrice Switching */
      LL_PWR_EnableOverDriveSwitching();  
      while(LL_PWR_IsActiveFlag_ODSW() != 1);
    }
    else
    {
      /* Disable Overdrive Mode */
      LL_PWR_DisableOverDriveMode();
      /* Disable Overdrice Switching */
      LL_PWR_DisableOverDriveSwitching();
    }
    /* Copy map pll configuration to temp var */
    /* Using Scale 2 Mode (Medium) */
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
    temp = ClockRateScale_Medium;
  }
  /*High Range Frequency ------------------------------------------------------------*/
  else if (target_f >= LOWER_BND_HIGH_CLK_RATE && target_f <= UPPER_BND_HIGH_CLK_RATE)
  {
    /* Enable Overdrive Mode */
    LL_PWR_EnableOverDriveMode();
    while(LL_PWR_IsActiveFlag_OD() != 1)
    /* Enable Overdrice Switching */
    LL_PWR_EnableOverDriveSwitching();  
    while(LL_PWR_IsActiveFlag_ODSW() != 1)
    /* Using Scale 1 Mode (High) */
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    
    /* Copy map pll configuration to temp var */
    temp = ClockRateScale_High[target_f - LOWER_BND_HIGH_CLK_RATE];
  }
  /* Insanity case, trap in inf loop */
  else 
  {
    while(1);
  }
  
  LL_RCC_PLL_ConfigDomain_SYS(temp.clock_src, 
                              temp.pll_m, 
                              temp.pll_n, 
                              temp.pll_p);
  
  SysTick_Config((target_f * 1000000) / 1000);
  SystemCoreClock = target_f * 1000000;
}