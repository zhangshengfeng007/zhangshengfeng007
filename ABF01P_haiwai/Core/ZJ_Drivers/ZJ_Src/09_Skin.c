#include "includes.h"
#include "skin.h"

/*************************************************************************************
 * FunctionName	: Skin_Touch_Scan(void)
 * Description		: 皮肤接触检测 主要为射频接触检测
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void Skin_Touch_Scan(void) // 10ms运行一次
{
	static uint16_t no_touch_filter_count = 0;
#if ARF001
	;
#else
	if (RF_Handle.Flag || SysInfo.Restore_Flag)
#endif
	{
		if (SysInfo.Skin_Touch_RF_Flag)
		{
			no_touch_filter_count = 0;
			SysInfo.Skin_Touch_Flag = 1; // 系统接触标志位
		}
		else
		{
			no_touch_filter_count++;
			if (no_touch_filter_count > 500)
			{
				no_touch_filter_count = 500;
				SysInfo.Skin_Touch_Flag = 0;
			}
		}
	}
}

/*************************************************************************************
 * FunctionName	: Cosmetic_Time_Count(void)
 * Description		: 60S皮肤无接触、模式后60S内无切换 系统关机
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
uint8_t Cosmetic_Time_Count(void) // 10ms运行
{
	uint8_t TimeOut = 0;
	//	static uint8_t Montor_Flag=0;
	static uint8_t No_Touch_Cnt;

	if (SysInfo.Power_Value.state == System_ON)
	{
		if (SysInfo.Skin_Touch_Flag) // 计时开始
		{
			if (!SysInfo.Montor_Flag && SysInfo.Skin_Touch_Montor_Flag)
			{
				No_Touch_Cnt++;
			}
			else
			{
				No_Touch_Cnt = 0;
			}

			if ((SysInfo.Skin_No_Touch_Timer || No_Touch_Cnt > 100) && SysInfo.Skin_Touch_Montor_Flag)
			{
				//				if(!SysInfo.Montor_Flag &&SysInfo.Skin_Touch_Montor_Flag ) //开机第一次接触到皮肤
				//				{
				SysInfo.Skin_Touch_Montor_Flag = 0;
				SysInfo.Montor_Flag = 1; // 电机振动标志
				SysInfo.StayTime = 20;
				No_Touch_Cnt = 0;

				//				}
			}

			SysInfo.Skin_No_Touch_Timer = 0;
			TimeOut = 0;
			//			Montor_Flag =0;
		}
		else
		{
			SysInfo.Skin_No_Touch_Timer++;
			//			Montor_Flag =1;
			if (SysInfo.Skin_No_Touch_Timer > 6000) // 6000
			{
				SysInfo.Skin_No_Touch_Timer = 0;
				TimeOut = 1; // 进程结束
			}
		}
	}
	else
	{
		//		Montor_Flag =0;
		;
	}
	return TimeOut;
}
/*************************************************************************************
 * FunctionName	: skin_rf_scan(void)
 * Description		: RF皮肤检测
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
uint8_t Skin_RF_Scan(void) // 1ms运行1次
{
	static uint16_t Skin_RF_Value = 0;
	static uint8_t RF_Count = 0, ShinCount = 0;
	static uint8_t RF_Over_Count = 0, SystemON_2s_Cnt;
	static uint16_t Skin_Touch_Data[BAT_ADC_COUNT];

	if (++ShinCount <= BAT_ADC_COUNT)
	{
#if ARF001
		if (!SysInfo.Montor_Flag)
#else
		if ((RF_Handle.ADC_Flag && !SysInfo.Montor_Flag) || SysInfo.Restore_Flag)
#endif
		{
			Skin_Touch_Data[ShinCount - 1] = filter((uint16_t *)&ADC_Value, 3, ADC_CHANNEL_NUM, ADC_ADD_COUNT);
		}
		else
		{
			ShinCount--;
		}
		return 0;
	}
	else
	{
		ShinCount = 0;
		Skin_RF_Value = Get_Battery_Read((uint16_t *)&Skin_Touch_Data, BAT_ADC_COUNT);
		HAL_ADCEx_Calibration_Start(&hadc1);
		//		Skin_RF_Value = Skin_filter((uint16_t *)&Skin_Touch_Data,BAT_ADC_COUNT);
	}

	if (SysInfo.Power_Value.state != System_OFF || SysInfo.Test_Mode.Test_Mode_Flag != OFF) // 开机2s后检测皮肤
	{
		SystemON_2s_Cnt++;
	}
	else
	{
		SystemON_2s_Cnt = 0;
	}
	if (SystemON_2s_Cnt >= 15)
	{
		SystemON_2s_Cnt = 15;
		//			#if ARF001
		//				;
		//			#else
		if (RF_Handle.Flag || SysInfo.Restore_Flag) // RF开启后，判断是否接触皮肤
		//			#endif
		{
			//			Skin_RF_Value = filter((uint16_t *)&ADC_Value,3,ADC_CHANNEL_NUM,ADC_ADD_COUNT);

			if (Skin_RF_Value >= RF_TOUCH_THRESHOLD) // 接触皮肤
			{
				if (++RF_Over_Count >= 2)
				{
					SysInfo.Skin_Touch_RF_Flag = 1; // RF射频接触标志位
					RF_Over_Count = 0;
				}
				RF_Count = 0;
			}
			else
			{
				if (++RF_Count >= Skin_Remove_Cnt) // 离开皮肤
				{
					SysInfo.Skin_Touch_RF_Flag = 0;
					RF_Count = 0;
					RF_Over_Count = 0;
				}
			}
		}
	}
	return 1;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
