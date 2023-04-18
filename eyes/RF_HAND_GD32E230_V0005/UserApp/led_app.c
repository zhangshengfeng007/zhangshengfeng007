/***********************************************************
 * @file    sys_app.c
 * @brief  主应用程序
 * @version V1.0
 * @date    2022.11.21
 ***********************************************************/
#include "led_app.h"

#include "device_app.h"
#include "app_include.h"

LED_DISP_t disp_data;

/*
*************************************************************
* 功能说明: LED强制刷新一次
* 形    参: cmd 开关
* 返 回 值: 无
*************************************************************
*/
void Level_Leds_Force_refresh(void)
{
	switch(Device.Level)
	{
		case LEVEL2:
		{
			LED_1D_ON;
			LED_2D_ON;
			LED_3D_OFF;
			break;
		}
		case LEVEL3:
		{
			LED_1D_ON;
			LED_2D_ON;
			LED_3D_ON;
			break;
		}
		case LEVEL1:
		default:
		{
			LED_1D_ON;
			LED_2D_OFF;
			LED_3D_OFF;
			break;
		}
	}
}

/*
*************************************************************
* 功能说明: 档位灯开关
* 形    参: cmd 开关
* 返 回 值: 无
*************************************************************
*/
void Level_Leds_Control(LEVEL_LEDS_e cmd)
{
	static Sys_level_e tmp_level = LEVEL0;
	switch (cmd)
	{
		case LEVEL_LEDS_ON:
		{
			if(Device.Level == tmp_level)
			{
				break;
			}
			Level_Leds_Force_refresh();
			tmp_level = Device.Level;
			break;
		}
		default:
		{
			if(LEVEL0 == tmp_level)
			{
				break;
			}
			tmp_level = LEVEL0;
			LED_1D_OFF;
			LED_2D_OFF;
			LED_3D_OFF;
			break;
		}
	}
}


