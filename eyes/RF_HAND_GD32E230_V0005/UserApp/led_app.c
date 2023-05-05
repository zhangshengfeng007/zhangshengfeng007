/***********************************************************
 * @file    sys_app.c
 * @brief  ?????
 * @version V1.0
 * @date    2022.11.21
 ***********************************************************/
#include "led_app.h"

#include "device_app.h"
#include "app_include.h"

LED_DISP_t disp_data;

/*
*************************************************************
* ????: LED??????
* ?    ?: cmd ??
* ? ? ?: ?
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
* ????: ?????
* ?    ?: cmd ??
* ? ? ?: ?
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
* ????:  ???????,???????,??????
* ?    ?: ?
* ? ? ?: ?
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
			// 	// ????
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
			break;
		}

		case BAT_LEVEL_LOW20:
		case BAT_LEVEL_LOW10:
		{
			// ????
			Bat_Led_Low_Breathe();
			Bat_Led_Full_Control(DISABLE);
			break;
		}
		case BAT_LEVEL_LOW5:
		case BAT_LEVEL_LOW3:
		{
			// ????
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
* ????:  ????????,???????,??????
* ?    ?: ?
* ? ? ?: ?
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
			// ????
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
			break;
		}

		case BAT_LEVEL_LOW20:
		{
			// ????
			Bat_Led_Low_Control(ENABLE);
			Bat_Led_Full_Control(DISABLE);
			break;
		}
		case BAT_LEVEL_LOW10:
		case BAT_LEVEL_LOW5:
		case BAT_LEVEL_LOW3:
		case BAT_LEVEL_EMETY:
		{
			// ????
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
* ????:  ?????,???????,??????
* ?    ?: ?
* ? ? ?: ?
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
				// ????
				Bat_Led_Low_Control(DISABLE);
				Bat_Led_Full_Breathe();
			}
			else
			{
				// ????
				Bat_Led_Low_Breathe();
				Bat_Led_Full_Control(DISABLE);
			}
			break;
		}
		case CHARGE_FINISH:
		{
			// ????
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
		}
		default:    // ?????,led???
		{
			break;
		}
	}
}

/*
*************************************************************
* ????:  ?????,???????,??????
* ?    ?: ?
* ? ? ?: ?
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
* ????:  ?????,???????,??????
* ?    ?: ?
* ? ? ?: ? 10ms ????
*           1. NTC??,    ??1??
*           2. ??????,??2??
*           3. ??????,??3??
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
* ????: ???????,??????
* ?    ?: ?
* ? ? ?: ? 10ms????
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
* ????: ???????,?????? ??????
* ?    ?: ?
* ? ? ?: ? 10ms????
*           1. ???????,?????
*           2.  ems???,??LEVEL3 ??
*           3.  ???,   ?? level2 ??
*       10ms ????
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
* ????: ???????,?????? ??????
* ?    ?: ?
* ? ? ?: ? 10ms????
*           1. ???????,?????
*           2.  ems???,??LEVEL3 ??
*           3.  ???,   ?? level2 ??
*       10ms ????
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
* ????: ???????,?????? ??????
* ?    ?: ?
* ? ? ?: ? 10ms????
*           1. ???????,?????
*           2.  ems???,??LEVEL3 ??
*           3.  ???,   ?? level2 ??
*       10ms ????
*************************************************************
*/
static void test_Aging_disp(void)
{
	if(Test.Charge_flag)
	{
		Aging_charge_disp();
	}
	else if ((Test.EMS_RF_out_flag) && (Test.Ems_RF_cnt < AGING_EMS_RF_MAX_CNT))
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
* ????: ???????,?????? ??????
* ?    ?: ?
* ? ? ?: ? 10ms????
*           1. ???????,?????
*           2.  ems???,??LEVEL3 ??
*           3.  ???,   ?? level2 ??
*       10ms ????
*************************************************************
*/
static void Inset_life_charge_disp(void)
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
			//LED_1D_ON;
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
* ????: ???????,?????? ??????
* ?    ?: ?
* ? ? ?: ? 10ms????
*           1. ???????,?????
*           2.  ems???,??LEVEL3 ??
*           3.  ???,   ?? level2 ??
*       10ms ????
*************************************************************
*/
static void Inset_Life_EMS_RF_disp(void)
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
			//LED_1D_ON;
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
* ????: ???????,?????? ??????
* ?    ?: ?
* ? ? ?: ? 10ms????
*           1.  ??? 1,???????
*           2.  ems???,??LEVEL3 ??
*           3.  ???,   ?? level2 ??
*       10ms ????
*************************************************************
*/
static void Inset_Life_disp(void)
{
	if(Test.Charge_flag)
	{
		Inset_life_charge_disp();
	}
	else
	{
		Inset_Life_EMS_RF_disp();
	}
	LED_1D_OFF;
}


/*
*************************************************************
* ????: ???????,??????
* ?    ?: ?
* ? ? ?: ? 10ms????
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

				case Aging_Model_SAT:      // ????
				{
					test_Aging_disp();
					break;
				}
				case Inset_Life_Test_STA: // ????
				{
					Inset_Life_disp();
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

