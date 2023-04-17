/***********************************************************
* @file    drv_power.c
* @brief  电源管理程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_power.h"
#include "drv_adc.h"
#include "drv_gpio.h"
#include "math.h"

#include "device_app.h"


extern Device_Type Device;
extern Motor_Type Motor;
Power_Type Power;

/*
*************************************************************
* 功能说明: 电源管理初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Power_Init(void)
{
	Power.BatTemp = 250;
	Power.BatLevel = BAT_LEVEL_PRE_70;
	Power.CharheState = CHARGE_STOP;

	CHARGE_EN_OFF;
}

/*
*************************************************************
* 功能说明: 电池充电开关
* 形    参: cmd 开关
* 返 回 值: 无
*************************************************************
*/
void Power_Charge_Control(Charge_Type cmd)
{
	Power.CharheState = cmd;

	if(cmd == CHARGE_START){
		CHARGE_EN_ON;
	}else{
		CHARGE_EN_OFF;
	}
}

/*
*************************************************************
* 功能说明: BAT 温度读取,需要周期调用
* 形    参: ntc 需要更新的温度变量
* 返 回 值: 无
*************************************************************
*/
void Bat_Temp_Read(void)
{
	static u16 arr[10] = {0};
	static u8 count = 0;
	u16 data = 0;
	u8 i = 0;
	u8 j = 0;

	if(count < 10)
	{
		arr[count] = adc_value[1];
		count++;
		return;
	}

	count = 0;

	//冒泡排序取中值
	for(i = 0; i < 10-1; i++){
		for(j = 0; j < 10-i-1; j++){
			if(arr[j] > arr[j+1]){
				data = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = data;
			}
		}
	}

	if(arr[4] < 3800)
	{
		//计算电阻
		arr[4] =  arr[4]*10000/(4095-arr[4]);

		//计算温度,1位小数点
		Power.BatTemp = ((1/( log(arr[4]/10000.0)/3950.0 + 0.003354)) - 273.15)*10;
	}
	else
	{
		Power.BatTemp = 880;
	}
}





/*
*************************************************************
* 功能说明: 电池电量检测,需要周期调用 3.2v附近电压测试
* 形    参: 无
* 返 回 值: 无
*
*   1ms 调用一次

		  GND
*************************************************************/

static void bat_val_3v2_check(u32 bat_val)
{
	static u8 High_lvl_change_delay = 0;
	static u8 LOW_lvl_change_delay = 0;

	if(Power.Last_Bat_Level == BAT_LEVEL_LOW10)
	{
		if(bat_val <= BAT_3V2_ADC_VAL - BAT_0V_05_ADC_VAL)
		{   
			High_lvl_change_delay ++;
			if(High_lvl_change_delay > 100)
			{
				High_lvl_change_delay = 0;
				Power.BatLevel = BAT_LEVEL_LOW5;
			}
		}
		else
		{
			High_lvl_change_delay = 0;
		}
		LOW_lvl_change_delay = 0;

	}
	else if(Power.BatLevel = BAT_LEVEL_EMETY)
	{
		High_lvl_change_delay = 0;
		if(bat_val > BAT_3V0_ADC_VAL + BAT_0V_05_ADC_VAL)
		{
			LOW_lvl_change_delay ++;
			if(LOW_lvl_change_delay > 100)
			{
				LOW_lvl_change_delay = 0;
				Power.BatLevel = BAT_LEVEL_LOW5;
			}
		}
		else
		{
			LOW_lvl_change_delay = 0;
		}
	}
	else
	{
		Power.BatLevel = BAT_LEVEL_LOW5;
	}
}

/*
*************************************************************
* 功能说明: 电池电量检测,需要周期调用 3.2v附近电压测试
* 形    参: 无
* 返 回 值: 无
*
*   1ms 调用一次

		  GND
*************************************************************/

