#ifndef __DRV_ADC_H
#define __DRV_ADC_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/
extern volatile uint16_t adc_value[4];


/* Exported functions ------------------------------------------------------- */


void Adc_Init(void);
void Adc_Clock_Config(void);
void Adc_Gpio_Config(void);
void Adc_Dma_Config(void);
void Adc_Config(void);

void Adc_DeInit(void);
void Adc_ReInit(void);

#endif

