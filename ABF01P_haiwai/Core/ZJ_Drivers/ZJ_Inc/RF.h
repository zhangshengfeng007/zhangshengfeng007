#ifndef _RF_H
#define _RF_H
#include "includes.h"

#define RF_Tim htim17
#define RF_CHANNEL   TIM_CHANNEL_1

#if (ARF001 == DEVICE_R1_RPO)
	#define Frequency_2M0_CNT 5
	#define Frequency_1M5_CNT 20
	#define Frequency_1M0_CNT 50
	#define Pole_Change_Time  170
	#define Heating_CNT		  20

#elif (ARF001 == DEVICE_R1_RPO_MAX)
	#define Frequency_2M0_CNT 10
	#define Frequency_1M5_CNT 20
	#define Frequency_1M0_CNT 50
	#define Pole_Change_Time  170
	#define Heating_CNT		  30

#elif (ARF001 == DEVICE_R1_HAIWAI)   // 具体波形输出还需调试
	#define Frequency_2M0_CNT 5
	#define Frequency_1M5_CNT 20
	#define Frequency_1M0_CNT 50
	#define Pole_Change_Time  170
	#define Heating_CNT		  20
#endif


#define RF_VALUE_SET(Compare) {__HAL_TIM_SetCompare(&RF_Tim,RF_CHANNEL,Compare);}
///��Ƶ���ʹ��
#define RF_EN_PIN       GPIO_PIN_13
#define RF_EN_PORT			GPIOC

#define RF_EN_UP()      HAL_GPIO_WritePin(RF_EN_PORT,RF_EN_PIN,GPIO_PIN_SET)
#define RF_EN_DOWN()    HAL_GPIO_WritePin(RF_EN_PORT,RF_EN_PIN,GPIO_PIN_RESET)

#define VBAT_DET_PIN    GPIO_PIN_15
#define VBAT_DET_PORT	GPIOC

#define VBAT_DET_ON()   HAL_GPIO_WritePin(VBAT_DET_PORT,VBAT_DET_PIN,GPIO_PIN_SET)
#define VBAT_DET_OFF()  HAL_GPIO_WritePin(VBAT_DET_PORT,VBAT_DET_PIN,GPIO_PIN_RESET)

#define VBAT_OUT_ON_Pin GPIO_PIN_0
#define VBAT_OUT_ON_GPIO_Port GPIOF

#define VBAT_OUT_ON()   HAL_GPIO_WritePin(VBAT_OUT_ON_GPIO_Port,VBAT_OUT_ON_Pin,GPIO_PIN_SET)
#define VBAT_OUT_OFF()  HAL_GPIO_WritePin(VBAT_OUT_ON_GPIO_Port,VBAT_OUT_ON_Pin,GPIO_PIN_RESET)
////��Ƶ ��λ ���  ����
#define GEAR2_PIN       GPIO_PIN_7 ///��λ2
#define GEAR2_PORT		GPIOA
#define GEAR3_PIN       GPIO_PIN_0 ///��λ3
#define GEAR3_PORT		GPIOB
#define GEAR4_PIN       GPIO_PIN_1 ///��λ4
#define GEAR4_PORT		GPIOB



#define GEAR2_UP()    HAL_GPIO_WritePin(GEAR2_PORT,GEAR2_PIN,GPIO_PIN_SET)
#define GEAR2_DOWN()  HAL_GPIO_WritePin(GEAR2_PORT,GEAR2_PIN,GPIO_PIN_RESET)
#define GEAR3_UP()    HAL_GPIO_WritePin(GEAR3_PORT,GEAR3_PIN,GPIO_PIN_SET)
#define GEAR3_DOWN()  HAL_GPIO_WritePin(GEAR3_PORT,GEAR3_PIN,GPIO_PIN_RESET)
#define GEAR4_UP()    HAL_GPIO_WritePin(GEAR4_PORT,GEAR4_PIN,GPIO_PIN_SET)
#define GEAR4_DOWN()  HAL_GPIO_WritePin(GEAR4_PORT,GEAR4_PIN,GPIO_PIN_RESET)



#if (ARF001 == DEVICE_R1_RPO)

	#define	RF_Level_1()	{GEAR2_DOWN();GEAR3_DOWN();GEAR4_DOWN();}
	#define	RF_Level_2()	{GEAR2_UP();GEAR3_DOWN();GEAR4_DOWN();}
	#define	RF_Level_3()	{GEAR2_DOWN();GEAR3_UP();GEAR4_DOWN();}
	#define	RF_Level_4()	{GEAR2_UP();GEAR3_UP();GEAR4_DOWN();}
	#define	RF_Level_5()	{GEAR2_UP();GEAR3_DOWN();GEAR4_UP();}
	#define	RF_Level_6()	{GEAR2_UP();GEAR3_DOWN();GEAR4_UP();}

#elif (ARF001 == DEVICE_R1_RPO_MAX)

	#define	RF_Level_1()	{GEAR2_DOWN();GEAR3_DOWN();GEAR4_DOWN();}
	#define	RF_Level_2()	{GEAR2_UP();GEAR3_DOWN();GEAR4_DOWN();}
	#define	RF_Level_3()	{GEAR2_DOWN();GEAR3_UP();GEAR4_DOWN();}
	#define	RF_Level_4()	{GEAR2_UP();GEAR3_UP();GEAR4_DOWN();}
	#define	RF_Level_5()	{GEAR2_DOWN();GEAR3_UP();GEAR4_UP();}
	#define	RF_Level_6()	{GEAR2_DOWN();GEAR3_UP();GEAR4_UP();}

#elif (ARF001 == DEVICE_R1_HAIWAI)                            // 需要根据原理图，重新修改

	#define	RF_Level_1()	{GEAR2_DOWN();GEAR3_DOWN();GEAR4_DOWN();}
	#define	RF_Level_2()	{GEAR2_UP();GEAR3_DOWN();GEAR4_DOWN();}
	#define	RF_Level_3()	{GEAR2_DOWN();GEAR3_UP();GEAR4_DOWN();}
	#define	RF_Level_4()	{GEAR2_UP();GEAR3_UP();GEAR4_DOWN();}
	#define	RF_Level_5()	{GEAR2_DOWN();GEAR3_UP();GEAR4_UP();}
	#define	RF_Level_6()	{GEAR2_DOWN();GEAR3_UP();GEAR4_UP();}
#endif

void Set_RF_Level(_Sys_Info_TypeDef* Level) ;
void RF_Osc_ON(void);
void RF_Osc_Off(void);
void RF_Stop(void);
void RF_Start(void);
void RF_Process(_RF_HANDLE_TypeDef* RfHandle);
void RF_Frequency_conversion(uint8_t level);
void Frequency_conversion_Process(void);
void Pole_Change(void);
void RF_Init(void);
void RF_DeInit(void);
#endif
