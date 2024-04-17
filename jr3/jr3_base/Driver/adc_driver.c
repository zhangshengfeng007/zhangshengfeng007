#include "adc_driver.h"

#define		ADCEn			(1 << 7)
#define		GCHS			(0x40)			//enable ADC channel
#define		ADCKS			(0x02 << 4)		//ADC clock = fosc/1
#define		VHSH			(0 << 2)		//Vrefh = VDD
#define		VHSL			(0x02 << 0)
#define		ADCStart		(0x40)			//start ADC conversion
#define		EOC				(0x20)			//ADC Status



void ADCInit(void)
{
	ADM &= ~0x0c;
	// set AIN5 pin¡¯s mode at pure analog pin
	P2CON |= 0x0d; 	//AIN0/P20 AIN2/P22 AIN3/P23
	P1CON |= 0x80;	//AIN6/P17
	// configure ADC channel and enable ADC.
	ADM |= ADCEn;
	ADM |= ADCChannel400V;
	// enable channel and select conversion speed
	ADR |= GCHS;
	ADR |= ADCKS;
	// configure reference voltage
	VREFH = VHSH | VHSL;
	// start ADC conversion
	//ADM |= ADCStart;
	Get_ADC_Value(ADCChannel400V);
	
//	if(Supply_Test()){
//		PowerOnOff_Set(0);
//		ErrorType_Set(ERROR_TYPE_SUPPLY);
//	}
}

uint16_t Get_ADC_Value(uint8_t channel)
{
	uint16_t temp_ad_data = 0;
	
	// configure ADC channel and enable ADC.
	ADM &= 0xf0;
	ADM |= channel;
	// start ADC conversion
	ADM |= ADCStart;
	while((ADM&EOC)!= EOC);
	ADM &= ~ADCStart;
	
	temp_ad_data = ADB;
	temp_ad_data = (temp_ad_data<<4) + (ADR&0x0f);
	
	return temp_ad_data;
}




