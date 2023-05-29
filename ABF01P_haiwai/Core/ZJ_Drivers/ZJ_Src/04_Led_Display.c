
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
 * Description    : ������ָʾ����˸
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
 * Description    : �͵絵λ�ƺ���
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

			if (SysInfo.WorkState == repair_mode)
			{
				Repair_LED_ON();
			}
			else
			{
				Upkeep_LED_ON();
			}
			IRled_start();
		}
	}
	else
	{
		Mode_Twink1hzcnt++;
		if (Mode_Twink1hzcnt >= stayTime)
		{
			Mode_Twink1hzState = 1;
			Mode_Twink1hzcnt = stayTime;

			if (SysInfo.WorkState == repair_mode)
			{
				Repair_LED_OFF();
			}
			else
			{
				Upkeep_LED_OFF();
			}

			if (SysInfo.OverTemp_Flag == 0x02) // �???????
			{
				IRled_stop();
			}
		}
	}
}
/**************************************************************************************
 * FunctionName   : MODE_LEVEL_1hz_Display(uint8_t BitFlag,uint16_t stayTime)
 * Description    :
 * EntryParameter :      --解决 档位灯和 状态灯闪烁不同步问题
 * ReturnValue    : None
 **************************************************************************************/
void MODE_LEVEL_1hz_Display(uint8_t BitFlag, uint16_t stayTime)
{
	static uint8_t Twink1hzState;
	static uint16_t Twink1hzcnt;

	if (Twink1hzState)
	{
		if(Twink1hzcnt)
		{
			Twink1hzcnt--;
			return;
		}

		Twink1hzState = 0;
		Twink1hzcnt = 0;
		led_scan(0);
		if (SysInfo.WorkState == repair_mode)
		{
			Repair_LED_OFF();
		}
		else
		{
			Upkeep_LED_OFF();
		}
		if (SysInfo.OverTemp_Flag == 0x02) // �???????
		{
			IRled_stop();
		}

	}
	else
	{
		Twink1hzcnt++;
		if (Twink1hzcnt >= stayTime)
		{
			Twink1hzState = 1;
			Twink1hzcnt = stayTime;
			led_scan(BitFlag);
			if (SysInfo.WorkState == repair_mode)
			{
				Repair_LED_ON();
			}
			else
			{
				Upkeep_LED_ON();
			}
			IRled_start();
		}
	}
}

