#include "includes.h"

_Moto_Control_Typedef  Moto_Control;
/*************************************************************************************
* FunctionName	: MOTO_Handle
* Description   : 
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void MOTO_Handle(void)
{
  if(Moto_Control.Electric_Machinery_Onoff == 1) ///�����
  {
    MOTO_ENABLE;
  }
  else
  {
    MOTO_DISABLE;
  }
}
//================================================================================================
// Descriptions:      ���������к��� ����һ����0.5S��
// Input parameters:        control  1 �򿪷���������0.5s  0�ر�
// Output parameters:  
// Returned value:
// Created by:        zhangx
// Created date:      2021.05.27
//------------------------------------------------------------------------------------------------
// Modified by:
// Modified date:
// Descriptions:
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
      Moto_Control.Electric_Machinery_Onoff = 1;			
    }
  }
  
  if((HAL_GetTick() - buzzertime) > 400) ///0.5s��رշ�����	
  {
    if(*control != 0)
    {
      Moto_Control.Electric_Machinery_Onoff = 0;
    }
    *last = 0;
    *control = 0;
  }
}
//================================================================================================
// Descriptions:      ���������к��� ����һ����0.1S��
// Input parameters:        control  1 �򿪷���������1.0s  0�ر�
// Output parameters:  
// Returned value:
// Created by:        zhangx
// Created date:      2021.05.27
//------------------------------------------------------------------------------------------------
// Modified by:
// Modified date:
// Descriptions:
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
      Moto_Control.Electric_Machinery_Onoff = 1;			
    }
  }
  
  if((HAL_GetTick() - buzzertime) > 200) ///1.0s��رշ�����	
  {
    if(*control != 0)
    {
      Moto_Control.Electric_Machinery_Onoff = 0;
    }
    *last = 0;
    *control = 0;
  }
}
/*************************************************************************************
* FunctionName	: MOTO_Handle
* Description   : 
* EntryParameter:
* ReturnValue   :
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
  //static uint8_t buzzer_onoff_flage;
  static uint8_t buzzer_load_flage;
  static uint8_t buzzer_color_flage;
  
  static uint8_t last_slidemode_flage;
  static uint8_t last_mode_flage;
  static uint8_t last_Seal_gear_flage;
  static uint8_t last_Slide_gear_flage;
  static uint8_t last_onoff_flage;
  static uint8_t last_load_flage;
  static uint8_t last_color_flage;	
  
  //  if(mode == 3)///����������ģʽ��
  //  {
  //    load = 0;
  //  }
  
  if(MACHINEON == onoff)///����ڿ���״̬��
  {
    //    if(lastonoff != onoff)									//������ػ��б仯����
    //    {
    //      // buzzer_onoff_flage = 1;
    //      // last_onoff_flage = 0;
    //    }
    if(lastslidemode != slide_mode)         //�л����ģʽ ��
    {
      buzzer_slidemode_flage = 1;
      last_slidemode_flage = 0;			
    }    
    if(lastmode != mode)									  //����ģʽ�л�EMS�������
    {
      if((lastmode == 0)||(lastmode == 1))
      {
        buzzer_mode_flage = 1;
        last_mode_flage = 0;			
      }
    }
    
    if(lastSealgear != Seal_gear)										//�����λ�б仯 ��
    {
      buzzer_Seal_gear_flage = 1;
      last_Seal_gear_flage = 0;
    }
    if(lastSlidegear!= Slide_gear)										//�����λ�б仯 ��
    {
      buzzer_Slide_gear_flage = 1;
      last_Slide_gear_flage = 0;
    }
    if(lastload == 0)
    {
      if(load == 1)                       // ������븺�� ��
      {
        // if(Key_State.Mode_choice_key == SEAL_MODE)///����ģʽ
        // {
        //   buzzer_load_flage = 1;
        //   last_load_flage = 0;
        // }
      }
    }
    if(lastcolormode != color)						//ʶ����Ĥ��ɫ����
    {
      buzzer_color_flage = 1;
      last_color_flage = 0;
    }    
  }
  else
  {
    if(lastonoff != onoff)								//������ػ��б仯
    {
      if(onoff != CHECK_BATTER)
      {
        buzzer_onoff_flage = 1;
      }
    }
  }
  
  buzzer0_5(&buzzer_onoff_flage,&last_onoff_flage);
  buzzer1_0(&buzzer_slidemode_flage, &last_slidemode_flage );
  buzzer1_0(&buzzer_mode_flage, &last_mode_flage );
  buzzer1_0(&buzzer_Seal_gear_flage, &last_Seal_gear_flage );
  buzzer1_0(&buzzer_Slide_gear_flage,&last_Slide_gear_flage);
  buzzer1_0(&buzzer_load_flage, &last_load_flage );
  buzzer1_0(&buzzer_color_flage,&last_color_flage);
  
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
* ReturnValue   :
**************************************************************************************/
void Moto_Set_Funtion(void)
{
  if(Moto_Control.Moto_Temp != Moto_Control.Moto_Flag)
  {
    if(Moto_Control.Moto_Flag)
    {
      Moto_Control.Moto_State = Start;
    }
  }
  Moto_Control.Moto_Temp = Moto_Control.Moto_Flag;
  
  switch(Moto_Control.Moto_State)
  {
  case Ready:
    Moto_Control.Moto_Cnt = 0;
    break;
    
  case Start:
    Moto_Control.Moto_State = Runing;
    break;
    
  case Runing:
    Moto_Control.Electric_Machinery_Onoff = 1;
    if(++Moto_Control.Moto_Cnt > 20)
    {
      Moto_Control.Moto_State = End;
      Moto_Control.Electric_Machinery_Onoff = 0;
    }
    break;
    
  case End:
    Moto_Control.Electric_Machinery_Onoff = 0;
    Moto_Control.Moto_Cnt =0;
    Moto_Control.Moto_State = Ready;
    break;
  default:break;
  }
}


