#ifndef __RF_HANDLE_H
#define __RF_HANDLE_H

typedef enum
{
//  Seal_Target_One   = 6500,  
//  Seal_Target_Two   = 7000,
//  Seal_Target_Three = 7500,
//  Slide_Target_One  = 7000,
//  Slide_Target_Two  = 7500,
//  Slide_Target_Three= 8000,
  
  Seal_Target_One   = 387,
  Seal_Target_Two   = 375,
  Seal_Target_Three = 368,
  Slide_Target_One  = 364,
  Slide_Target_Two  = 353,
  Slide_Target_Three= 340,
}_RF_Value_Target_Typedef;


typedef struct
{
  float Kp;                       //����ϵ��Proportional
  float Ki;                       //����ϵ��Integral
  float Kd;                       //΢��ϵ��Derivative
  
  float Ek;                       //��ǰ���
  float Ek1;                      //ǰһ����� e(k-1)
  float Ek2;                      //��ǰһ����� e(k-2)
  
  float Inc;
  int32_t data;
  uint32_t target;
}PID_IncTypeDef;

typedef struct
{
  PID_IncTypeDef  RF_PID;
  uint16_t RF_True_Value;
  uint8_t RF_Pwm_flage  :1;
  uint8_t g_RfOpenFlage :1;         //�Ƿ����Ƶ�����־λ  1 ��Ƶ����Ѿ���   0  ��Ƶ����ر�
  uint8_t RF_Load_Flag  :1;
  uint8_t RF_Load_Out_Scope :1;
  
}_RF_Handler_Typedef;
#define RF_Start_DelayTime 8   //��ʱ 80ms����RF��ѹ����

extern _RF_Handler_Typedef   RF_Handler;

extern void RF_OUT_PWM_Start(void);
extern void RF_OUT_PWM_Stop(void);
extern void RF_SET_VALUE_PWM_Start(void);
extern void RF_SET_VALUE_PWM_Stop(void);
extern void Set_RF_Tim_Psc(uint8_t freq);
extern void RF_PID_Init(void);
extern void RF_Value_PID_Handler(void);
extern void RF_OUT_Start(void);
extern void RF_OUT_Stop(void);
extern float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID);

#endif

