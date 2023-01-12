#include "includes.h"

_RF_Handler_Typedef   RF_Handler;

extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;

/*************************************************************************************
* FunctionName   : EMS_Pwm_Start()
* Description    : �����˴�ʵ��RF�����Ƶ������������TIM17�ķ�Ƶ��32Mhz��Ƶ
* EntryParameter : 2Mhz��  freq=7   
                   1.6Mhz��freq=10    
                   1Mhz��  freq=15
* ReturnValue    : None
**************************************************************************************/
void Set_RF_Tim_Psc(uint8_t freq)
{
  TIM17->PSC = freq;
}
/**************************************************************************************
�������� �� RF_PID_Init
��    �� �� PID KP,KI,KD    ���ò���ֵ
��    �� �� RF_Handler.RF_PID.Kp ����ϵ��
            RF_Handler.RF_PID.Ki ����ϵ��
            RF_Handler.RF_PID.Kd ΢��ϵ��
�� �� ֵ �� None
**************************************************************************************/
void RF_PID_Init(void)
{
  RF_Handler.RF_PID.Kp = 0.012;
  RF_Handler.RF_PID.Ki = 0.005;
  RF_Handler.RF_PID.Kd = 0.002;
  
  RF_Handler.RF_PID.target = Seal_Target_One;
}
/**************************************************************************************
�������� �� PID_Inc
��    �� �� PID����(Increment)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID���ݽṹ
�� �� ֵ �� PIDInc -------- ����PID����(+/-)
**************************************************************************************/
float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)
{
  float PIDInc;                                  //����
  
  PID->Ek = SetValue - ActualValue;
  PIDInc = (PID->Kp * PID->Ek) - (PID->Ki * PID->Ek1) + (PID->Kd * PID->Ek2);
  
  PID->Ek2 = PID->Ek1;
  PID->Ek1 = PID->Ek;  
  return PIDInc;
}
/**************************************************************************************
�������� �� RF_Set_Value
��    �� �� RF��λ��ѹ���ã�����RF_PID������
��    �� �� 
�� �� ֵ �� -
**************************************************************************************/
void RF_Set_Value(void)
{ 
  if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)
  {
    if(Sys_Info.Temp_Protect == OverHEAT)   //����ʱ
    {
      switch(Sys_Info.Key_State.Seal_gear)
      {
      case GEARONE  :RF_Handler.RF_PID.target = Seal_Target_One;break;
      case GEARTWO  :RF_Handler.RF_PID.target = Seal_Target_One;break;
      case GEARTHREE:RF_Handler.RF_PID.target = Seal_Target_Two;break;
      default:break;
      }
    }
    else
    {
      switch(Sys_Info.Key_State.Seal_gear)
      {
      case GEARONE  :RF_Handler.RF_PID.target = Seal_Target_One;break;
      case GEARTWO  :RF_Handler.RF_PID.target = Seal_Target_Two;break;
      case GEARTHREE:RF_Handler.RF_PID.target = Seal_Target_Three;break;
      default:break;
      }
    }
  }
  else if(Sys_Info.Key_State.Mode_choice_key == SLIDE_MODE)
  {
    if(Sys_Info.Temp_Protect == OverHEAT)   //����ʱ
    {
      switch(Sys_Info.Key_State.Slide_gear)
      {
      case GEARONE  :RF_Handler.RF_PID.target = Slide_Target_One;break;
      case GEARTWO  :RF_Handler.RF_PID.target = Slide_Target_One;break;
      case GEARTHREE:RF_Handler.RF_PID.target = Slide_Target_One;break;
      default:break;
      }
    }
    else
    {
      switch(Sys_Info.Key_State.Slide_gear)
      {
      case GEARONE  :RF_Handler.RF_PID.target = Slide_Target_One;break;
      case GEARTWO  :RF_Handler.RF_PID.target = Slide_Target_Two;break;
      case GEARTHREE:RF_Handler.RF_PID.target = Slide_Target_Three;break;
      default:break;
      }
    }
  }
}
/**************************************************************************************
�������� �� RF_Value_PID_Handler
��    �� �� RF_PID��������������ѭ��
��    �� �� RF_Handler.RF_True_Value ʵ�ʵ�ѹֵ����λmV
            RF_Handler.RF_PID.Inc    ����ֵ
            RF_Handler.RF_PID.data   PWM����ֵ
�� �� ֵ �� -
**************************************************************************************/
void RF_Value_PID_Handler(void)
{
  RF_Handler.RF_True_Value = (uint16_t)(ADC_Data.RF_U_Val*2.59);			//2500/4096*668/158
  
  RF_Set_Value();

  if(Sys_Info.PowerOn_Time_Counter > RF_Start_DelayTime)  //PID��ʱ�������ȴ���ģ���ϵ����
  {
//    RF_Handler.RF_PID.Inc = PID_Inc(RF_Handler.RF_PID.target,RF_Handler.RF_True_Value,&RF_Handler.RF_PID);
//    RF_Handler.RF_PID.data -= RF_Handler.RF_PID.Inc;
//------------------// ��������----------------
//    Total_Power = RF_Handler.RF_True_Value * RF_Handler.CurrentDynam;
//-------------------------------------
    RF_Handler.RF_PID.data = RF_Handler.RF_PID.target;

//    if(Total_Power > 8000000) 
//    {
//      RF_Handler.RF_PID.data += (Total_Power-8000000)/1000000;
//    }
    if(RF_Handler.RF_PID.data > 600)
    {
      RF_Handler.RF_PID.data = 600;
    }
    if( RF_Handler.RF_PID.data < 280)
    {
      RF_Handler.RF_PID.data = 280;
    }
  }
  else
  {
    RF_Handler.RF_PID.data = 600;
  }
  
  SET_BIT(TIM16->CR1,TIM_CR1_UDIS);
  __HAL_TIM_SET_AUTORELOAD(&htim16,1000);	
  __HAL_TIM_SET_COMPARE(&htim16,TIM_CHANNEL_1,RF_Handler.RF_PID.data);
  CLEAR_BIT(TIM16->CR1,TIM_CR1_UDIS);
}

