#include "includes.h"

/*
   充电检测脚 硬件电路接线图
	-----DC_5V  （4.8v--5.2v）
          |
	------140K
	      |----charge_pin_adc
	------100k
	      |
	------GND

	ADC参考电压 2.5v ---对应4096
	charge_pin_vol = 5v*100/(100+140) = 2.0833v （2.0v --2.16v ）

	charge_pin_adc_val = charge_pin_vol * 4096 / 2.5(参考电压)
	                   = 3412 (理论值) （3276 -- 3583）
*/


/*
	电池 电量检测控制脚 硬件接线图
	---BAT (3.2V --4.2V)
		|
	---100K
		|  ---BAT_ADC_PIN  (MAX-VAL 2.1V)
	---100K
		|
	---GND

	ADC 参考电压 2.5v ---对应4096

	ADC_VAL = pin_val * 4096 /2.5v
*/

#define CHARGE_M_LVL3_VAL   (BAT_4050mV_ADC_VAL) //4050
#define CHARGE_M_LVL2_VAL   (BAT_3900mV_ADC_VAL) //3890
#define CHARGE_M_LVL1_VAL   (BAT_3750mV_ADC_VAL)//3760

#define USER_M_LVL2_VAL    (BAT_3450mV_ADC_VAL) //3450
#define USER_M_LVL1_VAL    (BAT_3300mV_ADC_VAL) //3300

#define PERCENT_10_VAL    (CHARGE_M_LVL1_VAL - 85)
#define PERCENT_30_VAL    (CHARGE_M_LVL2_VAL - 85)//3800//3800
#define PERCENT_70_VAL    (CHARGE_M_LVL3_VAL - 85)//3950//4000
#define PERCENT_100_VAL   (BAT_4200mV_ADC_VAL)//4200

#define BAT_FLOAT_VAL     30 ///电池电压容差范围

_bat_data_t bat_data;
/*************************************************************************************
* FunctionName	 : bat_check_in_sleep_mode()
* Description    :   --------------3.675v ------------------------3.805v -------------------------3.965v ----
* describe :     : --------- |3.675v(+/-)range| --------------|3.805v(+/-)range|-------------|3.965v(+/-)range|----
*                : -lvl0 --- if（not ==lvl0）lvl1 -----lvl1-- if(not lvl1) lvl2  ---lvl2 ------if(not lvl2) lvl3 --lvl3
*
* ReturnValue    : None 电池无RF 和 EMS输出时 电量判断标准
**************************************************************************************/
void bat_check_in_sleep_mode(uint16_t bat_V)
{
  if((bat_V > (PERCENT_70_VAL - BAT_FLOAT_VAL))&&(bat_V < (PERCENT_70_VAL + BAT_FLOAT_VAL)))
  {
    if(bat_data.disp_level != Bat_Level2) bat_data.disp_level = Bat_Level3;
  }
  else if((bat_V > (PERCENT_30_VAL - BAT_FLOAT_VAL))&&(bat_V < (PERCENT_30_VAL + BAT_FLOAT_VAL)))
  {
    if(bat_data.disp_level != Bat_Level1) bat_data.disp_level = Bat_Level2;
  }
  else if((bat_V > (PERCENT_10_VAL - BAT_FLOAT_VAL))&&(bat_V < (PERCENT_10_VAL + BAT_FLOAT_VAL)))
  {
    if(bat_data.disp_level != Bat_Level0) bat_data.disp_level = Bat_Level1;
  }
  else
  {
    if(bat_V > PERCENT_70_VAL)
    {
      bat_data.disp_level = Bat_Level3;
    }
    else if(bat_V > PERCENT_30_VAL)
    {
      bat_data.disp_level = Bat_Level2;
    }
    else if(bat_V > PERCENT_10_VAL)
    {
      bat_data.disp_level = Bat_Level1;
    }
    else
    {
      bat_data.disp_level = Bat_Level0;
    }
  }
}
/*************************************************************************************
* FunctionName	 : bat_check_in_normal_mode()
* Description    :
* describe :  ------------3.3v---------------------------3.45v------------------
*             ---------|3.3v+/range|------------------| 3.45v +/ range| -----
*             lvl_SP10 --lvl_SP10  ---lvl_SP10----------Bat_Level_SP20
*
* ReturnValue    : None 有RF 或ems输出时，电量判断标准
**************************************************************************************/
void bat_check_in_normal_mode(uint16_t bat_V)
{
  if((bat_V > USER_M_LVL2_VAL - BAT_FLOAT_VAL)&&(bat_V < USER_M_LVL2_VAL + BAT_FLOAT_VAL))
  {
    bat_data.disp_level = Bat_Level_SP20; //10%~20%
  }
  else if((bat_V > USER_M_LVL1_VAL - BAT_FLOAT_VAL)&&(bat_V < USER_M_LVL1_VAL + BAT_FLOAT_VAL))
  {
    bat_data.disp_level = Bat_Level_SP10; //0%~10%
  }
	else if(bat_V < USER_M_LVL1_VAL)
	{
		bat_data.disp_level = Bat_Level_SP10; //0%~10%
	}
}
/*************************************************************************************
* FunctionName	 : bat_check_in_charge_mode()
* Description    :
* describe       :   ----3.76v --------------------------3.89v --------------------------4.05v---
*           --------|3.76v(+/-)range| ------------|3.89v(+/-)range| ---------------|4.05v(+/-)range|-------|charge_state_pin|
*              -----if(not ==lvl0) lvl1---lvl1--- if(not ==lvl1) lvl2 --lvl2 ---if(not ==lvl2) lvl3--lvl3-----lvl_full
*
* ReturnValue    : None  充电状态下 电池电量的判断标准
 **************************************************************************************/

