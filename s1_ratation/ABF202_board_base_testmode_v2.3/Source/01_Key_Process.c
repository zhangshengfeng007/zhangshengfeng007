#include "includes.h"

_Key_Flag_Struct       Key_Flag;
static struct_KeyInfo  g_KeyInfo1 = {0, 0, 0, 0, 0, 0, KeyDownCallBack,KeyUpCallBack}; 

volatile typedef struct
{
  _Key_Input KEY_NAME;
  uint16_t KEY_TIMES;
  uint8_t KEY_LONG_CNT;
}struct_Key_Set;

volatile const struct_Key_Set KEY_Comps[] =
{
  {KEY_POWER,75,10},//{KEY_POWER,75,10},
  {KEY_GEAR, 75,10},//{KEY_GEAR, 75,10}
};

void Key_SetDown_Function(uint8_t Info);
void Key_SetUp_Function(uint8_t Info);
void Key_SetLong_Function(uint8_t Info);
void Key_SetForWard_Function(uint8_t Info);

/**************************************************************************************
* FunctionName   : Key_Scan 
* Description    : 
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/ 
uint8_t Key_Scan(void)
{
  uint8_t  key_set = 0;    
  
  if(Power_key_Read_In == KEY_SET_DOWN)
  {
    key_set = KEY_Comps[0].KEY_NAME;  //
		Sys_Info.KEY_Value = 0x0001;
  }
  else if(Gear_key_Read_In == KEY_SET_DOWN)
  {
    key_set = KEY_Comps[1].KEY_NAME;  //
		Sys_Info.KEY_Value = 0x0002;
  }
  else
  {
    key_set = KEY_None;
//		Sys_Info.KEY_Value = 0;
  }      
  return key_set;
}
/**************************************************************************************
* FunctionName   : KeyDownCallBack 
* Description    : 
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/ 
uint8_t KeyDownCallBack(uint16_t Key, uint16_t Times) 
{ 
  uint8_t i;
  
  for(i=0;i<_KEY_MAX;i++)
  {
    if(Key == KEY_Comps[i].KEY_NAME)
    {
      if(Times < KEY_Comps[i].KEY_TIMES)
      {
        return _REENTER;
      }
    }
  }
  return _NO_REENTER;
} 
/**************************************************************************************
* FunctionName   : KeyUpCallBack 
* Description    : 
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/ 
void KeyUpCallBack(uint16_t Key)
{
  Key_Flag.Key_Short = 0;
  Key_Flag.Key_Long  = 0; 
}
/**************************************************************************************
* FunctionName   : DitherlessKey(struct_KeyInfo* pInfo) 
* Description    : 
* EntryParameter : 
* ReturnValue    : None
**************************************************************************************/ 
void DitherlessKey(struct_KeyInfo* pInfo) 
{ 
  switch(pInfo->KeyState) 
  { 
  case _HAS_NO_KEY:
    {
      pInfo->SameKeyCntr = 0; 
      if(pInfo->CurKey != _KEY_NONE) 
      { 
        pInfo->KeyState = _HAS_KEY_DOWN; 
      }            
    }break;
    
  case _HAS_KEY_DOWN:
    {
      if(pInfo->PreDownKey == pInfo->CurKey) 
      { 
        pInfo->KeyState = _HAS_KEY_SURE; 
      } 
      else 
      { 
        pInfo->KeyState = _HAS_NO_KEY; 
      }
    }break;
    
  case _HAS_KEY_SURE:
    {
      if(pInfo->CurKey == pInfo->PreDownKey) 
      { 
        pInfo->KeyState = _HAS_KEY_WAITUP; 
        if(pInfo->KeyDownCallBack) 
        { 
          if(_REENTER == pInfo->KeyDownCallBack(pInfo->CurKey, pInfo->SameKeyCntr))
          { 
            pInfo->KeyState = _HAS_KEY_SURE; 
            ++pInfo->SameKeyCntr; 
          } 
        } 
      } 
      else 
      { 
        pInfo->KeyState = _KEY_NONE; 
      }  
    }break;
    
  case _HAS_KEY_WAITUP:
    {
      if(pInfo->CurKey != pInfo->PreDownKey) 
      { 
        pInfo->KeyState = _HAS_NO_KEY; 
        
        if(pInfo->KeyUpCallBack) 
        { 
          pInfo->KeyUpCallBack(pInfo->PreDownKey); 
        } 
      } 
    }break;
    
  default: break; 
  }
  
  pInfo->PreDownKey = pInfo->CurKey; 
  
  return; 
}