static void bat_val_3v3_check(u32 bat_val)
{
	static u8 High_lvl_change_delay = 0;
	static u8 LOW_lvl_change_delay = 0;
	if(Power.Last_Bat_Level == BAT_LEVEL_LOW20)
	{
		if(bat_val <= BAT_3V3_ADC_VAL - BAT_0V_05_ADC_VAL)
		{   
			High_lvl_change_delay ++;
			if(High_lvl_change_delay > 100)
			{
				High_lvl_change_delay = 0;
				Power.BatLevel = BAT_LEVEL_LOW10;
			}
		}
		else
		{
			High_lvl_change_delay = 0;
		}
		LOW_lvl_change_delay = 0;
	}
	else if(Power.Last_Bat_Level == BAT_LEVEL_LOW5)
	{
		High_lvl_change_delay = 0;
		if(bat_val > BAT_3V2_ADC_VAL + BAT_0V_05_ADC_VAL)
		{
			LOW_lvl_change_delay ++;
			if(LOW_lvl_change_delay > 100)
			{
				LOW_lvl_change_delay = 0;
				Power.BatLevel = BAT_LEVEL_LOW10;
			}
		}
		else
		{
			LOW_lvl_change_delay = 0;
		}
	}
	else
	{
		Power.BatLevel = BAT_LEVEL_LOW10;
		LOW_lvl_change_delay = 0;
		High_lvl_change_delay = 0;
	}
}
/*************************************************************
* 功能说明: 电池电量检测,需要周期调用 3.2v附近电压测试
* 形    参: 无
* 返 回 值: 无
*
*   1ms 调用一次

		  GND
*************************************************************/

static void bat_val_3v4_check(u32 bat_val)
{
	static u8 High_lvl_change_delay = 0;
	static u8 LOW_lvl_change_delay = 0;

	if(Power.Last_Bat_Level == BAT_LEVEL_PRE_70)
	{
		if(bat_val <= BAT_3V4_ADC_VAL - BAT_0V_05_ADC_VAL)
		{   
			High_lvl_change_delay ++;
			if(High_lvl_change_delay > 100)
			{
				High_lvl_change_delay = 0;
				Power.BatLevel = BAT_LEVEL_LOW20;
			}
		}
		else
		{
			High_lvl_change_delay = 0;
		}
		LOW_lvl_change_delay = 0;
	}
	else if(Power.Last_Bat_Level == BAT_LEVEL_LOW10)
	{
		High_lvl_change_delay = 0;
		if(bat_val > BAT_3V3_ADC_VAL + BAT_0V_05_ADC_VAL)
		{
			LOW_lvl_change_delay ++;
			if(LOW_lvl_change_delay > 100)
			{
				LOW_lvl_change_delay = 0;
				Power.BatLevel = BAT_LEVEL_LOW20;
			}
		}
		else
		{
			LOW_lvl_change_delay = 0;
		}
	}
	 else
	 {
		Power.BatLevel = BAT_LEVEL_LOW20;
		LOW_lvl_change_delay = 0;
		High_lvl_change_delay = 0;
	 }
}
/*************************************************************
* 功能说明: 电池电量检测,需要周期调用 3.2v附近电压测试
* 形    参: 无
* 返 回 值: 无
*
*   1ms 调用一次

		  GND
*************************************************************/

static void bat_val_3v9_check(u32 bat_val)
{
	static u8 High_lvl_change_delay = 0;
	static u8 LOW_lvl_change_delay = 0;

	if(Power.Last_Bat_Level == BAT_LEVEL_MID)
	{
		if(bat_val <= BAT_3V9_ADC_VAL - BAT_0V_05_ADC_VAL)
		{   
			High_lvl_change_delay ++;
			if(High_lvl_change_delay > 100)
			{
				High_lvl_change_delay = 0;
				Power.BatLevel = BAT_LEVEL_PRE_70;
			}
		}
		else
		{
			High_lvl_change_delay = 0;
		}
		LOW_lvl_change_delay = 0;


	}
	else if(Power.Last_Bat_Level == BAT_LEVEL_LOW20)
	{
		High_lvl_change_delay = 0;
		if(bat_val > BAT_3V4_ADC_VAL + BAT_0V_10_ADC_VAL)
		{
			LOW_lvl_change_delay ++;
			if(LOW_lvl_change_delay > 100)
			{
				LOW_lvl_change_delay = 0;
				Power.BatLevel = BAT_LEVEL_PRE_70;
			}
		}
		else
		{
			LOW_lvl_change_delay = 0;
		}
	}
	else
	{
		Power.BatLevel = BAT_LEVEL_PRE_70;
		LOW_lvl_change_delay = 0;
		High_lvl_change_delay = 0;
	}
}

