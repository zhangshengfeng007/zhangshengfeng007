#ifndef _ADC_DRIVER_H
#define _ADC_DRIVER_H

#include "system.h"

#define		ADCREFRATIO			4000


#define		ADCChanne0			(0x00)
#define		ADCChanne2			(0x02)
#define		ADCChanne3			(0x03)
#define		ADCChanne6			(0x06)
#define		ADCChannel12		(0x0c)

#define		ADCChannelNTC		ADCChanne0
#define		ADCChannel400V		ADCChanne2
#define		ADCChannelFAN		ADCChanne3
#define		ADCChannel12V		ADCChanne6

uint16_t Get_ADC_Value(uint8_t channel);
void ADCInit(void);

#endif
