#ifndef __OUTPUT_H
#define __OUTPUT_H

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

void SEAL_MODE_RF_EMS_NOMEL_OUTPUT(uint32_t time);
void SEAL_MODE_RF_EMS_PLUS_OUTPUT(void);
void SLIDE_MODE_RF_EMS_OUPUT(void);
void SLIDE_MODE_RF_NoEMS_OUPUT(void);
extern void RF_EMS_OUTPUT_Handle(void);
extern void RF_EMS_OUTPUT_Function(void);
extern void Seal_Check_Skin_Output(void);

//typedef enum
//{
//	Output_Sile_Mode,
//	Output_Seal_Mode,
//	Output_Check_Mode,	
//}_Output_State_Typedef;

//#define Silde_RF_Freq_Conver   //滑动模式变频开关
#define Seal_RF_Freq_Conver    //盖章模式变频开关

#endif
