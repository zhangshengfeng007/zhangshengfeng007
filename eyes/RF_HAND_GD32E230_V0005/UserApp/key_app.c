/***********************************************************
 * @file    key_app.c
 * @brief  按键应用程序
 * @version V1.0
 * @date    2023.02.02
 ***********************************************************/
#include "key_app.h"

static void KeyPower_CallBack(Key_Event Event);

/**************************  按键应用层start  **********************************/
Key_Type KeyPower; // 开机按键

/*
生产需求 ： 通过按键进入老化模式
           快速按下5次按键
*/
/********************  按键回调函数      **************************/
static void KeyPower_CallBack(Key_Event Event)
{
	Device.IdleCnt = 0;

	switch (Event)
	{
		case KEY_LONG:
		{
			switch(Device.State)
			{

				case SYS_OFF:
				case SYS_WAKE:
				{
					if (Power.BatLevel > BAT_LEVEL_LOW10)
					{
						Sys_state_Ctrl(SYS_ON);
						Motor_Start_Time(_LONG_TIME_);
					}
					else
					{
						Sys_state_Ctrl(SYS_WAKE);
					}
					break;
				}
				case SYS_CHARGE:
				{
					break;
				}

				case SYS_ERR:
				{
					MOTOR_ON;
					Delay_ms(_LONG_TIME_);
					POW_OFF;
					break;
				}
				case SYS_ON:
				case SYS_TEST:
				default:
				{
					MOTOR_ON;
					Delay_ms(_LONG_TIME_);

					Sys_state_Ctrl(SYS_OFF);
				}
			}
			break;

		}

		case KEY_PRESS:
		{
			switch (Device.State)
			{
				case SYS_OFF:
				{
					POW_ON;
					// printf("key_press \r\n");
					break;
				}
				case SYS_CHARGE:
				{
					if(0 == KeyPower.press_cnt)
					{
						KeyPower.count_press_delay = 500;
					}
					if(KeyPower.count_press_delay)
					{
						KeyPower.press_cnt ++;
						if(KeyPower.press_cnt == 5)
						{
							// printf("key_press_enter test mode \r\n");
							Test.mode_sta = Aging_Model_SAT;
							Test.State = Func_ENABLE;
							Device.State = SYS_TEST;
							LED_LASER_ON;
							Test.Aging_ems_rf_flag = 1;
							Test.Aging_ems_rf_cnt = 0;
							Test.Aging_charge_cnt = 0;
													// enter_test_mode
						}
					}
					break;
				}
				default:
				{
					break;
				}

			}



			break;
		}

		case KEY_PRESS_UP:
		{
			if ((Device.State == SYS_ON)||((Device.State == SYS_TEST) &&(Aging_Model_SAT != Test.mode_sta)))
			{
				if (Device.LevelDir == UP_DIR)
				{
					Device.Level ++;
					if(Device.Level > LEVEL3)
					{
						Device.LevelDir = DOWN_DIR;
						Device.Level = LEVEL2;
					}
				}
				else
				{
					Device.Level --;
					if(Device.Level <= LEVEL1)
					{
						Device.LevelDir = UP_DIR;
						Device.Level = LEVEL1;
					}
				}
				Device_Level_Set(Device.Level);
				Level_Leds_Control(LEVEL_LEDS_ON);

				if(0 == KeyPower.press_cnt)
				{
					KeyPower.count_press_delay = 500;
				}
				if(KeyPower.count_press_delay)
				{
					KeyPower.press_cnt ++;
					if(KeyPower.press_cnt == 5)
					{
						Sys_state_Ctrl(SYS_ADJUST_RELOAD);
						//Device.State = SYS_ADJUST_RELOAD;
												// enter_test_mode
					}
				}


			}
			if(Device.State == SYS_OFF)
			{
				printf("sys_wakeup \r\n");
				Sys_state_Ctrl(SYS_WAKE);
			}
			break;
		}

	case KEY_LONG_UP:
	case KEY_CONTINUOS_UP:
		break;
	default:
		break;
	}
}


/*
*************************************************************
* 功能说明: 按键应用层初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Key_App_Init(void)
{
	// 创建按键
	Key_Create(&KeyPower, Read_KeyPower_Level, 0, KeyPower_CallBack);
}

/**************************   按键应用层end  **********************************/