/*
*************************************************************
* 功能说明:  系统正常工作时，电源指示灯处理,需要周期调用
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
static void sys_on_bat_leds_disp(void)
{

	switch(Power.BatLevel)
	{
		case BAT_LEVEL_FULL:
		case BAT_LEVEL_MID:
		case BAT_LEVEL_PRE_70:
		{
			// 	// 白灯常亮
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
			break;
		}

		case BAT_LEVEL_LOW20:
		{
			// 红灯呼吸
			Bat_Led_Low_Breathe();
			Bat_Led_Full_Control(DISABLE);
			break;
		}

		case BAT_LEVEL_LOW10:
		case BAT_LEVEL_LOW5:
		{
			// 红灯常亮
			Bat_Led_Low_Control(ENABLE);
			Bat_Led_Full_Control(DISABLE);
			break;
		}

		default:
		{
			break;
		}
	}
}

/*
*************************************************************
* 功能说明:  系统被短按唤醒时，电源指示灯处理,需要周期调用
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
static void sys_wakeup_bat_leds_disp(void)
{
	static u8 i = 0, breath_flag = 0;

	switch(Power.BatLevel)
	{
		case BAT_LEVEL_FULL:
		case BAT_LEVEL_MID:
		case BAT_LEVEL_PRE_70:
		{
			// 白灯常亮
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
			break;
		}

		case BAT_LEVEL_LOW20:
		{
			// 红灯常亮
			Bat_Led_Low_Control(ENABLE);
			Bat_Led_Full_Control(DISABLE);
			break;
		}
		case BAT_LEVEL_LOW10:
		case BAT_LEVEL_LOW5:
		case BAT_LEVEL_EMETY:
		{
			// 红灯闪烁
			i ++;
			Bat_Led_Full_Control(DISABLE);
			if (i % 50 == 0)
			{
				if(0 == breath_flag)
				{
					Bat_Led_Low_Control(ENABLE);
					breath_flag = 1;
				}
				else
				{
					Bat_Led_Low_Control(DISABLE);
					breath_flag = 0;
				}
				i = 0;
			}
			break;
		}


		default :
		{
			break;
		}
	}
}

/*
*************************************************************
* 功能说明:  系统充电时，电源指示灯处理,需要周期调用
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
static void sys_charge_bat_leds_disp(void)
{
	switch(Power.CharheState)
	{
		case CHARGE_START:
		{
			if (Power.BatLevel > BAT_LEVEL_LOW20)
			{
				// 白灯呼吸
				Bat_Led_Low_Control(DISABLE);
				Bat_Led_Full_Breathe();
			}
			else
			{
				// 红灯呼吸
				Bat_Led_Low_Breathe();
				Bat_Led_Full_Control(DISABLE);
			}
			break;
		}
		case CHARGE_FINISH:
		{
			// 白灯常亮
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
		}
		default:    // 充电暂停时，led不刷新
		{
			break;
		}
	}
}

/*
*************************************************************
* 功能说明:  系统关机时，电源指示灯处理,需要周期调用
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
static void sys_off_bat_leds_disp(void)
{
	Level_Leds_Control(LEVEL_LEDS_OFF);
	Bat_Led_Full_Control(DISABLE);
	Bat_Led_Low_Control(DISABLE);
}

/*
*************************************************************
* 功能说明:  系统出错时，电源指示灯处理,需要周期调用
* 形    参: 无
* 返 回 值: 无 10ms 调用一次
*           1. NTC异常，    档位1灯亮
*           2. 电池温度异常，档位2灯亮
*           3. 电池电压异常，档位3灯亮
*
*************************************************************
*/
static void sys_Err_leds_disp(void)
{
	static u8 inver_alarm_cnt;
	static u8 alarm_leds_on_flag;

	switch(Device.Err_State)
	{
		case ERR_RF_TEMP:
		{
			LED_1D_ON;
			LED_2D_OFF;
			LED_3D_OFF;
			break;
		}
		case ERR_BAT_TEMP:
		{
			LED_1D_OFF;
			LED_2D_ON;
			LED_3D_OFF;
			break;
		}
		case ERR_BAT_LEVEL:
		{
			LED_1D_OFF;
			LED_2D_OFF;
			LED_3D_ON;
			break;
		}
		default:
		{
			break;
		}
	}

	inver_alarm_cnt ++;

	if(0 == inver_alarm_cnt % 25)
	{
		if(alarm_leds_on_flag)
		{
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(DISABLE);
			alarm_leds_on_flag = 0;
		}
		else
		{
			Bat_Led_Low_Control(ENABLE);
			Bat_Led_Full_Control(ENABLE);
			alarm_leds_on_flag =1;
		}

		inver_alarm_cnt = 0;
	}
}


/*
*************************************************************
* 功能说明: 电源指示灯处理,需要周期调用
* 形    参: 无
* 返 回 值: 无 10ms调用一次
*************************************************************
*/
static void test_leds_disp(void)
{
	disp_data.trigle_delay_cnt ++;
	if(disp_data.trigle_delay_cnt > 50)
	{
		disp_data.trigle_delay_cnt = 0;
		if(disp_data.trigle_flag)
		{
			LED_1D_OFF;
			LED_2D_OFF;
			LED_3D_OFF;
			LED_LASER_OFF;
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(DISABLE);
			disp_data.trigle_flag = 0;
			//  printf("LED test : led off \r\n");
		}
		else
		{
			LED_1D_ON;
			LED_2D_ON;
			LED_3D_ON;
			LED_LASER_ON;
			Bat_Led_Low_Control(ENABLE);
			Bat_Led_Full_Control(ENABLE);
			disp_data.trigle_flag = 1;
			// printf("LED test : led on \r\n");
		}
	}
}