/*************************************************************
* 功能说明: 电池电量检测,需要周期调用 3.2v附近电压测试
* 形    参: 无
* 返 回 值: 无
*
*   1ms 调用一次

		  GND
*************************************************************/

static void bat_val_4v1_check(u32 bat_val)
{

	if(Power.Last_Bat_Level == BAT_LEVEL_FULL)
	{
		if(bat_val <= BAT_4V1_ADC_VAL - BAT_0V_05_ADC_VAL)
		{
			Power.BatLevel = BAT_LEVEL_MID;
		}
	}
	else	//if(Power.Last_Bat_Level == BAT_LEVEL_PRE_70)
	{
		if(bat_val > BAT_3V9_ADC_VAL + BAT_0V_05_ADC_VAL)
		{
			Power.BatLevel = BAT_LEVEL_MID;
		}
		else
		{
			Power.BatLevel = BAT_LEVEL_PRE_70;
		}

	}
	// else
	// {
	// 	Power.BatLevel = BAT_LEVEL_MID;
	// }

}

/*
*************************************************************
* 功能说明: 电池电量检测,需要周期调用
* 形    参: 无
* 返 回 值: 无
*
*  // 电池分压等级，
*	1. wake_up情况下，不做偏差
*   2. 充电时，浮高 0.15v
*   3. 放电时，拉低 0.15v
*
*   1ms 调用一次
*   vbat-----
		  |
	     100k
	      |-------adc_read   // 充电时，ADC检测点会浮高0.11v，实际电压约上浮0.15v
		 200k
		  |
		  GND
*************************************************************
*/
void Bat_Level_Detection(void)
{
	static u8 count = 0;
	static u32 sum = 0;
	static u8 check_delay = 0;   //按键按下,前50ms不检测


	check_delay ++;
	if(check_delay < 50)
	{
		return;
	}
	if(Motor.State == Func_ENABLE)
	{
		return;
	}

	check_delay = 51;

	if(count < 50){
		sum += adc_value[2];
		count++;
		return;
	}else{
		sum /= 50;

		Power.adc_val = sum;

		switch(Device.State)
		{
			case SYS_CHARGE:
			{
				if(Power.adc_val < BAT_4V1_ADC_VAL)
				{
					Power.adc_val -= BAT_CHARGE_0V_15_ADC_VAL;
				}
				break;
			}
			case SYS_TEST:
			{
				if(Test.Aging_charge_flag)
				{
					if(Power.adc_val < BAT_4V1_ADC_VAL)
					{
						Power.adc_val -= BAT_CHARGE_0V_15_ADC_VAL;
					}
				}
				break;
			}
			default:
			{
				break;
			}

		}

		if(Power.adc_val  <= BAT_3V0_ADC_VAL)
		{
			Power.BatLevel = BAT_LEVEL_EMETY;    // 异常电压
		}
		else if(Power.adc_val  <= BAT_3V2_ADC_VAL)        //
		{
			bat_val_3v2_check(Power.adc_val );
		}
		else if(Power.adc_val  <= BAT_3V3_ADC_VAL)        //
		{
			bat_val_3v3_check(Power.adc_val );
		}

		else if(Power.adc_val  <= BAT_3V4_ADC_VAL)
		{
			bat_val_3v4_check(Power.adc_val);
		}
		else if(Power.adc_val  <= BAT_3V9_ADC_VAL)
		{
			bat_val_3v9_check(Power.adc_val );
		}
		else if(Power.adc_val  <= BAT_4V1_ADC_VAL)
		{
			bat_val_4v1_check(Power.adc_val);
		}
		else
		{
			Power.BatLevel = BAT_LEVEL_FULL;
		}

		Power.Last_Bat_Level = Power.BatLevel;
		//Power.BatLevel = BAT_LEVEL_LOW20; //DEBUG
		count = 0;
		sum = 0;
	}
}

