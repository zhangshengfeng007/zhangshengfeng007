#include "includes.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim14;

_EMS_Handler_Typedef  EMS_Handler;

void EMS_Value_PID_Handler(void);
/*************************************************************************************
* FunctionName   : EMS_Pwm_Start()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Set_Ems_Tim_Psc(uint8_t freq)
{
  TIM1->PSC = freq;
}
/*************************************************************************************
* FunctionName   : Get_EMS_Pow_Value()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Get_EMS_Pow_Value(void)
{
  EMS_Handler.EMS_True_Value = (uint16_t)(ADC_Data.EMS_U_Val*8.592);
}
/**************************************************************************************
函数名称 ： RF_PID_Init
功    能 ： PID KP,KI,KD    设置参数值
参    数 ： RF_Handler.RF_PID.Kp 比例系数
RF_Handler.RF_PID.Ki 积分系数
RF_Handler.RF_PID.Kd 微分系数
返 回 值 ： -
**************************************************************************************/
#define EMS_Freq 49
#define duty 10
#define interval 10
#define channel_p_start 0
#define channel_p_stop  channel_p_start+duty
#define channel_n_start channel_p_stop+interval
#define channel_n_stop  channel_n_start+duty
void EMS_PID_Init(void)
{
  EMS_Handler.EMS_PID.Kp = 0.0025;
  EMS_Handler.EMS_PID.Ki = 0.0015;
  EMS_Handler.EMS_PID.Kd = 0.0012;
  
  SET_BIT(TIM1->CR1,TIM_CR1_UDIS);
  __HAL_TIM_SET_AUTORELOAD(&htim1,EMS_Freq);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,channel_n_start);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,channel_n_stop);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,channel_p_start);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,channel_p_stop);
  CLEAR_BIT(TIM1->CR1,TIM_CR1_UDIS);
  
  EMS_Handler.EMS_PID.target = EMS_Seal_One;
}

/**************************************************************************************
函数名称 ： EMS_Set_Value
功    能 ： EMS档位电压设置，放入EMS_PID服务函数
参    数 ： 
返 回 值 ： -
**************************************************************************************/
void EMS_Set_Value(void)
{ 
  if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)
  {
    switch(Sys_Info.Key_State.Seal_gear)
    {
    case GEARONE  :EMS_Handler.EMS_PID.target = EMS_Seal_One;break;
    case GEARTWO  :EMS_Handler.EMS_PID.target = EMS_Seal_Two;break;
    case GEARTHREE:EMS_Handler.EMS_PID.target = EMS_Seal_Three;break;
    default:break;
    }      
  }
  else if(Sys_Info.Key_State.Mode_choice_key == SLIDE_MODE)
  {
    switch(Sys_Info.Key_State.Slide_gear)
    {
    case GEARONE  :EMS_Handler.EMS_PID.target = EMS_Slide_One;break;
    case GEARTWO  :EMS_Handler.EMS_PID.target = EMS_Slide_Two;break;
    case GEARTHREE:EMS_Handler.EMS_PID.target = EMS_Slide_Three;break;
    default:break;
    }    
  }
}
/**************************************************************************************
* FunctionName   : EMS_Value_PID_Handler
* Description    : 
* EntryParameter : 
* ReturnValue    :
**************************************************************************************/ 
void EMS_Value_PID_Handler(void)
{
  Get_EMS_Pow_Value();
  EMS_Set_Value();
  
  if(Sys_Info.PowerOn_Time_Counter > EMS_Start_DelayTime)
  {  
    EMS_Handler.EMS_PID.Inc = PID_Inc(EMS_Handler.EMS_PID.target,EMS_Handler.EMS_True_Value,&EMS_Handler.EMS_PID);
    EMS_Handler.EMS_PID.data += EMS_Handler.EMS_PID.Inc;
    
    if(EMS_Handler.EMS_PID.data > 90)
    {
      EMS_Handler.EMS_PID.data = 90;
    }
    else if(EMS_Handler.EMS_PID.data < 50)
    {
      EMS_Handler.EMS_PID.data = 50;
    }
    //EMS_Handler.EMS_PID.data = 65;
   }
   else if(Sys_Info.PowerOn_Time_Counter > 2)  //上电缓启动
   {
      if(EMS_Handler.EMS_PID.data < 80)
      EMS_Handler.EMS_PID.data+=1;
   }
   else 
   {
      EMS_Handler.EMS_PID.data = 10;
   }
   
  SET_BIT(TIM14->CR1,TIM_CR1_UDIS);
  __HAL_TIM_SET_AUTORELOAD(&htim14,100);	
  __HAL_TIM_SET_COMPARE(&htim14,TIM_CHANNEL_1,EMS_Handler.EMS_PID.data);
  CLEAR_BIT(TIM14->CR1,TIM_CR1_UDIS);
}

/*************************************************************************************
* FunctionName   : EMS_OUT_PWM_Start，EMS_OUT_PWM_Stop，EMS_SET_VALUE_PWM_Start，EMS_SET_VALUE_PWM_Stop
* Description    : EMS输出的相关PWM操作，调压PWM+波形驱动PWM控制
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void EMS_OUT_PWM_Start(void)
{
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
}
void EMS_OUT_PWM_Stop(void)
{
  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
}
void EMS_SET_VALUE_PWM_Start(void)
{
  HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);    //启动EMS调压
}
void EMS_SET_VALUE_PWM_Stop(void)
{
  HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);      //关闭EMS调压
}
/**************************************************************************************
* FunctionName   : EMS_OUT_Start
* Description    : 
* EntryParameter : 
* ReturnValue    :
**************************************************************************************/ 
void EMS_OUT_Start(void)
{
  RF_OUTPUT_DISABLE;
  EMS_OUTPUT_ENABLE;
}
/**************************************************************************************
* FunctionName   : EMS_OUT_Stop
* Description    : 
* EntryParameter : 
* ReturnValue    :
**************************************************************************************/ 
void EMS_OUT_Stop(void)
{
  EMS_OUTPUT_DISABLE;
  RF_OUTPUT_DISABLE;
}

