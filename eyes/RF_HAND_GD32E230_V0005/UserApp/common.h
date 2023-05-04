#ifndef	_COMMON_H
#define _COMMON_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gd32e23x.h"

//0????Å≥?????????1????????????
#define _WDT_ENABLE 		1
#define _DEBUG_ 			1

#define _BAT_TEMP_ERR_ 		1
#define _BAT_LEVEL_ERR_ 	1
#define _RF_TEMP_ERR_		1


//??????Å≥???--- ????????Å≥???
#define OUTSIDE_Version       0x0005
//?????ï˜??--- ????ï˜??
#define INSIDE_Version        0x0051

//*******************************************************************************
// ?????????Çk????
//       EMS?Å≥???Çkms??+ ?????Å≥???Çkms??+ RF?Å≥???Çkms??+?????Å≥???Çkms??  = 1?????Å≥???Çkms??
//1?? ??  2.25           1.2           3.82?Çk??Å≥???     + 15  (??Å≥???   = 22.2
//2?? ??  2.32           1.2           11.1?Çk??Å≥???     +7.88 (??Å≥???	= 22.5
//??  ??  2.32           1.2           11.1?Çk??Å≥???     +7.88?Çk??Å≥???	= 22.5
//**********************************************************************************
//????????
#define _AUTO_OFF_			60	//????Å±Å≤Å±????????Å≥??S
#define _WAKE_OFF_			3	//???????????Å≥??S
#define _USE_OFF_			240	//??????Å±????????Å≥??S
#define _SHORT_TIME_		200	//???????Å≥??MS
#define _LONG_TIME_			500	//???????Å≥??MS

#define _RF_LEVEL1_TIME_	10	// 1??RF?????Å≥??MS
#define _RF_LEVEL2_TIME_	12	// 2??RF?????Å≥??MS
#define _RF_LEVEL3_TIME_	14	// 3??RF?????Å≥??MS


#define _EMS_LEVEL1_TIME_	 1	// 1??EMS?????Å≥??MS
#define _EMS_LEVEL2_TIME_	 1   //2	// 2??EMS?????Å≥??MS
#define _EMS_LEVEL3_TIME_	 1   //3	// 3??EMS?????Å≥??MS
#define _OUT_TIME_			 30 //40  //25	// Å±??????Å≥??MS

#if _DEBUG_
#define pr(...) printf(__VA_ARGS__)
#else
#define pr(...)
#endif


typedef signed char s8;
typedef signed short int s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int 	u32;

typedef enum {
	SYS_OFF = 0,
	SYS_ON,
	SYS_WAKE,
	SYS_CHARGE,
	SYS_ERR,
	SYS_TEST,   // ?????????????
	SYS_ADJUST_RELOAD,
} Sys_State_Type;

typedef enum {
	Func_DISABLE = 0,
	Func_ENABLE,
	Func_FINISH,
} Func_Type;

typedef enum {
	Flag_RESET = 0,
	Flag_SET,
} Flag_Type;

typedef enum {
	ERR_NONE = 0,
	ERR_BAT_TEMP,
	ERR_BAT_LEVEL,
	ERR_RF_TEMP,
} Err_Type;

typedef enum {
	BAT_LEVEL_NONE = 0,
	BAT_LEVEL_EMETY,
	BAT_LEVEL_LOW5,
	BAT_LEVEL_LOW10,
	BAT_LEVEL_LOW20,
	BAT_LEVEL_PRE_70,
	BAT_LEVEL_MID,
	BAT_LEVEL_FULL,
} BatLevel_Type;

typedef enum {
	CHARGE_STOP = 0,
	CHARGE_START,
	CHARGE_FINISH,
} Charge_Type;

typedef struct
{
	int dbg_val;
	/* data */
}Debug_data_t;

extern Debug_data_t Debug_data;

#define RF_NTC_PORT			GPIOA	//ADC_IN2
#define RF_NTC_PIN			GPIO_PIN_2

#define BAT_NTC_PORT		GPIOA	//ADC_IN3
#define BAT_NTC_PIN			GPIO_PIN_3

#define BAT_ADC_PORT		GPIOA	//ADC_IN4
#define BAT_ADC_PIN			GPIO_PIN_4

#define POW_ADC_PORT		GPIOA	//ADC_IN5
#define POW_ADC_PIN			GPIO_PIN_5

#define LASER_PORT			GPIOA
#define LASER_PIN			GPIO_PIN_15

#define LED_BAT_LOW_PORT	GPIOB			//T2_CH2
#define LED_BAT_LOW_PIN		GPIO_PIN_0

#define LED_BAT_FULL_PORT	GPIOA			//T2_CH1
#define LED_BAT_FULL_PIN	GPIO_PIN_7

#define LED_1D_PORT			GPIOA
#define LED_1D_PIN			GPIO_PIN_1

#define LED_2D_PORT			GPIOB
#define LED_2D_PIN			GPIO_PIN_1

#define LED_3D_PORT			GPIOA
#define LED_3D_PIN			GPIO_PIN_8

#define KEY_POWER_PORT		GPIOB
#define KEY_POWER_PIN		GPIO_PIN_3

#define MOTOR_PORT			GPIOB
#define MOTOR_PIN			GPIO_PIN_4

#define CHARGE_EN_PORT		GPIOB
#define CHARGE_EN_PIN		GPIO_PIN_5

#define CHARGE_PORT			GPIOA
#define CHARGE_PIN			GPIO_PIN_0

#define PPR_PORT			GPIOF
#define PPR_PIN				GPIO_PIN_1

#define PWM1_PORT			GPIOA	//TIMER0_CH1
#define PWM1_PIN			GPIO_PIN_9

#define PWM_EN_PORT			GPIOA
#define PWM_EN_PIN			GPIO_PIN_6

#define POWER_PORT			GPIOA
#define POWER_PIN			GPIO_PIN_10

#define TX0_PORT			GPIOB
#define TX0_PIN				GPIO_PIN_6

#define RX0_PORT			GPIOB
#define RX0_PIN				GPIO_PIN_7

void Data16_Set(u16 *dest,u16 data,u16 len);

void Data16_Copy(u16 *dest,const u16 *src,u16 len);


#endif


