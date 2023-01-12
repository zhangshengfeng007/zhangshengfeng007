#include "includes.h"

#define BAT_CHARGE_LEVEL_3  4050
#define BAT_CHARGE_LEVEL_2  3890
#define BAT_CHARGE_LEVEL_1  3760

#define BAT_USER_LEVEL_2  3450
#define BAT_USER_LEVEL_1  3300

#define BAT_NORMAL_10    (BAT_CHARGE_LEVEL_1-85)
#define BAT_NORMAL_30    (BAT_CHARGE_LEVEL_2-85)//3800//3800
#define BAT_NORMAL_70    (BAT_CHARGE_LEVEL_3-85)//3950//4000
#define BAT_NORMAL_100   4200

#define BAT_LEV_TOLERANCE  30 ///电池电压容差范围

_Batt_State_Typedef Batt_State;
/*************************************************************************************
* FunctionName	 : PowerOnElectricQuantity()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
////电量判断
void PowerOnElectricQuantity(uint16_t bat_V)   //开机空载状态
{
  if((bat_V > (BAT_NORMAL_70 - BAT_LEV_TOLERANCE))&&(bat_V < (BAT_NORMAL_70 + BAT_LEV_TOLERANCE)))
  {
    if(Batt_State.BatteryLevel != Batt_Level_Gear_2) Batt_State.BatteryLevel = Batt_Level_Gear_3;
  }
  else if((bat_V > (BAT_NORMAL_30 - BAT_LEV_TOLERANCE))&&(bat_V < (BAT_NORMAL_30 + BAT_LEV_TOLERANCE)))
  {
    if(Batt_State.BatteryLevel != Batt_Level_Gear_1) Batt_State.BatteryLevel = Batt_Level_Gear_2;
  }
  else if((bat_V > (BAT_NORMAL_10 - BAT_LEV_TOLERANCE))&&(bat_V < (BAT_NORMAL_10 + BAT_LEV_TOLERANCE)))
  {
    if(Batt_State.BatteryLevel != Batt_Level_Gear_0) Batt_State.BatteryLevel = Batt_Level_Gear_1;
  }
  else
  {
//    if(bat_V > BAT_NORMAL_70)     //50-75 亮3个灯
//    {
//      Batt_State.BatteryLevel = Batt_Level_Gear_3;
//    }
//    else if((bat_V <= BAT_NORMAL_70)&&(bat_V > BAT_NORMAL_30)) //25-50 亮2个灯
//    {
//      Batt_State.BatteryLevel = Batt_Level_Gear_2;
//    }				
//    else if((bat_V <= BAT_NORMAL_30)&&(bat_V > BAT_NORMAL_10)) //10-25 亮一个灯
//    {
//      Batt_State.BatteryLevel = Batt_Level_Gear_1;
//    }
//    else /// < 10 所有灯闪烁
//    {
//      Batt_State.BatteryLevel = Batt_Level_Gear_0;
//    }
    
    if(bat_V > BAT_NORMAL_70)
    {
      Batt_State.BatteryLevel = Batt_Level_Gear_3;
    }
    else if(bat_V > BAT_NORMAL_30)
    {
      Batt_State.BatteryLevel = Batt_Level_Gear_2;
    }
    else if(bat_V > BAT_NORMAL_10)
    {
      Batt_State.BatteryLevel = Batt_Level_Gear_1;
    }
    else
    {
      Batt_State.BatteryLevel = Batt_Level_Gear_0;
    }
  }
}
/*************************************************************************************
* FunctionName	 : UsingElectricQuantity()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
////使用过程中低电量判断
void UsingElectricQuantity(uint16_t bat_V)
{
  if((bat_V > BAT_USER_LEVEL_2 - BAT_LEV_TOLERANCE)&&(bat_V < BAT_USER_LEVEL_2 + BAT_LEV_TOLERANCE))
  {
    Batt_State.BatteryLevel = Batt_Level_SP20;//10%~20%
  }
  else if((bat_V > BAT_USER_LEVEL_1 - BAT_LEV_TOLERANCE)&&(bat_V < BAT_USER_LEVEL_1 + BAT_LEV_TOLERANCE))
  {
    Batt_State.BatteryLevel = Batt_Level_SP10;//0%~10%
  }
	else if(bat_V < BAT_USER_LEVEL_1)
	{
		Batt_State.BatteryLevel = Batt_Level_SP10;//0%~10%
	}
}
/*************************************************************************************
* FunctionName	 : StateOfChargeBatteryVoltage()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
////充电状态电池电压判断
void StateOfChargeBatteryVoltage(uint16_t bat_V)
{
  if((bat_V > (BAT_CHARGE_LEVEL_3 - BAT_LEV_TOLERANCE))&&(bat_V < (BAT_CHARGE_LEVEL_3 + BAT_LEV_TOLERANCE)))
  {
    if(Batt_State.BatteryLevel != Batt_Level_Gear_2) Batt_State.BatteryLevel = Batt_Level_Gear_3;
  }
  else if((bat_V > (BAT_CHARGE_LEVEL_2 - BAT_LEV_TOLERANCE))&&(bat_V < (BAT_CHARGE_LEVEL_2 + BAT_LEV_TOLERANCE)))
  {
    if(Batt_State.BatteryLevel != Batt_Level_Gear_1) Batt_State.BatteryLevel = Batt_Level_Gear_2;
  }
  else if((bat_V > (BAT_CHARGE_LEVEL_1 - BAT_LEV_TOLERANCE))&&(bat_V < (BAT_CHARGE_LEVEL_1 + BAT_LEV_TOLERANCE)))
  {
    if(Batt_State.BatteryLevel != Batt_Level_Gear_0) Batt_State.BatteryLevel = Batt_Level_Gear_1;
  }
  else
  {
    if(bat_V > BAT_CHARGE_LEVEL_3)
    {
      if((BAT_CHARGE_STATE_IN == GPIO_PIN_SET))//||(bat_V > 4150))
      {
        Batt_State.BatteryLevel = Batt_Level_Full;
      }
      else
      {
        Batt_State.BatteryLevel = Batt_Level_Gear_3;
      }
    }
    else if((bat_V <= BAT_CHARGE_LEVEL_3)&&(bat_V > BAT_CHARGE_LEVEL_2)) 
    {
      Batt_State.BatteryLevel = Batt_Level_Gear_2;
    }				
    else if((bat_V <= BAT_CHARGE_LEVEL_2)&&(bat_V > BAT_CHARGE_LEVEL_1))
    {
      Batt_State.BatteryLevel = Batt_Level_Gear_1;
    }
    else /// < 10 所有灯闪烁
    {
      ;
    }		
  }
}
//------------------------------------------------------------------------------------------------
uint16_t Get_Batt_Value(uint16_t adc_data)
{
  uint16_t value;
  value = adc_data*5000/4096;		//res:100k+100k
  return value;
}

uint16_t Get_Usb_Value(uint16_t adc_data)
{
  uint16_t value;
  value = adc_data*6000/4096;		//res:140k+100k
  return value;
}
/*************************************************************************************
* FunctionName	 : BatteryPowerJudgment()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void BatteryPowerJudgment(_sys_state_e state)
{  
  static uint8_t charge_check = 0;
  static uint32_t check_time = 0;
	
  Batt_State.BATT_TRUE_Value = Get_Batt_Value(ADC_Data.bat_val);
  Batt_State.USB_TRUE_Value  = Get_Usb_Value(ADC_Data.Usb_Val);
  
  if(USB_INPUT_CHECK_IN == GPIO_PIN_RESET)  
  {
    if(Batt_State.USB_TRUE_Value > 4000) //输入引脚拉低+输入电压大于4V判断为USB接入
    {
      Sys_Info.Charge_State = CHARGING_STA;  // 2023 01 10 关闭
      
      if(!charge_check)
      {
        check_time = HAL_GetTick();
        charge_check = 1;
      }
      if(HAL_GetTick()-check_time > 2000)
      {
        CHARGE_LIMIT_ENABLE;
      }
    }   //输入引脚拉低+输入电压小于4V判断为USB接入，OVP断开
    else
    {
      charge_check = 0;
      Sys_Info.Charge_State = No_CHARGE_STA;
    }
  }
  else
  {
    charge_check = 0;
    Sys_Info.Charge_State = No_CHARGE_STA;
  }
  
  if(Sys_Info.Charge_State == No_CHARGE_STA)
  {
    if(state == MACHINEOFF)
    {
			if(Sys_Info.Adc_First_Run_Flage)
      PowerOnElectricQuantity(Batt_State.BATT_TRUE_Value);   //查看电量
    }
    else
    {
      UsingElectricQuantity(Batt_State.BATT_TRUE_Value);     //使用过程中
    }
  }
  else
  {
    StateOfChargeBatteryVoltage(Batt_State.BATT_TRUE_Value); //充电
  }
}


/*
电池不同的状态进行判断电池电量的方式有所区别
1. 电量识别状态 在未开机的情况下进行
2. 工作状态
3. 充电状态  充电中  充电完成

*/
