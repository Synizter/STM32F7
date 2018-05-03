#ifndef __CURR_MEAS_H__
#define __CURR_MEAS_H__

#include "stm32f7xx.h"
#include "stm32f7xx_ll_adc.h"
#include "stm32f7xx_ll_dma.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_gpio.h"

#define ADC_CALIBRATION_TIMEOUT_MS       ((uint32_t)   1)
#define ADC_ENABLE_TIMEOUT_MS            ((uint32_t)   1)
#define ADC_DISABLE_TIMEOUT_MS           ((uint32_t)   1)
#define ADC_STOP_CONVERSION_TIMEOUT_MS   ((uint32_t)   1)
#define ADC_CONVERSION_TIMEOUT_MS        ((uint32_t)   2)

/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       ((uint32_t)3300)

/* Definitions of data related to this example */
  /* Definition of ADCx conversions data table size */
  #define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)   4)

  /* Init variable out of expected ADC conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


void Configure_DMA(void);
void Configure_ADC(void);
void Activate_ADC(void);
void MeasurementSystem_Init();
void ADCStart_Meas(void);
uint16_t GetADC_Value(void);

#endif