/**************************************************************************************
* FunctionName   : Key_Stroke()
* Description    : 
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Key_Stroke(void)
{  
  if(g_KeyInfo1.KeyState == _HAS_KEY_SURE)
  {
    if(Key_Flag.Key_Short == 0)
    {
      Key_Flag.Key_Short = 1;
      Key_Flag.Key_Short_Set = 1;
      g_KeyInfo1.LastKey = g_KeyInfo1.CurKey;
      Key_SetDown_Function(g_KeyInfo1.CurKey);
    }        
  }
  else if(g_KeyInfo1.KeyState == _HAS_NO_KEY)
  {
    Key_Flag.Key_Short = 0;
    Key_Flag.Key_Long  = 0;
    g_KeyInfo1.Long_Key_t = 0;
    if(Key_Flag.Key_Short_Set)
    {
      Key_Flag.Key_Short_Set = 0;
      Key_SetUp_Function(g_KeyInfo1.LastKey);
      g_KeyInfo1.LastKey = 0;
    }
  }    
  else if(g_KeyInfo1.KeyState == _HAS_KEY_WAITUP)    
  {
    uint8_t i;
    if(Key_Flag.Key_Long == 0) 
    {
      Key_Flag.Key_Long = 1;
      Key_Flag.Key_Short_Set = 0;
      Key_SetLong_Function(g_KeyInfo1.CurKey);
    }
#if 0
    if(++g_KeyInfo1.Long_Key_t > 10)
    {
      g_KeyInfo1.Long_Key_t = 0;
      Key_SetForWard_Function(g_KeyInfo1.LastKey);
    }
#endif
    for(i=0;i<_KEY_MAX;i++)
    {
      if(++g_KeyInfo1.Long_Key_t > KEY_Comps[i].KEY_LONG_CNT)
      {
        g_KeyInfo1.Long_Key_t = 0;
        Key_SetForWard_Function(g_KeyInfo1.LastKey);
      }
    }
  }
}
/**************************************************************************************
* FunctionName   : Key_Stroke()
* Description    : 
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Key_Proc(void)
{ 
  uint16_t temp;    
  temp = Key_Scan();     
  g_KeyInfo1.CurKey = temp & 0x00FF;                 
  DitherlessKey(&g_KeyInfo1);
  Key_Stroke();
}

void Key_SetDown_Function(uint8_t Info)
{
  switch(Info)
  {
  case KEY_POWER:Key_S1_SetDown_Str();break;
  case KEY_GEAR: break;
  default: break;
  }
}
void Key_SetUp_Function(uint8_t Info)
{
  switch(Info) 
  {
  case KEY_POWER:Key_S1_Str();break;
  case KEY_GEAR: Key_S2_Str();break;
  default: break;
  }
}
void Key_SetLong_Function(uint8_t Info)
{
  switch(Info)
  {
  case KEY_POWER:Key_S1_Long_Str();break;
  case KEY_GEAR: Key_S2_Long_Str();break;
  default: break;
  }
}
void Key_SetForWard_Function(uint8_t Info)
{
  switch(Info) 
  {
  default: break; 
  } 
}

void Key_S1_SetDown_Str(void)
{
//  if(Sys_Info.Power_State == MACHINEOFF)
//  {
//    Sys_Info.Key_State.Bus_Power_Up_Flag = 1;
//  }
}
/**************************************************************************************
* FunctionName   : Key_S1_Str
* Description    : 
* EntryParameter : 
* ReturnValue    :
**************************************************************************************/ 
void Key_S1_Str(void) //
{
  if(Sys_Info.Power_State == MACHINEON)
  {
//    if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)
//    {
//      Sys_Info.Key_State.Mode_choice_key = SLIDE_MODE;
//    }
//    else
//    {
//      Sys_Info.Key_State.Mode_choice_key = SEAL_MODE;
//    }    
	  Sys_Info.Key_State.Mode_choice_key = SLIDE_MODE;    //---2022 12 30 --模式按键功能取消，只改为滑动模式
  }
  else
  {
			if(HAL_GetTick() > 100)
			Sys_Info.Power_State = CHECK_BATTER;
  }
}
/**************************************************************************************
* FunctionName   : Key_S1_Long_Str
* Description    : 
* EntryParameter : 
* ReturnValue    :
**************************************************************************************/ 
void Key_S1_Long_Str(void)
{
  if(Sys_Info.Power_State == MACHINEOFF)
  {
    Sys_Power_On();
  }
  else
  {
    Sys_Power_Off();
  }
}
/**************************************************************************************
* FunctionName   : Key_S2_Str
* Description    : 
* EntryParameter : 
* ReturnValue    :
**************************************************************************************/ 
void Key_S2_Str(void)  //
{
  static uint8_t big_little_seal = 0;
  static uint8_t big_little_slide = 0;
  
  if(Sys_Info.Power_State == MACHINEON)
  {
	  Uart2_Send_Data();   // 2023 01 11 调试打开
	  
    if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)   //盖章模式
    {
      if(Sys_Info.Key_State.Seal_gear >= GEARTHREE)
      {
        Sys_Info.Key_State.Seal_gear = GEARTHREE;
        big_little_seal = 1;
      }
      if(Sys_Info.Key_State.Seal_gear < GEARTWO)
      {
        Sys_Info.Key_State.Seal_gear = GEARONE;
        big_little_seal = 0;
      }
      
      if(!big_little_seal)
      {
        if(Sys_Info.Key_State.Seal_gear < GEARTHREE)
        {
          Sys_Info.Key_State.Seal_gear++;
        }
      }    
      else
      {
        if(Sys_Info.Key_State.Seal_gear > GEARONE)
          Sys_Info.Key_State.Seal_gear--;
      }
    }
    else                //滑动模式
    {
      if(Sys_Info.Key_State.Slide_gear >= GEARTHREE)
      {
        Sys_Info.Key_State.Slide_gear = GEARTHREE;
        big_little_slide = 1;
      }
      if(Sys_Info.Key_State.Slide_gear < GEARTWO)
      {
        Sys_Info.Key_State.Slide_gear = GEARONE;
        big_little_slide = 0;
      }
      
      if(!big_little_slide)
      {
        if(Sys_Info.Key_State.Slide_gear < GEARTHREE)
        {
          Sys_Info.Key_State.Slide_gear++;
        }
        
      }    
      else
      {
        if(Sys_Info.Key_State.Slide_gear > GEARONE)
        {
          Sys_Info.Key_State.Slide_gear--;
        }
      }
    }
    
//    if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)
//    {
//      Sys_Info.Key_State.Gears_key = Sys_Info.Key_State.Seal_gear;
//    }
//    else
//    {
//      Sys_Info.Key_State.Gears_key = Sys_Info.Key_State.Slide_gear;
//    }
  }
  else if(Sys_Info.Power_State == CHECK_BATTER)
  {
    static uint8_t Gear_Key_Set_Cnt = 0;
    if(HAL_GetTick() < 2000)
    {
      Gear_Key_Set_Cnt++;
      if(Gear_Key_Set_Cnt >= 3)//5
      {
        Gear_Key_Set_Cnt = 0;
        Sys_Info.Work_Mode = CALIBRATION;
		#ifdef COLOR_SENSOR_ENABLE
        RGB_Sensor_Calibration();  //进入颜色传感器校准模式
		#endif
      }	  
    }
  }
	else if(Sys_Info.Power_State == MACHINEOFF)
	{
    static uint8_t Gear_Key_Set_Cnt1 = 0;
    if(HAL_GetTick() < 2000)
    {
      Gear_Key_Set_Cnt1++;
      if(Gear_Key_Set_Cnt1 >= 5)//5
      {
        Gear_Key_Set_Cnt1 = 0;
				if(Sys_Info.Charge_State == CHARGING_STA)
				{
//					BAT_CHARGE_DISABLE;
					Sys_Info.Work_Mode = TESTMODE;
					Sys_Info.Test_Mode.TestMode = Aging_TestModel;
					Sys_Info.Power_State = MACHINEON;
//					Sys_Power_On();
					//OVP_DISABLE;
				}
      }
    }
	}	  
}
/**************************************************************************************
* FunctionName   : Key_S2_Long_Str
* Description    : 
* EntryParameter : 
* ReturnValue    :
**************************************************************************************/ 
void Key_S2_Long_Str(void)
{
  if(Sys_Info.Power_State == MACHINEON)
  {
    if(Sys_Info.Key_State.Mode_choice_key == SLIDE_MODE)  //滑动模式
    {
//      if(Sys_Info.Key_State.Mode_key == RF_WITH_EMS)
//      {
//        Sys_Info.Key_State.Mode_key = RF_WITHOUT_EMS;
//      }
//      else
//      {
//        Sys_Info.Key_State.Mode_key = RF_WITH_EMS;
//      }                                          //---长按按键 切换 是否带有ems输出 20221230 关闭该功能
		Sys_Info.Key_State.Mode_key = RF_WITHOUT_EMS;
    }
  }
	else if((Sys_Info.Power_State == MACHINEOFF)&&(Sys_Info.Charge_State == CHARGING_STA))
	{
		if(HAL_GetTick() < 6000)
		{
			Sys_Info.Work_Mode = TESTMODE;
			Sys_Info.Test_Mode.TestMode = Life_TestModel;
			Sys_Info.Power_State = MACHINEON;			
		}
	}
}


