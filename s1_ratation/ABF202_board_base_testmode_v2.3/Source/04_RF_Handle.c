#include "includes.h"

_RF_Handler_Typedef   RF_Handler;

extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;

/*************************************************************************************
* FunctionName   : EMS_Pwm_Start()
* Description    : 操作此处实现RF输出变频动作，仅调节TIM17的分频，32Mhz主频
* EntryParameter : 2Mhz：  freq=7   
                   1.6Mhz：freq=10    
                   1Mhz：  freq=15
* ReturnValue    : None
**************************************************************************************/
void Set_RF_Tim_Psc(uint8_t freq)
{
  TIM17->PSC = freq;
}
/**************************************************************************************
函数名称 ： RF_PID_Init
功    能 ： PID KP,KI,KD    设置参数值
参    数 ： RF_Handler.RF_PID.Kp 比例系数
            RF_Handler.RF_PID.Ki 积分系数
            RF_Handler.RF_PID.Kd 微分系数
返 回 值 ： None
**************************************************************************************/
void RF_PID_Init(void)
{
  RF_Handler.RF_PID.Kp = 0.012;
  RF_Handler.RF_PID.Ki = 0.005;
  RF_Handler.RF_PID.Kd = 0.002;
  
  RF_Handler.RF_PID.target = Seal_Target_One;
}
/**************************************************************************************
函数名称 ： PID_Inc
功    能 ： PID增量(Increment)计算
参    数 ： SetValue ------ 设置值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID数据结构
返 回 值 ： PIDInc -------- 本次PID增量(+/-)
**************************************************************************************/
float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)
{
  float PIDInc;                                  //增量
  
  PID->Ek = SetValue - ActualValue;
  PIDInc = (PID->Kp * PID->Ek) - (PID->Ki * PID->Ek1) + (PID->Kd * PID->Ek2);
  
  PID->Ek2 = PID->Ek1;
  PID->Ek1 = PID->Ek;  
  return PIDInc;
}
/**************************************************************************************
函数名称 ： RF_Set_Value
功    能 ： RF档位电压设置，放入RF_PID服务函数
参    数 ： 
返 回 值 ： -
**************************************************************************************/
void RF_Set_Value(void)
{ 
  if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)
  {
    if(Sys_Info.Temp_Protect == OverHEAT)   //过温时
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
    if(Sys_Info.Temp_Protect == OverHEAT)   //过温时
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
函数名称 ： RF_Value_PID_Handler
功    能 ： RF_PID服务函数，放入主循环
参    数 ： RF_Handler.RF_True_Value 实际电压值，单位mV
            RF_Handler.RF_PID.Inc    增量值
            RF_Handler.RF_PID.data   PWM设置值
返 回 值 ： -
**************************************************************************************/
void RF_Value_PID_Handler(void)
{
  RF_Handler.RF_True_Value = (uint16_t)(ADC_Data.RF_U_Val*2.59);			//2500/4096*668/158
  
  RF_Set_Value();

  if(Sys_Info.PowerOn_Time_Counter > RF_Start_DelayTime)  //PID延时工作，等待此模块上电完成
  {
//    RF_Handler.RF_PID.Inc = PID_Inc(RF_Handler.RF_PID.target,RF_Handler.RF_True_Value,&RF_Handler.RF_PID);
//    RF_Handler.RF_PID.data -= RF_Handler.RF_PID.Inc;
//------------------// 电流限制----------------
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
* FunctionName   : RF_OUT_PWM_Start，RF_OUT_PWM_Stop，RF_SET_VALUE_PWM_Start，RF_SET_VALUE_PWM_Stop
* Description    : RF输出的相关PWM操作，调压PWM+波形驱动PWM控制
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void RF_OUT_PWM_Start(void)
{
  HAL_TIMEx_PWMN_Start(&htim17,TIM_CHANNEL_1);  //RF 启动定时器互补通道PWM输出 */	
  HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);    //RF 启动通道PWM输出 */
}
void RF_OUT_PWM_Stop(void)
{
  HAL_TIMEx_PWMN_Stop(&htim17,TIM_CHANNEL_1);   //RF 关闭定时器互补通道PWM输出 */	
  HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);     //RF 关闭启动通道PWM输出 */
}
void RF_SET_VALUE_PWM_Start(void)
{
  HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);     //启动RF调压PWM
}
void RF_SET_VALUE_PWM_Stop(void)
{
  HAL_TIM_PWM_Stop(&htim16,TIM_CHANNEL_1);      //关闭RF调压PWM
}
/*************************************************************************************
* FunctionName   : RF_OUT_Start()
* Description    : 输出固态继电器操作，RF输出开关
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
* Description    : 输出固态继电器操作，RF输出开关
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