void bat_check_in_charge_mode(uint16_t bat_V)
{
  if((bat_V > (CHARGE_M_LVL3_VAL - BAT_FLOAT_VAL))&&(bat_V < (CHARGE_M_LVL3_VAL + BAT_FLOAT_VAL)))
  {
    if(bat_data.disp_level != Bat_Level2) bat_data.disp_level = Bat_Level3;
  }
  else if((bat_V > (CHARGE_M_LVL2_VAL - BAT_FLOAT_VAL))&&(bat_V < (CHARGE_M_LVL2_VAL + BAT_FLOAT_VAL)))
  {
    if(bat_data.disp_level != Bat_Level1) bat_data.disp_level = Bat_Level2;
  }
  else if((bat_V > (CHARGE_M_LVL1_VAL - BAT_FLOAT_VAL))&&(bat_V < (CHARGE_M_LVL1_VAL + BAT_FLOAT_VAL)))
  {
    if(bat_data.disp_level != Bat_Level0) bat_data.disp_level = Bat_Level1;
  }
  else
  {
    if(bat_V > CHARGE_M_LVL3_VAL)
    {
      if((bat_data.disp_level != Bat_Level3) && (bat_data.disp_level != Bat_Level_Full))
      {
        bat_data.disp_level = Bat_Level3;
      }

    }
    else if((bat_V <= CHARGE_M_LVL3_VAL)&&(bat_V > CHARGE_M_LVL2_VAL))
    {
      bat_data.disp_level = Bat_Level2;
    }
    else if((bat_V <= CHARGE_M_LVL2_VAL)&&(bat_V > CHARGE_M_LVL1_VAL))
    {
      bat_data.disp_level = Bat_Level1;
    }
    else /// < 10
    {
      bat_data.disp_level = Bat_Level0;
    }
  }
}
/********************************************************************************
* FunctionName	 : Get_Batt_Value()
* Description    :
* EntryParameter :
* ReturnValue    :  电压从 AD值转化为实际值
*********************************************************************************/
static uint16_t Get_Batt_Value(uint16_t adc_data)
{
  uint16_t value;
  value = adc_data*5000/4096;		//res:100k+100k
  return value;
}

static uint16_t Get_Usb_Value(uint16_t adc_data)
{
  uint16_t value;
  value = adc_data*6000/4096;		//res:140k+100k
  return value;
}

/********************************************
 *充电状态检测 ：未充电，充电中，已充满
 *
**********************************************/
void bat_charge_state_check(void)
{
  static uint8_t charge_limit_delay = 0;

  if(0 == IS_DC5V_IN())
  {
    charge_limit_delay = 0;
    Sys_Info.Charge_State = No_CHARGE_STA;
    return;
  }

  if(bat_data.DC5v_adc_val < DC_4000mV_ADC_VAL) //usb电压超过 4.0v 判断为充电器接入
  {
    charge_limit_delay = 0;
    Sys_Info.Charge_State = No_CHARGE_STA;
    return;
  }

  if(charge_limit_delay >= 20)
  {
    charge_limit_delay = 20;
    CHARGE_LIMIT_ENABLE;
  }
  if(IS_CHAGE_FULL()) //||(bat_V > 4150))
  {
    bat_data.disp_level = Bat_Level_Full;
  }
  Sys_Info.Charge_State = CHARGING_STA;

}

/*************************************************************************************
* FunctionName	 : BatteryPowerJudgment()
* Description    :
* EntryParameter :
* ReturnValue    : None   100ms 调用一次
**************************************************************************************/
void BatteryPowerJudgment(_sys_state_e state)
{
  bat_data.adc_val = ADC_Data.bat_val;
  bat_data.DC5v_adc_val = ADC_Data.Usb_Val;

  bat_charge_state_check();
  if(Sys_Info.Charge_State == No_CHARGE_STA)
  {
    if(state == MACHINEOFF)
    {
			if(Sys_Info.Adc_First_Run_Flage)
      bat_check_in_sleep_mode(bat_data.adc_val);   //检查电池电量
    }
    else
    {
      bat_check_in_normal_mode(bat_data.adc_val);     //正常输出时
    }
  }
  else
  {
    bat_check_in_charge_mode(bat_data.adc_val); //充电时
  }
}

