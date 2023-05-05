/***********************************************************
 * @file    sys_app.c
 * @brief  ��Ӧ�ó���
 * @version V1.0
 * @date    2022.11.21
 ***********************************************************/
#include "device_app.h"
#include "led_app.h"
#include "drv_ems.h"

Device_Type Device;

/*
*************************************************************
* ����˵��: �����ܳ�ʼ��
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void Device_Init(void)
{
	Device.State = SYS_OFF;
	Device.Err_State = ERR_NONE;

	Device.LevelDir = UP_DIR;
	Device.OutCount = 0;
	Device.UseCount = 0;
	Device.WakeCount = 0;

	Device_Data_Read();
	Device_Level_Set(Device.Level);
}

/*
*************************************************************
* ����˵��: ��������
* ��    ��: State�л���״̬
* �� �� ֵ: ��
*************************************************************
*/
void Sys_state_Ctrl(Sys_State_Type State)
{
	Device.State = State;
	Device.OutCount = 0;
	Device.WakeCount = 0;
	Device.Err_Count = 0;


	LED_LASER_OFF;
	Power_Charge_Control(CHARGE_STOP);
	RF_Control(Func_DISABLE);
	Ems_Control(Func_DISABLE);

	switch(Device.State)
	{
		case SYS_OFF:
		{
				// USB����
			if (SensorPPR.State == 0)
			{

			}
			else
			{
				Device_Data_Write();   // ��Ϊд��flashʱ���������ȡADC�쳣
			}
			// Delay_ms(5);
			Device.IdleCnt = 0;
			Device.UseCount = 0;
			POW_OFF;
			break;
		}
		case SYS_ON:
		{
			LED_LASER_ON;
			POW_ON;
			break;
		}
		case SYS_WAKE:
		{
			POW_ON;
			break;
		}
		case SYS_CHARGE:
		{
			POW_ON;
			break;
		}
		case SYS_ERR:
		{
			change_to_err_state();
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
* ����˵��: ������λ����
* ��    ��: data��λ
* �� �� ֵ: ��
*************************************************************
*/
void Device_Level_Set(u8 data)
{
	Device.Level = data;

	// �������ʱ��

	switch (Device.Level)
	{
		case LEVEL1:
		{
			Device.RF_Time = _RF_LEVEL1_TIME_;
			Device.EMS_Time = _EMS_LEVEL1_TIME_;
			break;
		}
		case LEVEL2:
		{
			Device.RF_Time = _RF_LEVEL2_TIME_;
			Device.EMS_Time = _EMS_LEVEL2_TIME_;
			break;
		}

		case LEVEL3:
		{
			Device.RF_Time = _RF_LEVEL3_TIME_;
			Device.EMS_Time = _EMS_LEVEL3_TIME_;
			break;
		}
		default:
		{
			Device.RF_Time = _RF_LEVEL1_TIME_;
			Device.EMS_Time = _EMS_LEVEL1_TIME_;
			break;
		}
	}
}



/*
*************************************************************
* ����˵��: ����ػ�����
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void change_to_err_state(void)
{
	//static u16 pwr_off_delay = 0;
#if (_WDT_ENABLE)
	/* reload FWDGT counter */
	fwdgt_counter_reload();
#endif

	Motor_Start_Time(200);
	printf("err_sta_motor run \r\n");
}



/*
*************************************************************
* ???????: ??????????
* ??    ??: ??
* ?? ?? ?: ??
*************************************************************
*/
void Device_Data_Write(void)
{
	u32 buffer[4] = {0};

	Adc_DeInit();      // 2023 04 09 ???????flash?��?????? dma???adc??????��?????????????flash??????adc???
	buffer[0] = Device.Level;
	buffer[1] = RF.Ntc_Offset;
	buffer[2] = Test.Aging_finish_flag;
	buffer[3] = RF.UnLoad_ADC_Val;
	Flash_Write_Word(LEVEL_ADDR, buffer, 4);

	Adc_ReInit();
}

/*
*************************************************************
* ����˵��: ��ȡϵͳ����
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void Device_Data_Read(void)
{
	u32 temp = 0;
	s32 stemp = 0;

	RF.UnLoad_ADC_Val  = REG32(UNLOAD_ADC_VAL_ADDR);
	printf("read UnLoad_ADC_Val =%d\r\n", RF.UnLoad_ADC_Val);

	temp = REG32(LEVEL_ADDR);
	if ((temp < LEVEL1) || (temp > LEVEL3))
	{
		RF.Ntc_Offset = 30;   // ��ȡ�쳣�� Ĭ��ֵ
		Test.Aging_finish_flag = 0;
		Device.Level = LEVEL1;
		RF.UnLoad_ADC_Val = T9_2V1_ADC_VAL;
		return;
	}
	Device.Level = temp;

	stemp = REG32(NTC_OFFSET_ADDR);
	if (abs(stemp) < 100)
	{
		RF.Ntc_Offset = stemp;
	}
	else
	{
		RF.Ntc_Offset = 30;
	}

	Test.Aging_finish_flag = REG32(AGING_FINISH_FLAG_ADDR);
	//printf("read _finish_flag =%d\r\n", Test.Aging_finish_flag);
	if(Test.Aging_finish_flag > 1)
	{
		Test.Aging_finish_flag = 0;
	}

}

/*
*************************************************************
* ����˵��: ��������ڴ�����,�����ʱ��25MS , 1ms����һ��
*
* Ŀ��: 1.�¶����¿죬�ȶ���Ԥ��ֵ
* NTC�������ԣ�NTC�¶ȴ��ݵ� �����ԣ�
* ���еĲ�����
*      1.�������½׶Σ� �տ����׶Σ��¶�NTCֵ�ϵͣ�����RF�����ʱ�䣬
*      2.�������½׶Σ� ��NTC�����½׶δﵽһ��ֵʱ��˵���ⲿ�Ѿ�����42��C
*      3.���½׶Σ�     ��
*   �ú���ˮ�۲��ԣ��¶�NTC��ˮ�£����ղ��3��C
*************************************************************
*/
 u16 Temp_ctrl_RF_time(void)
{
	u16 Time = 0;
	static uint32_t Temp_lower_36_cnt = 0;
	static uint32_t Temp_lower_39_cnt = 0;
	static uint32_t Temp_lower_40_cnt = 0;
	static uint32_t Temp_lower_41_cnt = 0;

	static uint32_t printf_1s_cnt = 0;


	if (RF.Temp <= 370)  //380
	{
		Temp_lower_39_cnt ++;
		Temp_lower_40_cnt = 0;
		Temp_lower_41_cnt = 0;
		if(Temp_lower_39_cnt < 500)
		{
			Time = Device.RF_Time + 5;
		}
		else if(Temp_lower_39_cnt < 1000)
		{
			Time = Device.RF_Time + 10;
		}
		else if(Temp_lower_39_cnt < 5000)
		{
			Time = Device.RF_Time + 12;
		}
		else								//  ���ȳ���5s�󣬽���λ���ͣ������¶ȳ��ͣ���������
		{
			Temp_lower_39_cnt = 5000;
			Time = Device.RF_Time + 2;
		}
	}
	else if (RF.Temp <= 395)
	{
		Temp_lower_39_cnt = 0;
		Temp_lower_41_cnt = 0;
		Temp_lower_40_cnt ++;

		if(Temp_lower_40_cnt < 500)
		{
			Time = Device.RF_Time -2;
		}
		else if(Temp_lower_40_cnt < 1000)
		{
			Time = Device.RF_Time ;
		}
		else if(Temp_lower_40_cnt < 5000)
		{
			Time = Device.RF_Time + 2;
		}
		else                  //  ���ȳ���5s�󣬽���λ���ͣ������¶ȳ��ͣ���������
		{
			Temp_lower_40_cnt = 5000;
			Time = Device.RF_Time - 4;
		}
		//printf("RF_tmp_390 = %d \r\n", RF.Temp);
	}
	else if (RF.Temp <= 410)
	{
		// Time = Device.RF_Time;
		Temp_lower_39_cnt = 0;
		Temp_lower_40_cnt = 0;
		Temp_lower_41_cnt ++;

		if(Temp_lower_41_cnt < 500)
		{
			Time = Device.RF_Time - 7;
		}
		else if(Temp_lower_40_cnt < 1000)
		{
			Time = Device.RF_Time - 6;
		}
		else if(Temp_lower_40_cnt < 5000)
		{
			Time = Device.RF_Time - 5;
		}
		else
		{
			Temp_lower_40_cnt = 0;
			Time = Device.RF_Time - 6;
		}
		//printf("RF_tmp_405 = %d \r\n", RF.Temp);
	}
	else if (RF.Temp <= 420)
	{
		Temp_lower_39_cnt = 0;
		Temp_lower_40_cnt = 0;
		Temp_lower_41_cnt = 0;
		Time = Device.RF_Time/3;
		//printf("RF_tmp_415 = %d \r\n", RF.Temp);
	}
	else
	{
		Time = 0;
	}
	RF.duty_time = Time;

	return Time;
}

/*
*************************************************************
* ����˵��: ��������ڴ�����,�����ʱ��25MS
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void Device_Out_Process(void)
{
	static u16 MsCnt = 0; // ���Ӽ���
	static u8 Load_first = 0;
	static u8 Ems_Time_change_flag = 0;

	static u8 Ems_1ms_out_cnt;

	u16 RF_T = 0;

	if (Power.BatLevel <= BAT_LEVEL_LOW5)
	{
		RF_Control(Func_DISABLE);
		Ems_Control(Func_DISABLE);
		Device.Bat_Lower_3v2_cnt ++;
		if(Device.Bat_Lower_3v2_cnt > 1000)
		{
			Device.IdleCnt ++;
			Device.Bat_Lower_3v2_cnt = 0;
		}

		return;
	}
	Device.Bat_Lower_3v2_cnt = 0;

	MsCnt++;
	// �и��ؿ�ʼ���
	if (RF.Load == 1)
	{
		Device.IdleCnt = 0;
		RF.UnLoad_Count = 0;

		if (RF.Load_First == 0)
		{
			RF.Load_First = 1;
			Motor_Start_Time(200);
			// printf("\r\n rf_load motor_on:\r\n");
		}
		RF_T = Temp_ctrl_RF_time();
		// ���ʱ�����
		if (MsCnt >= 1000)
		{
			MsCnt = 0;
			Device.UseCount++;
		}

		// ������� ems + 1ms + rf + ����ʱ��ms = _OUT_TIME_
		// if (Device.OutCount == 0)
		// {
		// 	Ems_Control(Func_ENABLE);
		// }
		// else if (Device.OutCount == Device.EMS_Time)
		// {
		// 	Ems_Control(Func_DISABLE);
		// }
		// else if (Device.OutCount == (Device.EMS_Time + 1))
		// {
		// 	RF_Control(Func_ENABLE);
		// }
		// else if (Device.OutCount == (Device.EMS_Time + 1 + RF_T))
		// {
		// 	RF_Control(Func_DISABLE);
		// }
		// --------------------------Ems_Time_change_flag

		if (Device.OutCount == 0)
		{
			Ems_Control(Func_ENABLE);
		}
		else if (Device.OutCount == Device.EMS_Time)
		{
			Ems_Control(Func_DISABLE);
		}
		else if (Device.OutCount == (Device.EMS_Time + 1))
		{
			RF_Control(Func_ENABLE);
		}
		else if (Device.OutCount == (Device.EMS_Time + 1 + RF_T))
		{
			RF_Control(Func_DISABLE);
		}
        //-----------------------------
		Device.OutCount++;
		if (Device.OutCount >= _OUT_TIME_)
		{
			Device.OutCount = 0;
			Ems_1ms_out_cnt ++;
			switch (Device.Level)
			{
				case LEVEL2:
				{
					if(Ems_1ms_out_cnt > 2)
					{
						Ems_1ms_out_cnt = 0;
						Device.EMS_Time = _EMS_LEVEL2_TIME_ + 1;
					}
					else
					{
						Device.EMS_Time = _EMS_LEVEL2_TIME_;
					}
					break;
				}
				case LEVEL3:
				{
					// if(Ems_1ms_out_cnt > 1)
					{
						Ems_1ms_out_cnt = 0;
						Device.EMS_Time = _EMS_LEVEL3_TIME_ + 1;
					}
					// else
					// {
					// 	Device.EMS_Time = _EMS_LEVEL3_TIME_;
					// }
					break;
				}
				case LEVEL1:
				default:
				{
					Ems_1ms_out_cnt = 0;
					Device.EMS_Time = _EMS_LEVEL1_TIME_;
					break;
				}
			}
		}
	}
	else
	{
		// �뿪Ƥ��ʱ�����
		if (MsCnt >= 1000)
		{
			MsCnt = 0;
			Device.IdleCnt ++;
			if (RF.Load_First != 0)
			{
				RF.UnLoad_Count++;
				if (RF.UnLoad_Count >= 30)
				{
					RF.Load_First = 0;
					RF.UnLoad_Count = 0;
					Device.UseCount = 0; // �뿪Ƥ��30s���ۼ�����ʱ����0
				}
			}
		}

		// // �޸���RF���������м��
		// Ems_Control(Func_DISABLE);
		// switch (Device.OutCount)
		// {
		// case 1:
		// 	RF_Control(Func_ENABLE);
		// 	break;
		// case 6:
		// 	RF_Control(Func_DISABLE);
		// 	break;
		// }

		// Device.OutCount ++;
		// if (Device.OutCount >= 25)
		// {
		// 	Device.OutCount = 0;
		// }

		// �޸���EMS���������м��
		RF_Control(Func_DISABLE);
		switch (Device.OutCount)
		{
		case 1:
			Ems_Control(Func_ENABLE);
			break;
		case 3:   //6
			Ems_Control(Func_DISABLE);
			break;
		}

		Device.OutCount ++;
		if (Device.OutCount >= 25)
		{
			Device.OutCount = 0;
		}


	}

	// RFƵ��(���¶ȿ���)
	if (RF.State == Func_ENABLE)
	{
		RF_OutFreq_Cycle();
	}
}

/*
*************************************************************
* ����˵��: �Զ��ػ����ڴ�����
* ��    ��: ��
* �� �� ֵ: ��    1s ����һ��
*************************************************************
*/
void Device_AutoOff_Process(void)
{
	static u8 power_off_delay = 0;
	static u8 adjust_rf_delay = 0;

	switch(Device.State)
	{
		case SYS_OFF:
		{
			power_off_delay ++;       // �̰����ѣ���Ҫ��sys_off �л��ѣ����Բ������̹ر�
			if(power_off_delay > 2)
			{
				power_off_delay = 0;
				POW_OFF;
				// printf("SYS_OFF ,Close power on \r\n");
			}
			adjust_rf_delay = 0;
			break;
		}


		case SYS_ON:
		{
			if (Device.IdleCnt >= _AUTO_OFF_)
			{
				MOTOR_ON;
				Delay_ms(_LONG_TIME_);
				Sys_state_Ctrl(SYS_OFF);
			}
			else if (Device.UseCount >= _USE_OFF_)
			{
				MOTOR_ON;
				Delay_ms(_SHORT_TIME_);
				Sys_state_Ctrl(SYS_OFF);
			}
			adjust_rf_delay = 0;
			break;
		}
		case SYS_WAKE:
		{
			Device.WakeCount++;
			if(Power.BatLevel <= BAT_LEVEL_LOW10)
			{
				//if (Device.WakeCount >= _WAKE_OFF_ + 2)
				if (Device.WakeCount >= 10)
				{
					Sys_state_Ctrl(SYS_OFF);
				}
			}
			else
			{
				if (Device.WakeCount >= _WAKE_OFF_)
				{
					Sys_state_Ctrl(SYS_OFF);
				}
			}
			adjust_rf_delay = 0;
			break;
		}

		case SYS_ERR:
		{
			Device.Err_Count ++;
			if (Device.Err_Count >= _WAKE_OFF_)
			{
				Sys_state_Ctrl(SYS_OFF);
			}
			break;
		}

		case SYS_TEST:   // ����ģʽ�����ػ�   Device.State =  SYS_TEST
		{
			Device.IdleCnt = 0;
			//printf("Device.State in  SYS_TEST mode \r\n");
			break;
		}

		case SYS_ADJUST_RELOAD:
		{
			adjust_rf_delay ++;
			if(adjust_rf_delay >= 3)
			{
				adjust_rf_delay = 0;

				RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val;
				if(Power.adc_val < BAT_3V7_ADC_VAL)
				{
					RF.UnLoad_ADC_Val -= T9_0V5_ADC_VAL;

				}
				Device_Data_Write();
				Sys_state_Ctrl(SYS_ON);
				Level_Leds_Force_refresh();
			}
		}
		default:
		{
			break;
		}
	}

}

/*
*************************************************************
* ����˵��: ������,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: ��  100ms����һ��    if(Power.adc_val < BAT_3V9_ADC_VAL)
*************************************************************
*/
void Test_mode_Charge_Ctrl(void)
{
 //
	static uint16_t Charge_100ms_cnt;
	static uint8_t DC5V_not_online_cnt;

	switch(Test.mode_sta)
	{
		case Aging_Model_SAT:
		{
			if(Test.Aging_ems_rf_flag)
			{
				if((Test.Aging_finish_flag) && (Power.adc_val < BAT_3V7_ADC_VAL))
				{
					Test.Aging_charge_flag = 1;
					Test.Aging_ems_rf_flag = 0;
					Power_Charge_Control(CHARGE_START);
				}
				else
				{
					Test.Aging_charge_flag = 0;
					Power_Charge_Control(CHARGE_FINISH);
				}
				break;
			}
			Charge_100ms_cnt ++;
			if(Charge_100ms_cnt > 10)
			{
				Charge_100ms_cnt = 0;
				Test.Aging_charge_cnt ++;

				if (SensorPPR.State)
				{
					DC5V_not_online_cnt ++;
					if(DC5V_not_online_cnt > 5)    // ����ó���ʱ���������ӳ�����������ػ�״̬
					{
						Sys_state_Ctrl(SYS_WAKE);
					}
				}

			}
			if(Power.adc_val > BAT_3V9_ADC_VAL)
			{
				Power_Charge_Control(CHARGE_FINISH);
				Test.Aging_charge_cnt = AGING_CHARGE_MAX_CNT;
				Test.Aging_ems_rf_flag = 1;
				Test.Aging_charge_flag = 0;
			}
			else
			{
				Power_Charge_Control(CHARGE_START);
				Test.Aging_charge_flag = 1;
				Test.Aging_ems_rf_flag = 0;
			}
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
* ����˵��: ������,��Ҫ���ڵ���
* ��    ��: ��
* �� �� ֵ: ��  100ms����һ��
*************************************************************
*/
void Normal_mode_Charge_Ctrl(void)
{
	static u8 ChargeConut = 0; // ����������ʱһ��ʱ���ټ���Ƿ����

	// USB����
	if (SensorPPR.State == 0)
	{
		if ((Device.State == SYS_OFF) ||
			(Device.State == SYS_ON) ||
			(Device.State == SYS_WAKE))
		{
			Sys_state_Ctrl(SYS_CHARGE);
			return;
		}

		if (Device.State == SYS_CHARGE)
		{
			if(ERR_BAT_TEMP == Device.Err_State)  //bat_ntc err, stop charge Device.Err_State = ERR_BAT_TEMP;
			{
				Power_Charge_Control(CHARGE_STOP);
				return;
			}

			if (Power.CharheState == CHARGE_STOP)
			{
				if (Power.BatLevel <= BAT_LEVEL_FULL)
				{
					Power_Charge_Control(CHARGE_START);
					ChargeConut = 0;
				}
			}
			else if (Power.CharheState == CHARGE_START)
			{
				if (ChargeConut > 10)
				{
					if (SensorCharge.State == 1)
					{
						Power_Charge_Control(CHARGE_FINISH);
					}
				}
				else
				{
					ChargeConut++;
				}
			}
		}
	}
	else if (Device.State == SYS_CHARGE)
	{
		Sys_state_Ctrl(SYS_WAKE);
	}
}



