#ifndef __EMS_H
#define __EMS_H
#include "includes.h"

#define EMS_BOOST_ADC_AN	0
#define EMS_VOL_ERROR			20
#define EMS_BOOST_DEADLINE		38
#define EMS_BOOST_SW_PERIOD		20		//20KHz  400
#define EMS_Mode 0
#define EMS_1000_ms 100
#define EMS_1500_ms 150
#define EMS_2000_ms 200
#define PSC_Constant 4
#define Frequency_PSC 0
//******100 -100 *****************//
#define MaxValue   50
#define MinValue  (-50)

#define TEST_MODE  0


#if (ARF001 == DEVICE_R1_RPO)

#if TEST_MODE
/*********************����***********************/
	#define PidConstant       114
	#define	EMS_VOL_LEVEL1			(1228 + PidConstant)//1228 21.5V
	#define	EMS_VOL_LEVEL2			(1326 + PidConstant)//1351
	#define	EMS_VOL_LEVEL3			(1444 + PidConstant)//1474
	#define	EMS_VOL_LEVEL4			(1560 + PidConstant)//1597
	#define	EMS_VOL_LEVEL5			(1700  + PidConstant)//1720

#else
	#define EMS_VOL_Constant  85 // 85 1.5 V  114 2 V
	#define PidConstant       -171//30
	#define	EMS_VOL_LEVEL1			(1228 + PidConstant)//21.569 V
	#define	EMS_VOL_LEVEL2			(1392 + PidConstant)//24.540 V
	#define	EMS_VOL_LEVEL3			(1556 + PidConstant)//27.331 V
	#define	EMS_VOL_LEVEL4			(1720 + PidConstant)//30.211 V
	#define	EMS_VOL_LEVEL5			(1884 + PidConstant)//33.092 V
//	#define	EMS_VOL_LEVEL5			(1884 -224)//33.092 V
#endif

/******************************************/
#elif (ARF001 == DEVICE_R1_RPO_MAX)
	#define PidConstant       		 10
	#define	EMS_VOL_LEVEL1			(1220 + PidConstant)//(1228 + PidConstant)(VPP = 35V)
	#define	EMS_VOL_LEVEL2			(1320 + PidConstant)//(1326 + PidConstant)//(VPP = 39V)
	#define	EMS_VOL_LEVEL3			(1420 + PidConstant)//(1444 + PidConstant)//(VPP = 43V)
	#define	EMS_VOL_LEVEL4			(1520 + PidConstant) //(1560 + PidConstant)//(VPP = 46V)
	#define	EMS_VOL_LEVEL5			(1620  + PidConstant)//1720  ----1620 --(VPP = 49V)

#elif (ARF001 == DEVICE_R1_HAIWAI)
	#define EMS_VOL_Constant  		85 // 85 1.5 V  114 2 V    -------具体参数还要调试
	#define PidConstant       		-171//30
	#define	EMS_VOL_LEVEL1			(1228 + PidConstant)//21.569 V
	#define	EMS_VOL_LEVEL2			(1392 + PidConstant)//24.540 V
	#define	EMS_VOL_LEVEL3			(1556 + PidConstant)//27.331 V
	#define	EMS_VOL_LEVEL4			(1720 + PidConstant)//30.211 V
	#define	EMS_VOL_LEVEL5			(1884 + PidConstant)//33.092 V

#endif

#define EMS_TIM   htim16
#define EMS_CHANNEL   TIM_CHANNEL_1
#define EMS_VALUE_SET(Compare) {__HAL_TIM_SetCompare(&EMS_TIM,EMS_CHANNEL,Compare);}


void Ems_Osc_Off(void);
void Ems_Osc_On(void);
void Ems_Boost_Off(void);
void Ems_Boost_On(void );
uint16_t acquire_adc_value(uint8_t channel, uint16_t* data);
void Set_Ems_level(_Sys_Info_TypeDef* EmsLevel);
void Ems_Osc_Process(void);
void Ems_Boost_Process(void);
void EMS_Open_Process(void);
void Touch_Skin_EMS_Freq_Conversion(void);
void EMS_Pwm_Stop(void);
void EMS_Pwm_Start(void);
void Ems_DeInit(void);
void Ems_Init(void);
void EMS_Frequency_conversion(uint8_t PSC);
#endif


