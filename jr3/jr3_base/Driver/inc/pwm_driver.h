#ifndef _PWM_DRIVER_H
#define _PWM_DRIVER_H

#include "system.h"


//CHARGE
#define		PWCH11		(0x02)
//FAN
#define		PWCH21		(0x20)

#define		Pwm_Open_Fan()				PW1CH|=PWCH21
#define		Pwm_Open_Charge()			{P2M&=~0x10;Pwm_Set(CHARGE_FRE,0);PW1CH|=PWCH11;}
#define		Pwm_Close_Fan()				PW1CH&=~PWCH21
#define		Pwm_Close_Charge()			{PW1CH&=~PWCH11;P2M|=0x10;CHARGE=0;}

void Pwm_Init(void);
void Pwm_Set(uint16_t Frq_value,uint16_t duty_value);

#endif
