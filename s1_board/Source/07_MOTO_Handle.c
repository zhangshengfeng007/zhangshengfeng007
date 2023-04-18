#include "includes.h"

_motor_data_t  motor_data;
/*************************************************************************************
* FunctionName	: MOTO_Handle
* Description   :
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void MOTO_Handle(void)
{
  if(motor_data.Electric_Machinery_Onoff == 1)
  {
    MOTO_ENABLE;
  }
  else
  {
    MOTO_DISABLE;
  }
}
//================================================================================================
// Descriptions:      buzzer work 0.5S
// Input parameters:
// Output parameters:
// Returned value:
// Created by:        zhangx
// Created date:      2021.05.27
//------------------------------------------------------------------------------------------------
// Modified by:
// Modified date:   2023.01.12 ,
// Descriptions:  蜂鸣器控制函数，模拟输出pwm 波形，在这里不再控制马达
//=================================================================================================
void buzzer0_5(uint8_t *control,uint8_t *last)
{
  static uint32_t buzzertime;

  if(*control == 1)
  {
    if(*last == 0)
    {
      *last = 1;
      buzzertime = HAL_GetTick();
      // motor_data.Electric_Machinery_Onoff = 1;
    }
  }

  if((HAL_GetTick() - buzzertime) > 400) ///0.5s��رշ�����
  {
    if(*control != 0)
    {
      // motor_data.Electric_Machinery_Onoff = 0;
    }
    *last = 0;
    *control = 0;
  }
}
//================================================================================================
// Descriptions:         buzzer1_0  work 1.0s
// Input parameters:
// Output parameters:
// Returned value:
// Created by:        zhangx
// Created date:      2021.05.27
//------------------------------------------------------------------------------------------------
// Modified by:
// Modified date:     2023.01.12
// Descriptions:    蜂鸣器控制函数，模拟输出pwm 波形，在这里不再控制马达
//=================================================================================================
void buzzer1_0(uint8_t *control,uint8_t *last)
{
  static uint32_t buzzertime;

  if(*control == 1)
  {
    if(*last == 0)
    {
      *last = 1;
      buzzertime = HAL_GetTick();
      // motor_data.Electric_Machinery_Onoff = 1;
    }
  }

  if((HAL_GetTick() - buzzertime) > 200) ///1.0s��رշ�����
  {
    if(*control != 0)
    {
      // motor_data.Electric_Machinery_Onoff = 0;
    }
    *last = 0;
    *control = 0;
  }
}
/*************************************************************************************
* FunctionName	: MOTO_Handle
* Description   :
* EntryParameter:
* ReturnValue   : 10ms 调用一次
**************************************************************************************/
uint8_t buzzer_onoff_flage;
void buzzerChirp(uint8_t slide_mode,uint8_t mode, uint8_t Seal_gear,uint8_t Slide_gear,uint8_t onoff,uint8_t load,uint8_t color)
{
  static uint8_t lastslidemode;
  static uint8_t lastmode;
  static uint8_t lastSealgear;
  static uint8_t lastSlidegear;
  static uint8_t lastonoff;
  static uint8_t lastload;
  static uint8_t lastcolormode;

  static uint8_t buzzer_slidemode_flage;
  static uint8_t buzzer_mode_flage;
  static uint8_t buzzer_Seal_gear_flage;
  static uint8_t buzzer_Slide_gear_flage;
  static uint8_t buzzer_onoff_flage;
  static uint8_t buzzer_load_flage;
  static uint8_t buzzer_color_flage;

  static uint8_t last_slidemode_flage;
  static uint8_t last_mode_flage;
  static uint8_t last_Seal_gear_flage;
  static uint8_t last_Slide_gear_flage;
  static uint8_t last_onoff_flage;
  static uint8_t last_load_flage;
  static uint8_t last_color_flage;


  if(MACHINEON == onoff)//如果在开机状态下
  {

    if(lastslidemode != slide_mode)          //切换输出模式 振动
    {
      buzzer_slidemode_flage = 1;
      last_slidemode_flage = 0;
    }
    if(lastmode != mode)									 	  //滑动模式切换EMS输出，振动
    {
      if((lastmode == 0)||(lastmode == 1))
      {
        buzzer_mode_flage = 1;
        last_mode_flage = 0;
      }
    }

    if(lastSealgear != Seal_gear)					//如果档位有变化 振动
    {
      buzzer_Seal_gear_flage = 1;
      last_Seal_gear_flage = 0;
    }
    if(lastSlidegear!= Slide_gear)
    {
      buzzer_Slide_gear_flage = 1;
      last_Slide_gear_flage = 0;
    }
    if(lastload == 0)
    {
      if(load == 1)                       // 如果接入负载 振动
      {
        // if(Key_State.Mode_choice_key == SEAL_MODE)///����ģʽ
        // {
        //   buzzer_load_flage = 1;
        //   last_load_flage = 0;
        // }
      }
    }
    if(lastcolormode != color)					//识别到面膜颜色，振动
    {
      buzzer_color_flage = 1;
      last_color_flage = 0;
    }
  }
  else
  {
    if(lastonoff != onoff)							//如果开关机有变化
    {
      if(onoff != CHECK_BATTER)
      {
        buzzer_onoff_flage = 1;
      }
    }
  }

  // buzzer0_5(&buzzer_onoff_flage,&last_onoff_flage);
  // buzzer1_0(&buzzer_slidemode_flage, &last_slidemode_flage );
  // buzzer1_0(&buzzer_mode_flage, &last_mode_flage );
  // buzzer1_0(&buzzer_Seal_gear_flage, &last_Seal_gear_flage );
  // buzzer1_0(&buzzer_Slide_gear_flage,&last_Slide_gear_flage);
  // buzzer1_0(&buzzer_load_flage, &last_load_flage );
  // buzzer1_0(&buzzer_color_flage,&last_color_flage);

  if(onoff != CHECK_BATTER)
    lastonoff = onoff;

  lastslidemode = slide_mode;
  lastmode = mode;
  lastSealgear = Seal_gear;
  lastSlidegear = Slide_gear;
  lastload = load;
  lastcolormode = color;
}
/*************************************************************************************
* FunctionName	: Moto_Set_Funtion
* Description   :
* EntryParameter:
* ReturnValue   :  10ms 调用一次
**************************************************************************************/
void Moto_Set_Funtion(void)
{
  if(motor_data.Moto_Temp != motor_data.Moto_Flag)
  {
    if(motor_data.Moto_Flag)
    {
      motor_data.m_state = Start;
    }
  }
  motor_data.Moto_Temp = motor_data.Moto_Flag;

  switch(motor_data.m_state)
  {
    case Ready:
    {
      motor_data.m_run_Cnt = 0;
      break;
    }

    case Start:
    {
      motor_data.m_state = Runing;
      break;
    }

    case Runing:
    {
      motor_data.Electric_Machinery_Onoff = 1;
      if(++motor_data.m_run_Cnt > 20)
      {
        motor_data.m_state = End;
        motor_data.Electric_Machinery_Onoff = 0;
      }
      break;
    }

    case End:
    {
      motor_data.Electric_Machinery_Onoff = 0;
      motor_data.m_run_Cnt =0;
      motor_data.m_state = Ready;
      break;
    }

    default:
    {
      break;
    }

  }
}


