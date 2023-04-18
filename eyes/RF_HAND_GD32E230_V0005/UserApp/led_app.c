/***********************************************************
 * @file    sys_app.c
 * @brief  ��Ӧ�ó���
 * @version V1.0
 * @date    2022.11.21
 ***********************************************************/
#include "led_app.h"

#include "device_app.h"
#include "app_include.h"

LED_DISP_t disp_data;

/*
*************************************************************
* ����˵��: LEDǿ��ˢ��һ��
* ��    ��: cmd ����
* �� �� ֵ: ��
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
* ����˵��: ��λ�ƿ���
* ��    ��: cmd ����
* �� �� ֵ: ��
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
* ����˵��:  ϵͳ��������ʱ����Դָʾ�ƴ���,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: ��
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
			// 	// �׵Ƴ���
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
			break;
		}

		case BAT_LEVEL_LOW20:
		{
			// ��ƺ���
			Bat_Led_Low_Breathe();
			Bat_Led_Full_Control(DISABLE);
			break;
		}

		case BAT_LEVEL_LOW10:
		case BAT_LEVEL_LOW5:
		{
			// ��Ƴ���
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
* ����˵��:  ϵͳ���̰�����ʱ����Դָʾ�ƴ���,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: ��
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
			// �׵Ƴ���
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
			break;
		}

		case BAT_LEVEL_LOW20:
		{
			// ��Ƴ���
			Bat_Led_Low_Control(ENABLE);
			Bat_Led_Full_Control(DISABLE);
			break;
		}
		case BAT_LEVEL_LOW10:
		case BAT_LEVEL_LOW5:
		case BAT_LEVEL_EMETY:
		{
			// �����˸
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
* ����˵��:  ϵͳ���ʱ����Դָʾ�ƴ���,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: ��
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
				// �׵ƺ���
				Bat_Led_Low_Control(DISABLE);
				Bat_Led_Full_Breathe();
			}
			else
			{
				// ��ƺ���
				Bat_Led_Low_Breathe();
				Bat_Led_Full_Control(DISABLE);
			}
			break;
		}
		case CHARGE_FINISH:
		{
			// �׵Ƴ���
			Bat_Led_Low_Control(DISABLE);
			Bat_Led_Full_Control(ENABLE);
		}
		default:    // �����ͣʱ��led��ˢ��
		{
			break;
		}
	}
}

/*
*************************************************************
* ����˵��:  ϵͳ�ػ�ʱ����Դָʾ�ƴ���,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: ��
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
* ����˵��:  ϵͳ����ʱ����Դָʾ�ƴ���,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: �� 10ms ����һ��
*           1. NTC�쳣��    ��λ1����
*           2. ����¶��쳣����λ2����
*           3. ��ص�ѹ�쳣����λ3����
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
* ����˵��: ��Դָʾ�ƴ���,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: �� 10ms����һ��
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
* ����˵��: ��Դָʾ�ƴ���,��Ҫ���ڵ��� �ϻ�������ʾ
* ��    ��: ��
* �� �� ֵ: �� 10ms����һ��
*           1. �ϻ�������Ϻ����е���˸
*           2.  ems���ʱ����λLEVEL3 ��˸
*           3.  ���ʱ��   ��λ level2 ��˸
*       10ms ����һ��
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
* ����˵��: ��Դָʾ�ƴ���,��Ҫ���ڵ��� �ϻ�������ʾ
* ��    ��: ��
* �� �� ֵ: �� 10ms����һ��
*           1. �ϻ�������Ϻ����е���˸
*           2.  ems���ʱ����λLEVEL3 ��˸
*           3.  ���ʱ��   ��λ level2 ��˸
*       10ms ����һ��
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
* ����˵��: ��Դָʾ�ƴ���,��Ҫ���ڵ��� �ϻ�������ʾ
* ��    ��: ��
* �� �� ֵ: �� 10ms����һ��
*           1. �ϻ�������Ϻ����е���˸
*           2.  ems���ʱ����λLEVEL3 ��˸
*           3.  ���ʱ��   ��λ level2 ��˸
*       10ms ����һ��
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
* ����˵��: ��Դָʾ�ƴ���,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: �� 10ms����һ��
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