/**************************************************************************************
 * FunctionName   : LedStay_Display(uint8_t BitFlag,uint16_t stayTime)
 * Description    : ��ʱϨ��ָʾ��
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
 * Description    : ������ѯ��ȱ��1����λ����˸5��
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
static void charge_led_disp(uint8_t level)
{
	switch (level)
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
			break; // ?????
	}
}

/**************************************************************************************
 * FunctionName   : normal_mode _irled display
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
static void mode_led_normal_display(void)
{
	switch(SysInfo.WorkState)
	{
		case upkeep_mode:
		{
			if (SysInfo.Power_Value.state == System_ON)
			{
				Upkeep_LED_ON();
				IRled_start();
			}
			else
			{
				if (SysInfo.Test_Mode.Test_Mode_Flag != ON)
				{
					Upkeep_LED_OFF();
				}
			}
			break;
		}
		case repair_mode:
		{
			if (SysInfo.Power_Value.state == System_ON)
			{
				Repair_LED_ON();
				IRled_start();
			}
			else
			{
				if (SysInfo.Test_Mode.Test_Mode_Flag != ON)
				{
					Repair_LED_OFF();
				}
			}
			break;
		}
		default:
		{
			Upkeep_LED_OFF();
			Repair_LED_OFF();
			break;
		}
	}
}

/**************************************************************************************
 * FunctionName   : ??????20%?,led????
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
static void bat_lower_percent_20_disp(uint8_t level)
{
	switch (level)
	{
		case 0x00:
		case 0x01:
		{
			if(((SysInfo.Check_Protect_Flage)&&(SysInfo.Reminder_Cnt))||(SysInfo.OverTemp_Flag == 0x02))
			{
				MODE_LEVEL_1hz_Display(0x01, 50);
			}
			else
			{
				Twink1hz_Display(0x01, 50);
				mode_led_normal_display();
			}
			break; // 1??
		}

		case 0x02:
		{
			//Twink1hz_Display(0x03, 50);
			if(((SysInfo.Check_Protect_Flage)&&(SysInfo.Reminder_Cnt))||(SysInfo.OverTemp_Flag == 0x02))
			{
				MODE_LEVEL_1hz_Display(0x03, 50);
			}
			else
			{
				Twink1hz_Display(0x03, 50);
				mode_led_normal_display();
			}
			break; // 2??
		}

		case 0x03:
		{
			//Twink1hz_Display(0x07, 50);
			if(((SysInfo.Check_Protect_Flage)&&(SysInfo.Reminder_Cnt))||(SysInfo.OverTemp_Flag == 0x02))
			{
				MODE_LEVEL_1hz_Display(0x07, 50);
			}
			else
			{
				Twink1hz_Display(0x07, 50);
				mode_led_normal_display();
			}
			break; // 3??

		}

		case 0x04:
		{
			//Twink1hz_Display(0x0f, 50);
			if(((SysInfo.Check_Protect_Flage)&&(SysInfo.Reminder_Cnt))||(SysInfo.OverTemp_Flag == 0x02))
			{
				MODE_LEVEL_1hz_Display(0x07, 50);
			}
			else
			{
				Twink1hz_Display(0x07, 50);
				mode_led_normal_display();
			}
			break; // 4??
		}
		case 0x05:
		{
			//	Twink1hz_Display(0x1f, 50);
			if(((SysInfo.Check_Protect_Flage)&&(SysInfo.Reminder_Cnt))||(SysInfo.OverTemp_Flag == 0x02))
			{
				MODE_LEVEL_1hz_Display(0x1f, 50);
			}
			else
			{
				Twink1hz_Display(0x1f, 50);
				mode_led_normal_display();
			}
			break; // 5??

		}

		default:
		{
			led_scan(0x00);
			mode_led_normal_display();
			break; // ?????
		}

	}
	//Mode_Twink1hz_Display(50);

}
/**************************************************************************************
 * FunctionName   : ???????,led????
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
static void bat_lower_percent_00_disp(uint8_t level)
{



}

/**************************************************************************************
 * FunctionName   : ?????,?????,led????
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
static void work_level_display(uint8_t level)
{
	switch (level)
	{
		case 0x00:
		case 0x01:
		{
			led_scan(0x01);
			break; // 1??
		}
		case 0x02:
		{
			led_scan(0x03);
			break; // 2??
		}
		case 0x03:
		{
			led_scan(0x07);
			break; // 3??
		}
		case 0x04:
		{
			led_scan(0x0f);
			break; // 4??
		}

		case 0x05:
		{
			led_scan(0x1f);
			break; // 5??
		}
		default:
		{
			led_scan(0x00);
			break; // ?????
		}

	}
}


/**************************************************************************************
 * FunctionName   : Led_Display(_LED_VALUE_TypeDef * LED)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t Led_Display(_LED_VALUE_TypeDef *LED)
{
	static uint16_t TimerCnt = 0;
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

	if((System_OFF == SysInfo.Power_Value.state)&&(0 == LED->StayTime))
	{
		led_scan(0);
		IRled_stop();
		Repair_LED_OFF();
		Upkeep_LED_OFF();
		TimerCnt = 0;
		return 0;
	}

//-------------------------
switch(LED->Mode)
{
	case Batt_Charging:
	{
		charge_led_disp(LED->Level);
		break;
	}
	case Batt_Low_20:
	{
		bat_lower_percent_20_disp(LED->Level);
		break;
	}
	case Batt_Low_0:
	{
		if ((LED->Counts) != Flicker_Cnt)
		{
			if (LED_Flicker(50))
			{
				LED->Counts++;
			}
		}
		break;
	}
	default:
	{
		work_level_display(LED->Level);
		if (LED->state)
		{
			Led_Lock_Flag = 1;
			LED->state = 0;
		}


		if(((SysInfo.Check_Protect_Flage) && (SysInfo.Reminder_Cnt))|| (SysInfo.OverTemp_Flag == 0x02)) // 20230512
		{
			if ((SysInfo.Power_Value.state == System_ON))  // 接触到负载无gsensor就闪烁提示
			{
				Mode_Twink1hz_Display(50);
			}
		}
		else
		{
			switch(SysInfo.WorkState)
			{
				case upkeep_mode:
				{
					if (SysInfo.Power_Value.state == System_ON)
					{
						Upkeep_LED_ON();
						IRled_start();
					}
					else
					{
						if (SysInfo.Test_Mode.Test_Mode_Flag != ON)
						{
							Upkeep_LED_OFF();
						}
					}
					break;
				}
				case repair_mode:
				{
					if (SysInfo.Power_Value.state == System_ON)
					{
						Repair_LED_ON();
						IRled_start();
					}
					else
					{
						if (SysInfo.Test_Mode.Test_Mode_Flag != ON)
						{
							Repair_LED_OFF();
						}
					}
					break;
				}
				default:
				{
					Upkeep_LED_OFF();
					Repair_LED_OFF();
					break;
				}
			}

		}
		break;
	}
}

	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
