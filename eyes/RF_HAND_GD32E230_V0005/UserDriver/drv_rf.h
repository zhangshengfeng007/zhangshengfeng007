#ifndef __DRV_RF_H
#define __DRV_RF_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


/* Exported macro ------------------------------------------------------------*/
typedef enum{
	FREQ_1M,
	FREQ_125M,
	FREQ_15M,
}RF_FREQ_Type;

typedef struct{
	Func_Type State;	//??
	u8 Load;				//?????????
	u16 UnLoad_ADC_Val;				//?????????
	u16 POWER_DETECT_ADC_Val;				//?????????
	u8 Load_First;		//?????¦Í??
	u8 UnLoad_Count;	//?????????
	s16 Sc_Temp;		//????????,1¦Ë§³????
	s16 Temp;			//??????,1¦Ë§³????
	s16 Ntc_Offset;		//NTC????
	RF_FREQ_Type Freq;	//??????
	u8 duty_time;
}RF_Type;

extern RF_Type RF;

/* Exported functions ------------------------------------------------------- */
void RF_Init(void);
void RF_Control(Func_Type cmd);
void RF_Freq_Set(RF_FREQ_Type Freq);
void RF_PWM_OFF(void);
void RF_PWM_Config(RF_FREQ_Type Freq);
void RF_OutFreq_Cycle(void);
void RF_Temp_Read(void);
void RF_Load_Detection(void);



#endif

