#include "includes.h"

#define GearOne_Duty 7
#define GearTwo_Duty 12
#define GearThree_Duty 15
#define Max_Time 20
/**************************************************************************************
* FunctionName   : SealMode_Service_Function
* Description    : 盖章模式下的输出切换动作，档位变化，占空比变化
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void SealMode_Service_Function(void)
{
  static uint8_t time_cnt = 0;
  static uint8_t outleveltime = 0 ;
  
  switch(Sys_Info.Key_State.Seal_gear)
  {
  case GEARONE:   outleveltime = GearOne_Duty;   break;
  case GEARTWO:   outleveltime = GearTwo_Duty;   break;
  case GEARTHREE: outleveltime = GearThree_Duty; break;
  default:break;
  }
  
  time_cnt++;
  
  if(time_cnt <= outleveltime)
  {
    if(time_cnt <= 1)
    {
      EMS_OUT_Stop();
      RF_OUT_Stop();
    }
    else
    {
      EMS_OUT_Stop();
      RF_OUT_Start();
    }
  }
  else if((time_cnt > outleveltime)&&(time_cnt <= Max_Time))
  {
    if(time_cnt <= outleveltime+1)
    {
      EMS_OUT_Stop();
      RF_OUT_Stop();
    }
    else
    {
      RF_OUT_Stop();
			if(Sys_Info.Test_Mode.TestMode!=Auto_TestModel)
      EMS_OUT_Start();
    }
  }
  else
  {
    time_cnt = 0;
  }
  
//  if(time_cnt >= Max_Time)
//  {
//    time_cnt = 0;
//  }
}
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void SealModeOutput_Six(uint16_t time)
{
  switch(time)
  {
  case 6000: Set_Ems_Tim_Psc(31);break;//20K
  case 5000: Set_Ems_Tim_Psc(43); break;//14.5
  case 4000: Set_Ems_Tim_Psc(26); break;//23.7 
  case 3000: Set_Ems_Tim_Psc(43); break;//14.5 
  case 2000: Set_Ems_Tim_Psc(26); break;//23.7 
  case 1000: Set_Ems_Tim_Psc(43); break;//14.5 
  case 0  :  Set_Ems_Tim_Psc(26); break;//23.7
  default: break;
  }
}
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void SealModeOutput_Seven(uint16_t time)
{
  switch(time)
  {
  case 6000:Set_Ems_Tim_Psc(31);break;//20K
  case 0   :Set_Ems_Tim_Psc(26);break;//23.7
  default: break;
  }
}
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void SealModeOutput_Eight(uint16_t time)
{
  switch(time)
  {
  case 0   : Set_Ems_Tim_Psc(26); break;
  case 400 : Set_Ems_Tim_Psc(43); break;
  case 2000: Set_Ems_Tim_Psc(26); break; 
  case 2400: Set_Ems_Tim_Psc(43); break; 
  case 4000: Set_Ems_Tim_Psc(26); break; 
  case 4400: Set_Ems_Tim_Psc(43); break; 
  case 6000: Set_Ems_Tim_Psc(31); break;
  default: break;
  }
}
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void SealModeOutput_Nine(uint16_t time)
{
  switch(time)
  {
  case 0   : Set_Ems_Tim_Psc(26); break;
  case 1600: Set_Ems_Tim_Psc(43); break;
  case 2000: Set_Ems_Tim_Psc(26); break; 
  case 3600: Set_Ems_Tim_Psc(43); break; 
  case 4000: Set_Ems_Tim_Psc(26); break; 
  case 5600: Set_Ems_Tim_Psc(43); break; 
  case 6000: Set_Ems_Tim_Psc(31); break;
  default: break;
  }
}
/**************************************************************************************
* FunctionName   : SealModeOutput_Mode
* Description    : 盖章模式下，各颜色的输出模式，不同颜色EMS输出不一样，RF一致
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void SealModeOutput_Mode(_COLOUR_Enum_Typedef colour,uint32_t time)
{
  switch(colour)
  {
  case COLOUR_ORANGE : SealModeOutput_Six(time);break;   
  case COLOUR_BLUE   : SealModeOutput_Seven(time);break;
  case COLOUR_GREEN  : SealModeOutput_Nine(time);break;
  case COLOUR_BROWN  : SealModeOutput_Eight(time);break;
  default: SealModeOutput_Seven(time);break;
  }
  SealMode_Service_Function();
}
/*************************************************************************************
* FunctionName	: SEAL_MODE_RF_EMS_NOMEL_OUTPUT
* Description   : 普通盖章模式
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void SEAL_MODE_RF_EMS_NOMEL_OUTPUT(uint32_t time)
{
  SealModeOutput_Mode(RGB_Handle.Colour_Type,time);
}
/*************************************************************************************
* FunctionName	: SEAL_MODE_RF_EMS_PLUS_OUTPUT
* Description   : 增强盖章模式
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void SEAL_MODE_RF_EMS_PLUS_OUTPUT(void)
{
  ;
}
/*************************************************************************************
* FunctionName	: SLIDE_MODE_RF_EMS_OUPUT
* Description   : 滑动模式+EMS
* EntryParameter:
* ReturnValue   :   1ms调用一次
**************************************************************************************/
void SLIDE_MODE_RF_EMS_OUPUT(void)
{
  static uint8_t time_cnt = 0;
  time_cnt++;
  
  if(Sys_Info.G_sensor_active_flage)
  {
		Sys_Info.timestart_cnt++;
		if(Sys_Info.timestart_cnt>49)
		{	
			Sys_Info.timestart_cnt = 50;
			Set_Ems_Tim_Psc(31);//20K
			switch(time_cnt)
			{
			case 1 :EMS_OUT_Stop(); RF_OUT_Start();break;
			case 20:RF_OUT_Stop(); break;
			case 21:RF_OUT_Stop(); EMS_OUT_Start();break;
			case 24:EMS_OUT_Stop();time_cnt = 0;break;		
			default:break;
			}
		}
  }
  else
  {
		Sys_Info.timestart_cnt = 0;
#if 1
    switch(time_cnt)
    {
    case 1 :EMS_OUT_Stop(); break;
    case 20:RF_OUT_Stop(); break;
    case 21:RF_OUT_Stop();EMS_OUT_Start();break;//
    case 24:EMS_OUT_Stop();time_cnt = 0;break;		
    default:break;
    } 
#else
    Set_Ems_Tim_Psc(20);
    switch(time_cnt)
    {
    case 1 :EMS_OUT_Stop(); RF_OUT_Start();break;
    case 11:RF_OUT_Stop(); break;
    case 16:RF_OUT_Stop(); EMS_OUT_Start();break;
    case 19:EMS_OUT_Stop();break;
    case 24:time_cnt = 0;break;		
    default:break;
    }
#endif
  }
  if(time_cnt > 24)
  {
    time_cnt = 0;
  }
}
/*************************************************************************************
* FunctionName	: SLIDE_MODE_RF_NoEMS_OUPUT
* Description   : 滑动模式noEMS
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void SLIDE_MODE_RF_NoEMS_OUPUT(void)
{
  static uint8_t time_cnt = 0;
  time_cnt++;
  
  if(Sys_Info.G_sensor_active_flage)
  {
		Sys_Info.timestart_cnt++;
		if(Sys_Info.timestart_cnt>49)
		{
			Sys_Info.timestart_cnt = 50;		
			switch(time_cnt)
			{
				case 1 :EMS_OUT_Stop();RF_OUT_Start();break;
				case 20:RF_OUT_Stop(); break;
				case 21:RF_OUT_Stop(); EMS_OUT_Stop();break;
				case 24:EMS_OUT_Stop();time_cnt = 0;break;		
				default:break;
			}
		}			
  }
  else
  {
		Sys_Info.timestart_cnt = 0;
    switch(time_cnt)
    {
    case 1 :EMS_OUT_Stop();break;
    case 20:RF_OUT_Stop(); break;
    case 21:RF_OUT_Stop(); EMS_OUT_Stop();break;
    case 24:EMS_OUT_Stop();time_cnt = 0;break;
    default:break;
    } 
  }
  if(time_cnt > 24)
  {
    time_cnt = 0;
  }
}
/*************************************************************************************
* FunctionName	: Seal_Check_Skin_Output
* Description   : 降低频率输出用于接触检测
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void Seal_Check_Skin_Output(void)
{
  static uint8_t time_cnt = 0;
  
  Set_RF_Tim_Psc(15);  //先把RF调整到1Mhz
//  time_cnt++;                              //----20221230 关闭离开皮肤后不输出rf，
//  switch(time_cnt)
//  {
//  case 1 :EMS_OUT_Stop();RF_OUT_Start();break;
//  case 5 :RF_OUT_Stop(); break;
//  case 25:time_cnt = 0;break;		
//  default:break;
//  }
}
/*************************************************************************************
* FunctionName	: RF_EMS_OUTPUT_Function
* Description   : RF,EMS输出函数
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
#define Seal_Looptime 6000  //单位1ms
void RF_EMS_OUTPUT_Function(void)
{
  if(Sys_Info.Power_State == MACHINEON)
  {
    //-------------------------缓启动RF及EMS功率电路---------------------
    if(Sys_Info.PowerOn_Time_Counter > EMS_Start_DelayTime+8)
    {
      ;
    }    
    else if(Sys_Info.PowerOn_Time_Counter > EMS_Start_DelayTime+5)
    {
      //EMS_SET_VALUE_PWM_Start(); //启动调压PWM
      EMS_OUT_PWM_Start(); //启动输出PWM
    }
    else if(Sys_Info.PowerOn_Time_Counter > RF_Start_DelayTime+5)
    {
      //RF_SET_VALUE_PWM_Start(); //启动调压PWM
      RF_OUT_PWM_Start(); //启动输出PWM
    }

    //-------------------------------------------------------------------
    if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)  //盖章模式
    {
      if(RF_Handler.RF_Load_Flag) //接触皮肤
      {
        Sys_Info.SealMode_Conter++;
        
#ifdef Seal_RF_Freq_Conver   //盖章模式变频
        
        if(Sys_Info.SealMode_Conter < 1000)
        {
          Set_RF_Tim_Psc(7);   //2Mhz
        }
        else if(Sys_Info.SealMode_Conter < 3000)
        {
          Set_RF_Tim_Psc(10);  //1.45Mhz
        }
        else
        {
          Set_RF_Tim_Psc(15);  //1Mhz
        }
#endif
        if(Sys_Info.SealMode_Conter > Seal_Looptime) //单次Loop输出超时后，
        {
          Seal_Check_Skin_Output(); //进入皮肤接触检测动作
          Sys_Info.Slide_effective_flag = 0; //盖章模式有效时间标志
          Moto_Control.Moto_Flag = SEAL_TIME_END;
        }
        else
        {
          if(RF_Handler.RF_Load_Out_Scope) //过流保护，关闭输出
          {
            EMS_OUT_Stop(); RF_OUT_Stop();
            Sys_Info.Slide_effective_flag = 0; //盖章模式有效时间标志
          }
          else
          {
            Sys_Info.Slide_effective_flag = 1; //盖章模式有效时间标志
            SEAL_MODE_RF_EMS_NOMEL_OUTPUT(Sys_Info.SealMode_Conter); //单次Loop输出函数
          }
        }
      }
      else        //离开皮肤
      {
        Set_Ems_Tim_Psc(31);
        Sys_Info.SealMode_Conter = 0;
        Moto_Control.Moto_Flag  = None;
        Seal_Check_Skin_Output(); //进入皮肤接触检测动作
        Sys_Info.Slide_effective_flag = 0; //盖章模式有效时间标志
      }
      Sys_Info.SildeMode_Conter = 0;
    }
    else   //滑动模式
    { 
		RF_Handler.RF_Load_Flag = 1; // 2023 01 10 
      if(RF_Handler.RF_Load_Flag) //接触皮肤
      {
#ifdef Silde_RF_Freq_Conver   //滑动模式变频
        
        Sys_Info.SildeMode_Conter++;
        
        if(Sys_Info.SildeMode_Conter < 500)
        {
          Set_RF_Tim_Psc(7);   //2Mhz
        }
        else if(Sys_Info.SildeMode_Conter < 2000)
        {
          Set_RF_Tim_Psc(10);  //1.45Mhz
        }
        else
        {
          Set_RF_Tim_Psc(15);  //1Mhz
        }
#endif
        if(RF_Handler.RF_Load_Out_Scope) //过流保护关闭输出
        {
          EMS_OUT_Stop(); RF_OUT_Stop();
        }
        else
        {
//          if(Sys_Info.Key_State.Mode_key == RF_WITH_EMS) //有EMS输出
//          {
//            SLIDE_MODE_RF_EMS_OUPUT();
//          }
//          else      																	//关闭EMS输出
          {
            SLIDE_MODE_RF_NoEMS_OUPUT();
          }
        }
      }
      else        //离开皮肤
      {
        Sys_Info.SildeMode_Conter = 0;
        Seal_Check_Skin_Output(); //进入皮肤接触检测动作
      }
      if(Sys_Info.SealMode_Conter)
      {
        Moto_Control.Moto_Flag = None;
      }
      Sys_Info.SealMode_Conter = 0;
    }
  }
  else  //关机
  {
    //---------关闭EMS和RF的调压和PWM----------
    EMS_OUT_Stop(); 
    RF_OUT_Stop();
    RF_OUT_PWM_Stop(); //输出PWM 
//    RF_SET_VALUE_PWM_Stop(); //调压PWM
    EMS_OUT_PWM_Stop(); //输出PWM
//    EMS_SET_VALUE_PWM_Stop(); //调压PWM
  }
}
/*************************************************************************************
* FunctionName	: HAL_TIM_PeriodElapsedCallback
* Description   : tim回调
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  if(htim == &htim3)
//  {
//    RF_EMS_OUTPUT_Function();
//  }
//}



