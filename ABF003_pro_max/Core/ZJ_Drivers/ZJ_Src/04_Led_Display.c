
#include "includes.h"

_LED_VALUE_TypeDef Led_Value;
/**************************************************************************************
 * FunctionName   : led_scan(uint8_t bit_flag)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void led_scan(uint8_t bit_flag)
{
	if (bit_flag & WLED1_ON_BIT)
	{
		GEAR_LED1_UP();
	}
	else
	{
		GEAR_LED1_DOWN();
	}

	if (bit_flag & WLED2_ON_BIT)
	{
		GEAR_LED2_UP();
	}
	else
	{
		GEAR_LED2_DOWN();
	}

	if (bit_flag & WLED3_ON_BIT)
	{
		GEAR_LED3_UP();
	}
	else
	{
		GEAR_LED3_DOWN();
	}

	if (bit_flag & WLED4_ON_BIT)
	{
		GEAR_LED4_UP();
	}
	else
	{
		GEAR_LED4_DOWN();
	}

	if (bit_flag & WLED5_ON_BIT)
	{
		GEAR_LED5_UP();
	}
	else
	{
		GEAR_LED5_DOWN();
	}
}
/**************************************************************************************
 * FunctionName   : Twink_Display(uint8_t BitFlag,uint16_t stayTime)
 * Description    : 充电电量指示灯闪烁
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void Twink_Display(uint8_t BitFlag, uint16_t stayTime)
{
	static uint8_t BLNstate;
	static uint16_t BLNcnt;

	if (BLNstate)
	{
		BLNcnt--;
		if (BLNcnt == 0)
		{
			BLNstate = 0;
			BLNcnt = 0;
		}
	}
	else
	{
		BLNcnt++;
		if (BLNcnt == stayTime || BLNcnt > stayTime)
		{
			BLNstate = 1;
			BLNcnt = stayTime;
		}
	}

	if (BLNstate)
	{
		led_scan(BitFlag);
	}
	else
	{
		led_scan((BitFlag >> 1));
	}
}
/**************************************************************************************
 * FunctionName   : Twink1hz_Display(uint8_t BitFlag,uint16_t stayTime)
 * Description    : 低电档位灯呼吸
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void Twink1hz_Display(uint8_t BitFlag, uint16_t stayTime)
{
	static uint8_t Twink1hzState;
	static uint16_t Twink1hzcnt;

	if (Twink1hzState)
	{
		Twink1hzcnt--;
		if (Twink1hzcnt == 0)
		{
			Twink1hzState = 0;
			Twink1hzcnt = 0;
		}
	}
	else
	{
		Twink1hzcnt++;
		if (Twink1hzcnt == stayTime || Twink1hzcnt > stayTime)
		{
			Twink1hzState = 1;
			Twink1hzcnt = stayTime;
		}
	}

	if (Twink1hzState)
	{
		led_scan(BitFlag);
	}
	else
	{
		led_scan(0);
	}
}
/**************************************************************************************
 * FunctionName   : Mode_Twink1hz_Display(uint8_t BitFlag,uint16_t stayTime)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void Mode_Twink1hz_Display(uint16_t stayTime)
{
	static uint8_t Mode_Twink1hzState;
	static uint16_t Mode_Twink1hzcnt;

	if (Mode_Twink1hzState)
	{
		Mode_Twink1hzcnt--;
		if (Mode_Twink1hzcnt == 0)
		{
			Mode_Twink1hzState = 0;
			Mode_Twink1hzcnt = 0;
		}
	}
	else
	{
		Mode_Twink1hzcnt++;
		if (Mode_Twink1hzcnt == stayTime || Mode_Twink1hzcnt > stayTime)
		{
			Mode_Twink1hzState = 1;
			Mode_Twink1hzcnt = stayTime;
		}
	}

	if (Mode_Twink1hzState)
	{
		if (SysInfo.WorkState == repair_mode)
		{
			Repair_LED_ON();
		}
		else
		{
			Upkeep_LED_ON();
		}

		//				if(SysInfo.Check_Protect_Flage && !SysInfo.OverTemp_Flag) //??????????
		if (SysInfo.Check_Protect_Flage) // 没有检测到位移
		{
			IRled_start();
		}
	}
	else
	{
		if (SysInfo.WorkState == repair_mode)
		{
			Repair_LED_OFF();
		}
		else
		{
			Upkeep_LED_OFF();
		}
		if (SysInfo.Check_Protect_Flage) // 没有检测到位移
		{
			IRled_stop();
		}
	}
}
/**************************************************************************************
 * FunctionName   : LedStay_Display(uint8_t BitFlag,uint16_t stayTime)
 * Description    : 定时熄灭指示灯
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void LedStay_Display(uint8_t BitFlag, uint16_t stayTime)
{
	static uint16_t TimeCnt2, TargeTime;

	if (++TimeCnt2 > TargeTime)
	{
		TimeCnt2 = TargeTime;
		led_scan(0x00);
	}
	else
	{
		TargeTime = stayTime;
		//		TimeCnt=1;
		led_scan(BitFlag);
	}
}
/**************************************************************************************
 * FunctionName   : LED_Flicker(uint16_t stayTime)
 * Description    : 电量查询、缺电1档档位灯闪烁5次
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t LED_Flicker(uint16_t stayTime)
{
	static uint8_t state = 0;
	static uint16_t Time_Cnt;
	static uint16_t Time_Period;

	Time_Period = stayTime << 1;
	Time_Cnt++;
	if (Time_Cnt < stayTime)
	{
		led_scan(0x01);
		state = 0;
	}
	else
	{
		led_scan(0x00);
		if (Time_Cnt == Time_Period)
		{
			Time_Cnt = 0;
			state = 1;
		}
	}
	return state;
}
/**************************************************************************************
 * FunctionName   : Led_Display(_LED_VALUE_TypeDef * LED)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t Led_Display(_LED_VALUE_TypeDef *LED)
{
	static uint16_t TimerCnt;
	static uint8_t Led_Lock_Flag, Flicker_Cnt = 5;

	if (Led_Lock_Flag)
	{
		if (++TimerCnt > (LED->StayTime))
		{
			LED->Level = 0X5A;
			Led_Lock_Flag = 0;
		}
	}
	else
	{
		TimerCnt = 0;
	}

	if ((LED->Mode) == Batt_Charging) // 充电模式
	{
		switch (LED->Level)
		{
		case 0x00:
			Twink_Display(0x01, 50);
			break; // BAT_00_00_STATUS
		case 0x01:
			Twink_Display(0x01, 50);
			break; // BAT_00_20_STATUS
		case 0x02:
			Twink_Display(0x03, 50);
			break; // BAT_20_40_STATUS
		case 0x03:
			Twink_Display(0x07, 50);
			break; // BAT_40_60_STATUS
		case 0x04:
			Twink_Display(0x0f, 50);
			break; // BAT_60_80_STATUS
		case 0x05:
			Twink_Display(0x1f, 50);
			break; // BAT_80_100_STATUS
		case 0x06:
			led_scan(0x1f);
			//				Led_Lock_Flag = 1 ;
			break; // BAT_100_100_STATUS
		default:
			led_scan(0x00);
			break; // 不显示
		}
	}
	else if ((LED->Mode) == Batt_Low_20) // 低电LED灯保持相应档位的1Hz频率持续闪烁
	{
		switch (LED->Level)
		{
		case 0x00:
			led_scan(0x00);
			break; // BAT_00_00_STATUS
		case 0x01:
			Twink1hz_Display(0x01, 50);
			break; // 1档
		case 0x02:
			Twink1hz_Display(0x03, 50);
			break; // 2档
		case 0x03:
			Twink1hz_Display(0x07, 50);
			break; // 3档
		case 0x04:
			Twink1hz_Display(0x0f, 50);
			break; // 4档
		case 0x05:
			Twink1hz_Display(0x1f, 50);
			break; // 5档
		default:
			led_scan(0x00);
			break; // 不显示
		}
		Mode_Twink1hz_Display(50);
	}
	else if ((LED->Mode) == Batt_Low_0) // 缺电，1档LED灯1Hz频率闪烁5次
	{
		if ((LED->Counts) != Flicker_Cnt)
		{
			if (LED_Flicker(50))
			{
				LED->Counts++;
			}
		}
		//		else
		//		{
		//			SysInfo.Power_Value.state = System_OFF ;
		//		}
	}
	else // 非充电模式
	{
		switch (LED->Level)
		{
		case 0x00:
			led_scan(0x01);
			break; // 1档
		case 0x01:
			led_scan(0x01);
			break; // 1档
		case 0x02:
			led_scan(0x03);
			break; // 2档
		case 0x03:
			led_scan(0x07);
			break; // 3档
		case 0x04:
			led_scan(0x0f);
			break; // 4档
		case 0x05:
			led_scan(0x1f);
			break; // 5档
		default:
			led_scan(0x00);
			break; // 不显示
		}
		if (LED->state)
		{
			Led_Lock_Flag = 1;
			LED->state = 0;
		}
	}
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
