#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <SN8F5703.h>

#define		CHARGE_FRE		800		//480

#include "work.h"
#include "display.h"
#include "key.h"
#include "fan.h"
#include "touch_key.h"
#include "charge.h"
#include "touch.h"
#include "ntc.h"
#include "error_det.h"
#include "sleep.h"
#include "burnin.h"

#include "pwm_driver.h"
#include "adc_driver.h"
#include "timer.h"
#include "bs83b.h"
#include "74HC595.h"
#include "rotary.h"



//touch
sbit		SDA					=	P0^2;		//T_DATA
sbit		SCL					=	P0^3;		//T_CLOCK
//led
sbit		LEDB				=	P0^4;		//LEDB
sbit		SER					=	P0^5;		//SDIO
sbit		SCK					=	P0^6;		//SCKO
sbit		RCK					=	P0^7;		//LCKO

sbit		IPL					=	P1^0;
sbit		RELEASE_400V		=	P1^1;
sbit		KEY_LIGHT			=	P1^2;
sbit		TOUCH				=	P1^3;
sbit		FAN					=	P2^5;
sbit		CHARGE				=	P2^4;
//rotary
sbit		ROTARY_0			=	P1^4;
sbit		ROTARY_1			=	P1^5;
sbit		ROTARY_2			=	P1^6;
//ADC
sbit		VB1					=	P1^7;		//12Vadc
sbit		ADC_FAN				=	P2^3;
sbit		ADC_400V			=	P2^2;
sbit		ADC_NTC				=	P2^0;

#define		ARRAY_SIZE		13

typedef struct
{
	uint8_t flags;
	void (*handle)();
}TASK;

typedef struct
{
	uint16_t systick;
}TIM_Period_T;

#define		true		1
#define		false		0

extern TIM_Period_T TIM_Period;

void GPIO_Init(void);
void Task_Roll(void);
void SYSCLK_Init(void);
void System_Init(void);
void System_Run(void);

#endif
