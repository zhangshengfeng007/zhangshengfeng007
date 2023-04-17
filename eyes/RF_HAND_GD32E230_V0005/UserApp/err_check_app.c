/***********************************************************
* @file    task_app.c
* @brief  �������
* @version V1.0
* @date    2023.02.02
***********************************************************/
#include "err_check_app.h"




/************************************************
* //����¶ȱ����� ����¶ȼ��
* ����¶ȷ�Χ ��0~45��C
* �ŵ��¶ȷ�Χ ��-10��C~60��C
*
************************************************/

static void bat_temp_range_judge(s16 MAX_BatTemp, s16 Min_BatTemp)
{
	static u8 Bat_tmp_err_cnt = 0;
	static u8 Bat_tmp_normal_cnt = 0;

	if((Power.BatTemp > MAX_BatTemp)||(Power.BatTemp < Min_BatTemp))
	{
		Bat_tmp_err_cnt ++;
		if(Bat_tmp_err_cnt > 10)
		{
			Bat_tmp_err_cnt = 10;
			if(ERR_BAT_TEMP != Device.Err_State)
			{
				printf("_BAT_TEMP: Device.State =%d, BatTemp = %d\r\n", Device.State, Power.BatTemp);
				Device.Err_State = ERR_BAT_TEMP;
				if(SYS_CHARGE != Device.State)
				{
					Sys_state_Ctrl(SYS_ERR);
				}

			}
		}
	}
	else
	{    //�¶Ȼָ� [Min +2��C , Max -2��C]
		if((Power.BatTemp > Min_BatTemp + 20)&&(Power.BatTemp < MAX_BatTemp -20))
		{
			Bat_tmp_err_cnt = 0;
			Bat_tmp_normal_cnt ++;
			if(Bat_tmp_normal_cnt > 5)    // �ۼ�5�κ󣬻ָ�
			{
				Bat_tmp_normal_cnt = 5;
				if(ERR_BAT_TEMP == Device.Err_State)
				{
					Device.Err_State = ERR_NONE;
				}
			}
		}

	}
}

/************************************************
 * ����¶ȱ�����
* ����¶ȷ�Χ ��0~45��C
* �ŵ��¶ȷ�Χ ��-10��C~60��C
*               100ms����һ��
************************************************/
void bat_temp_err_check(void)
{
	s16 tmp_max_val = 450;
	s16 tmp_min_val = 0;
	static u16 check_bat_temp_delay = 0;

	if(SYS_WAKE == Device.State)
	{
		check_bat_temp_delay = 0;
		if(ERR_BAT_TEMP == Device.Err_State)
		{
			Device.Err_State = ERR_NONE;
		}
		return;
	}

	check_bat_temp_delay ++;
	if(check_bat_temp_delay < 15) // ����ǰ1.5s ��������¶�
	{
		return;
	}
	check_bat_temp_delay = 15;

	switch(Device.State)
	{
		case SYS_CHARGE:
		{
			bat_temp_range_judge(tmp_max_val, tmp_min_val);
			break;
		}
		case SYS_TEST:
		{
			switch(Test.mode_sta)
			{
				case Aging_Model_SAT:
				{
					if(Test.Aging_ems_rf_flag)
					{
						tmp_max_val = 600;
						tmp_min_val = -100;
					}
					bat_temp_range_judge(tmp_max_val, tmp_min_val);
					break;
				}

				default:
				{
					tmp_max_val = 600;
					tmp_min_val = -100;
					bat_temp_range_judge(tmp_max_val, tmp_min_val);
					break;
				}
			}
			break;
		}
		case SYS_OFF:    // �ػ���ʱ���ټ��,���������
		{
			check_bat_temp_delay = 0;
			if(ERR_BAT_TEMP == Device.Err_State)
			{
				Device.Err_State = ERR_NONE;
			}
			break;
		}
		case SYS_ON:
		default:
		{
			tmp_max_val = 600;
			tmp_min_val = -100;
			bat_temp_range_judge(tmp_max_val, tmp_min_val);
			break;
		}
	}
}