/*
*************************************************************
* 功能说明: 电源指示灯处理,需要周期调用 老化测试显示
* 形    参: 无
* 返 回 值: 无 10ms调用一次
*           1. 老化测试完毕后，所有灯闪烁
*           2.  ems输出时，档位LEVEL3 闪烁
*           3.  充电时，   档位 level2 闪烁
*       10ms 调用一次
*************************************************************
*/
static void Aging_charge_disp(void)
{
	disp_data.trigle_delay_cnt ++;
	if(disp_data.trigle_delay_cnt > 100)
	{
		disp_data.trigle_delay_cnt = 0;
		if(disp_data.trigle_flag)
		{
			LED_2D_OFF;
			LED_LASER_OFF;
			disp_data.trigle_flag = 0;
			//printf("\r\n charge_leds_off \r\n");
		}
		else
		{
			LED_1D_ON;
			LED_2D_ON;
			LED_3D_ON;
			Bat_Led_Low_Control(ENABLE);
			Bat_Led_Full_Control(ENABLE);
			disp_data.trigle_flag = 1;
			//printf("\r\n charge_leds_on \r\n");
		}
	}
}

/*
*************************************************************
* 功能说明: 电源指示灯处理,需要周期调用 老化测试显示
* 形    参: 无
* 返 回 值: 无 10ms调用一次
*           1. 老化测试完毕后，所有灯闪烁
*           2.  ems输出时，档位LEVEL3 闪烁
*           3.  充电时，   档位 level2 闪烁
*       10ms 调用一次
*************************************************************
*/
static void Aging_EMS_RF_disp(void)
{
	disp_data.trigle_delay_cnt ++;
	if(disp_data.trigle_delay_cnt > 100)
	{
		disp_data.trigle_delay_cnt = 0;
		if(disp_data.trigle_flag)
		{
			LED_3D_OFF;
			disp_data.trigle_flag = 0;
		}
		else
		{
			LED_1D_ON;
			LED_2D_ON;
			LED_3D_ON;
			LED_LASER_ON;
			Bat_Led_Low_Control(ENABLE);
			Bat_Led_Full_Control(ENABLE);
			disp_data.trigle_flag = 1;
		}
	}
}


/*
*************************************************************
* 功能说明: 电源指示灯处理,需要周期调用 老化测试显示
* 形    参: 无
* 返 回 值: 无 10ms调用一次
*           1. 老化测试完毕后，所有灯闪烁
*           2.  ems输出时，档位LEVEL3 闪烁
*           3.  充电时，   档位 level2 闪烁
*       10ms 调用一次
*************************************************************
*/
static void test_Aging_disp(void)
{
	if(Test.Aging_charge_flag)
	{
		Aging_charge_disp();
	}
	else if ((Test.Aging_ems_rf_flag) && (Test.Aging_ems_rf_cnt < AGING_EMS_RF_MAX_CNT))
	{
		Aging_EMS_RF_disp();
	}
	else //if(Test.Aging_finish_flag)
	{
		test_leds_disp();
	}
}

/*
*************************************************************
* 功能说明: 电源指示灯处理,需要周期调用
* 形    参: 无
* 返 回 值: 无 10ms调用一次
*************************************************************
*/
void Power_Led_Process(void)
{
	static u8 first_sys_off_flag;

//	printf(" bat_lvl =%d \r\n", Power.BatLevel);
	switch (Device.State)
	{
		case SYS_ON:
		{
			first_sys_off_flag = 0;
			Level_Leds_Control(LEVEL_LEDS_ON);
			sys_on_bat_leds_disp();
			break;
		}
		case SYS_WAKE:
		{
			Level_Leds_Control(LEVEL_LEDS_OFF);
			sys_wakeup_bat_leds_disp();
			break;
		}
		case SYS_CHARGE:
		{
			Level_Leds_Control(LEVEL_LEDS_OFF);
			sys_charge_bat_leds_disp();
			break;
		}
		case SYS_OFF:
		{
			Level_Leds_Control(LEVEL_LEDS_OFF);
			sys_off_bat_leds_disp();
			break;
		}

		case SYS_ERR:
		{
			sys_Err_leds_disp();
			break;
		}

		case SYS_TEST:
		{
			switch(Test.mode_sta)
			{
				case Light_Ctrl_STA:
				{
					test_leds_disp();
					break;
				}

				case Aging_Model_SAT:
				{
					test_Aging_disp();
					break;
				}

				default:
				{

					break;
				}
			}

			break;
		}
		case SYS_ADJUST_RELOAD:
		{
			test_leds_disp();
			break;
		}

		default:
		{
			break;
		}
	}
}

