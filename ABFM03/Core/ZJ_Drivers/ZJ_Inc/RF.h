#ifndef _RF_H
#define _RF_H
#include "includes.h"

#define RF_Tim htim17
#define RF_CHANNEL   TIM_CHANNEL_1

#if ARF001
//	#define Frequency_2M0_CNT 5
//	#define Frequency_1M5_CNT 20
//	#define Frequency_1M0_CNT 50
	#define Frequency_2M0_CNT 50
	#define Frequency_1M5_CNT 200
	#define Frequency_1M0_CNT 500

	#define Pole_Change_Time  170
	#define Heating_CNT		  20


#else
	#define Frequency_2M0_CNT 10
	#define Frequency_1M5_CNT 20
	#define Frequency_1M0_CNT 50
	#define Pole_Change_Time  170
	#define Heating_CNT		  30
#endif


#define RF_VALUE_SET(Compare) {__HAL_TIM_SetCompare(&RF_Tim,RF_CHANNEL,Compare);}
///��Ƶ���ʹ��
#define RF_EN_PIN       VRF_EN_Pin
#define RF_EN_PORT		VRF_EN_PORT

#define RF_EN_UP()      HAL_GPIO_WritePin(RF_EN_PORT,RF_EN_PIN,GPIO_PIN_SET)
#define RF_EN_DOWN()    HAL_GPIO_WritePin(RF_EN_PORT,RF_EN_PIN,GPIO_PIN_RESET)

#define VBAT_OUT_EN_PIN     GPIO_PIN_0
#define VBAT_OUT_EN_PORT 	GPIOF

#define VBAT_OUT_ON()   HAL_GPIO_WritePin(VBAT_OUT_EN_PORT, VBAT_OUT_EN_PIN, GPIO_PIN_SET)
#define VBAT_OUT_OFF()  HAL_GPIO_WritePin(VBAT_OUT_EN_PORT, VBAT_OUT_EN_PIN, GPIO_PIN_RESET)

////��Ƶ ��λ ���  ����
#define GEAR2_PIN       RF_LEVEL2_Pin ///��λ2
#define GEAR2_PORT		RF_LEVEL2_PORT
#define GEAR3_PIN       RF_LEVEL3_Pin ///��λ3
#define GEAR3_PORT		RF_LEVEL3_PORT
#define GEAR4_PIN       RF_LEVEL4_Pin ///��λ4
#define GEAR4_PORT		RF_LEVEL4_PORT


#define GEAR2_UP()    HAL_GPIO_WritePin(GEAR2_PORT,GEAR2_PIN,GPIO_PIN_SET)
#define GEAR2_DOWN()  HAL_GPIO_WritePin(GEAR2_PORT,GEAR2_PIN,GPIO_PIN_RESET)
#define GEAR3_UP()    HAL_GPIO_WritePin(GEAR3_PORT,GEAR3_PIN,GPIO_PIN_SET)
#define GEAR3_DOWN()  HAL_GPIO_WritePin(GEAR3_PORT,GEAR3_PIN,GPIO_PIN_RESET)
#define GEAR4_UP()    HAL_GPIO_WritePin(GEAR4_PORT,GEAR4_PIN,GPIO_PIN_SET)
#define GEAR4_DOWN()  HAL_GPIO_WritePin(GEAR4_PORT,GEAR4_PIN,GPIO_PIN_RESET)

#define	Level_5V0()	{GEAR2_DOWN();GEAR3_DOWN();GEAR4_DOWN();}    // --
#define	Level_5V5()	{GEAR2_UP();GEAR3_DOWN();GEAR4_DOWN();}

#define	Level_6V0()	{GEAR2_DOWN();GEAR3_UP();GEAR4_DOWN();}
#define	Level_6V5()	{GEAR2_UP();GEAR3_UP();GEAR4_DOWN();}
#define	Level_7V0()	{GEAR2_UP();GEAR3_DOWN();GEAR4_UP();}
#define	Level_7V5()	{GEAR2_DOWN();GEAR3_UP();GEAR4_UP();}
#if 0

	#define	RF_Level_1()	{GEAR2_DOWN();GEAR3_DOWN();GEAR4_DOWN();}
	#define	RF_Level_2()	{GEAR2_UP();GEAR3_DOWN();GEAR4_DOWN();}
	#define	RF_Level_3()	{GEAR2_DOWN();GEAR3_UP();GEAR4_DOWN();}
	#define	RF_Level_4()	{GEAR2_UP();GEAR3_UP();GEAR4_DOWN();}
	#define	RF_Level_5()	{GEAR2_UP();GEAR3_DOWN();GEAR4_UP();}
	#define	RF_Level_6()	{GEAR2_UP();GEAR3_DOWN();GEAR4_UP();}


	#define	EMS_RF_Level_1()	{GEAR2_DOWN();GEAR3_DOWN();GEAR4_DOWN();}
	#define	EMS_RF_Level_2()	{GEAR2_UP();GEAR3_DOWN();GEAR4_DOWN();}
	#define	EMS_RF_Level_3()	{GEAR2_DOWN();GEAR3_UP();GEAR4_DOWN();}
	#define	EMS_RF_Level_4()	{GEAR2_UP();GEAR3_UP();GEAR4_DOWN();}
	#define	EMS_RF_Level_5()	{GEAR2_UP();GEAR3_DOWN();GEAR4_UP();}

#else //���Դ���֤

	#define	RF_Level_1()	{GEAR2_UP();GEAR3_DOWN();GEAR4_DOWN();} //  ԭ��ͼ�� : ����1���� 4.7v   --
	#define	RF_Level_3()	{GEAR2_DOWN();GEAR3_DOWN();GEAR4_UP();}//    //   �����޸���2����3�� 5.5v    ----����ģʽ 1��2��3 ������5.5v
	#define	RF_Level_5()    Level_6V0()
	#define	RF_Level_6()	Level_6V0()


//	#define	EMS_RF_Level_1()	{GEAR2_DOWN();GEAR3_DOWN();GEAR4_DOWN();}
//	#define	EMS_RF_Level_2()	{GEAR2_UP();GEAR3_DOWN();GEAR4_DOWN();}
//	#define	EMS_RF_Level_3()	{GEAR2_DOWN();GEAR3_UP();GEAR4_DOWN();}
//	#define	EMS_RF_Level_4()	{GEAR2_UP();GEAR3_UP();GEAR4_DOWN();}
	#define	EMS_RF_Level_5()    {GEAR2_DOWN();GEAR3_DOWN();GEAR4_UP();}//{GEAR2_DOWN();GEAR3_UP();GEAR4_DOWN();}
#endif

typedef enum
{
	RF_2MHZ_FREQ = 0,
	RF_1_5MHZ_FREQ,
	RF_1MHZ_FREQ,
	RF_ALL_FREQ,
}RF_FREQ_e;

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