/************************************************
 * ��ص�ѹ�쳣����ص�ѹС�� 3.2v ��ѹ���ͣ�����
*
* 100ms����һ��
************************************************/
void bat_val_err_check(void)
{
	static u8 bat_val_err_cnt;
	static u8 bat_val_normal_cnt;

	switch (Device.State)
	{
		case SYS_ON:
		{
			if(Power.BatLevel <= BAT_LEVEL_EMETY)
			{
				bat_val_err_cnt ++;
				if(bat_val_err_cnt > 100)
				{
					printf("_BAT_LEVEL_ERR_  \r\n");
					Device.Err_State = ERR_BAT_LEVEL;
					Sys_state_Ctrl(SYS_ERR);
				}
			}
			else
			{
				bat_val_err_cnt = 0;
				bat_val_normal_cnt ++;
				if(bat_val_normal_cnt > 10)
				{
					bat_val_normal_cnt = 10;
					if(ERR_BAT_LEVEL == Device.Err_State)
					{
						Device.Err_State = ERR_NONE;
					}
				}
			}
			break;
		}
		case SYS_TEST:
		{
			if((Power.BatLevel == BAT_LEVEL_EMETY) && (Test.Aging_ems_rf_flag))
			{
				bat_val_err_cnt ++;
				if(bat_val_err_cnt > 5)
				{
					printf("_BAT_LEVEL_ERR_  \r\n");
					Device.Err_State = ERR_BAT_LEVEL;
					Sys_state_Ctrl(SYS_ERR);
				}
			}
			else
			{
				bat_val_err_cnt = 0;
				bat_val_normal_cnt ++;
				if(bat_val_normal_cnt > 10)
				{
					bat_val_normal_cnt = 10;
					if(ERR_BAT_LEVEL == Device.Err_State)
					{
						Device.Err_State = ERR_NONE;
					}
				}
			}
			break;
		}
		default:
		{
			if(Power.BatLevel >= BAT_LEVEL_LOW5)
			{
				bat_val_normal_cnt ++;
				if(bat_val_normal_cnt > 10)
				{
					bat_val_normal_cnt = 10;
					if(ERR_BAT_LEVEL == Device.Err_State)
					{
						Device.Err_State = ERR_NONE;
					}
				}
			}
			break;
		}
	}
}


/************************************************
 * NTC�¶ȱ���������45��C
*
* 1.��磬���ѣ�off ����״̬�²��ټ��rf NTC
*
************************************************/
void rf_temp_err_check(void)
{
	static u8 rf_temp_err_cnt = 0;
	static u8 rf_temp_normal_cnt = 0;

	switch (Device.State)
	{
		case SYS_WAKE:
		{
			/* code */
			if(ERR_RF_TEMP == Device.Err_State)
			{
				Device.Err_State = ERR_NONE;  // �澯ȡ��
			}
			break;
		}

		case SYS_OFF:
		{
						/* code */
			if(ERR_RF_TEMP == Device.Err_State)
			{
				Device.Err_State = ERR_NONE;  // �澯ȡ��
			}
			break;
		}

		case SYS_CHARGE:
		{
			if(ERR_RF_TEMP == Device.Err_State)
			{
				Device.Err_State = ERR_NONE;  // �澯ȡ��
			}
			break;
		}

		default:
		{
			if(RF.Temp >= 500)
			{
				rf_temp_err_cnt++;
				if(rf_temp_err_cnt > 5)
				{
					rf_temp_err_cnt = 5;
					if(SYS_ERR != Device.State)
					{
						Device.Err_State = ERR_RF_TEMP;
						printf("RF_TEMP_ERR , RF.Temp=%d \r\n", RF.Temp);
						Sys_state_Ctrl(SYS_ERR);
					}
				}
			}
			else
			{
				rf_temp_err_cnt = 0;
				rf_temp_normal_cnt ++;
				if(rf_temp_normal_cnt > 10)
				{
					rf_temp_normal_cnt = 10;
					if(ERR_RF_TEMP == Device.Err_State)
					{
						Device.Err_State = ERR_NONE;  // �澯ȡ��
					}
				}

			}

			break;
		}
	}
}