/*************************************************************************************
* FunctionName   : RF_OUT_PWM_Start��RF_OUT_PWM_Stop��RF_SET_VALUE_PWM_Start��RF_SET_VALUE_PWM_Stop
* Description    : RF��������PWM��������ѹPWM+��������PWM����
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void RF_OUT_PWM_Start(void)
{
  HAL_TIMEx_PWMN_Start(&htim17,TIM_CHANNEL_1);  //RF ������ʱ������ͨ��PWM��� */	
  HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);    //RF ����ͨ��PWM��� */
}
void RF_OUT_PWM_Stop(void)
{
  HAL_TIMEx_PWMN_Stop(&htim17,TIM_CHANNEL_1);   //RF �رն�ʱ������ͨ��PWM��� */	
  HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);     //RF �ر�����ͨ��PWM��� */
}
void RF_SET_VALUE_PWM_Start(void)
{
  HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);     //����RF��ѹPWM
}
void RF_SET_VALUE_PWM_Stop(void)
{
  HAL_TIM_PWM_Stop(&htim16,TIM_CHANNEL_1);      //�ر�RF��ѹPWM
}
/*************************************************************************************
* FunctionName   : RF_OUT_Start()
* Description    : �����̬�̵���������RF�������
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void RF_OUT_Start(void)
{
  RF_Handler.g_RfOpenFlage = 1;
  EMS_OUTPUT_DISABLE;
  RF_OUTPUT_ENABLE;
}
/*************************************************************************************
* FunctionName   : RF_OUT_Stop()
* Description    : �����̬�̵���������RF�������
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void RF_OUT_Stop(void)
{
  RF_Handler.g_RfOpenFlage = 0;
  EMS_OUTPUT_DISABLE;
  RF_OUTPUT_DISABLE;
}
//---------------------------------------------------------------------------------------//




