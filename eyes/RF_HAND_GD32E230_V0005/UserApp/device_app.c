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

	temp = REG32(LEVEL_ADDR);
	if ((temp < LEVEL1) || (temp > LEVEL3))
	{
		RF.Ntc_Offset = 0;   // ��ȡ�쳣�� Ĭ��ֵ
		Test.Aging_finish_flag = 0;
		Device.Level = LEVEL1;
		RF.UnLoad_ADC_Val = T9_0V70_ADC_VAL; // 2023 04 23 Ĭ��ֵдΪ 0.70mv
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
		RF.Ntc_Offset = 0;
	}

	Test.Aging_finish_flag = REG32(AGING_FINISH_FLAG_ADDR);
	//printf("read _finish_flag =%d\r\n", Test.Aging_finish_flag);
	if(Test.Aging_finish_flag > 1)
	{
		Test.Aging_finish_flag = 0;
	}

	RF.UnLoad_ADC_Val  = REG32(UNLOAD_ADC_VAL_ADDR);
	printf("read UnLoad_ADC_Val =%d\r\n", RF.UnLoad_ADC_Val);

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
*                  �ú���ˮ�۲�������ʱ�䣺���������£�Լ25��C��,������42��C,Լ2min~3min
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
* ����˵��: ��������ڴ�����,�����ʱ��25MS , 1ms����һ��
*
*
* Ŀ��: 1. ������λ֮�䣬rf�����Ҫ�����Բ���죨��Ʒ����Ҳ�Ǻ�������
		2. �¶����¿죬�ȶ���Ԥ��ֵ
* NTC�������ԣ�NTC�¶ȴ��ݵ� �����ԣ�
* ���еĲ�����
*      1.�������½׶Σ� �տ����׶Σ��¶�NTCֵ�ϵͣ�����RF�����ʱ�䣬
*      2.�������½׶Σ� ��NTC�����½׶δﵽһ��ֵʱ��˵���ⲿ�Ѿ�����42��C
*      3.���½׶Σ�     ��
*   �ú���ˮ�۲��ԣ��¶�NTC��ˮ�£����ղ��3��C
*                  �ú���ˮ�۲�������ʱ�䣺���������£�Լ25��C��,������42��C,Լ2min~3min
*************************************************************
*/
 u16 Ctrl_RF_time_depend_on_level(void)
{
	u16 Time = 0;

	if (RF.Temp <= 375)
	{
		Time = Device.RF_Time;
	}
	else if (RF.Temp <= 390)
	{
		Time = Device.RF_Time /2;
	}
	else if (RF.Temp <= 405)
	{
		Time = Device.RF_Time /3;
	}
	else if (RF.Temp <= 415)
	{
		//Time = Device.RF_Time /2;
		// Time = Device.RF_Time;
		Time = Device.RF_Time /4;
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
* ����˵��:
*
* ʵ�ʲ����з��֣�
* 1. ������ems���ʱ���¶���Ȼ����������emsҲ�����µĹ���
* 2. ����ά��ems����в��䣬���Ը����¶ȿ��� һ�����ڵ�ʱ��
*
*************************************************************
*/
 u16 Temp_Ctrl_OneCycle(void)
{
	u16 Time = _OUT_TIME_;

	if (RF.Temp >= 430)
	{
		Time = _OUT_TIME_ + _OUT_TIME_ + _OUT_TIME_;
	}
	else if (RF.Temp >= 415)
	{
		Time = _OUT_TIME_ + _OUT_TIME_;
	}
	else if (RF.Temp >= 405)
	{
		Time = _OUT_TIME_ + 10;
	}
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

	static u8 bat_low_sleep_delay_cnt = 0;

	u16 RF_T = 0;
	u16 Cycle_T = 0;

	if (Power.BatLevel <= BAT_LEVEL_LOW5)
	{
		RF_Control(Func_DISABLE);
		Ems_Control(Func_DISABLE);
		Device.Bat_Lower_3v2_cnt ++;
		if(Device.Bat_Lower_3v2_cnt > 1000)
		{
			Device.IdleCnt ++;
			Device.Bat_Lower_3v2_cnt = 0;

			bat_low_sleep_delay_cnt ++;
			if(bat_low_sleep_delay_cnt > 3)
			{
				Device.IdleCnt = _AUTO_OFF_;
			}
		}

		return;
	}
	bat_low_sleep_delay_cnt = 0;
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
		//RF_T = Temp_ctrl_RF_time();
		RF_T = Ctrl_RF_time_depend_on_level();
		Cycle_T = Temp_Ctrl_OneCycle();
		Debug_data.dbg_val = Cycle_T;
		// ���ʱ�����
		if (MsCnt >= 1000)
		{
			MsCnt = 0;
			Device.UseCount++;
		}


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
		if (Device.OutCount >= Cycle_T)  //_OUT_TIME_
		{
			Device.OutCount = 0;
			Ems_1ms_out_cnt ++;
			switch (Device.Level)
			{
				case LEVEL2:
				{
					if(Ems_1ms_out_cnt > 4)
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
					if(Ems_1ms_out_cnt > 4)
					{
						Ems_1ms_out_cnt = 0;
						Device.EMS_Time = _EMS_LEVEL3_TIME_ + 1;
					}
					else
					{
						Device.EMS_Time = _EMS_LEVEL3_TIME_;
					}
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

/*
1.��Ϊ������ͬ����Ӧ�Ŀ��غʹ��ص�ѹ��ͬ������ֻ��д������һ�������

 	��ص�ѹ         ���ص�ѹV0      ���ص�ѹV1       ��¼��Flash�Ļ�׼ֵ
	4.2v     		1.6v             2.3v            V0

	4.1v   			1.73v            2.3v            V0-0.15v
	4.0v   			1.75v            2.35v           V0-0.15v
	3.9v   			1.75v            2.4v            V0-0.15v

	3.8v   			1.85v            2.45v           V0-0.25v
	3.7v   			1.92v            2.5v            V0-0.30v

	3.6v   			2.0v             2.65v           V0-0.40v
	3.5v   			2.0v             2.7v            V0-0.40v

	3.4v   			2.1v             2.8v            V0-0.50v
	3.3v   			2.3v             2.9v            V0-0.70v
	3.2v   			2.3v             2.95v           V0-0.70v

*/
		case SYS_ADJUST_RELOAD:
		{
			adjust_rf_delay ++;
			if(adjust_rf_delay >= 3)
			{
				adjust_rf_delay = 0;

				// if(Power.adc_val > BAT_4V1_ADC_VAL)
				// {
				// 	RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val;
				// }
				// else if(Power.adc_val > BAT_3V8_ADC_VAL)
				// {
				// 	RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val - T9_0V15_ADC_VAL;
				// }
				// else if(Power.adc_val > BAT_3V7_ADC_VAL)
				// {

				// 	RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val - T9_0V25_ADC_VAL;
				// }
				// else if (Power.adc_val > BAT_3V6_ADC_VAL)
				// {
				// 	RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val - T9_0V30_ADC_VAL;
				// }
				// else if (Power.adc_val > BAT_3V4_ADC_VAL)
				// {
				// 	RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val - T9_0V40_ADC_VAL;
				// }
				// else if (Power.adc_val > BAT_3V3_ADC_VAL)
				// {
				// 	RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val - T9_0V50_ADC_VAL;
				// }
				// else if (Power.adc_val > BAT_3V2_ADC_VAL)
				// {
				// 	RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val - T9_0V70_ADC_VAL;
				// }
				// else
				{
					RF.UnLoad_ADC_Val = RF.POWER_DETECT_ADC_Val;
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
			if(Test.EMS_RF_out_flag)
			{
				if((Test.Aging_finish_flag) && (Power.adc_val < BAT_3V6_ADC_VAL))
				{
					Test.Charge_flag = 1;
					Test.EMS_RF_out_flag = 0;
					Power_Charge_Control(CHARGE_START);
				}
				else
				{
					Test.Charge_flag = 0;
					Power_Charge_Control(CHARGE_FINISH);
				}
				break;
			}
			Charge_100ms_cnt ++;
			if(Charge_100ms_cnt > 10)
			{
				Charge_100ms_cnt = 0;
				Test.Charge_cnt ++;

				if (SensorPPR.State)
				{
					DC5V_not_online_cnt ++;
					if(DC5V_not_online_cnt > 20)    // ����ó���ʱ���������ӳ�����������ػ�״̬
					{
						Sys_state_Ctrl(SYS_WAKE);
					}
				}
				else
				{
					DC5V_not_online_cnt = 0;
				}

			}
			if(Power.adc_val > BAT_3V9_ADC_VAL)
			{
				Power_Charge_Control(CHARGE_FINISH);
				Test.Charge_cnt = AGING_CHARGE_MAX_CNT;
				Test.EMS_RF_out_flag = 1;
				Test.Charge_flag = 0;
			}
			else
			{
				Power_Charge_Control(CHARGE_START);
				Test.Charge_flag = 1;
				Test.EMS_RF_out_flag = 0;
			}
			break;
		}

		case Inset_Life_Test_STA:
		{
			if(Test.EMS_RF_out_flag)
			{
				if(Power.adc_val < BAT_3V6_ADC_VAL)
				{
					Test.Charge_flag = 1;
					Test.EMS_RF_out_flag = 0;
					Power_Charge_Control(CHARGE_START);
				}
				else
				{
					Test.Charge_flag = 0;
					Power_Charge_Control(CHARGE_FINISH);
				}
				break;
			}

			Charge_100ms_cnt ++;
			if(Charge_100ms_cnt > 10)
			{
				Charge_100ms_cnt = 0;
				Test.Charge_cnt ++;

				if (SensorPPR.State)
				{
					DC5V_not_online_cnt ++;
					if(DC5V_not_online_cnt > 20)    // ����ó���ʱ���������ӳ�����������ػ�״̬
					{
						Sys_state_Ctrl(SYS_WAKE);
					}
				}
				else
				{
					DC5V_not_online_cnt = 0;
				}
			}
			if(Power.adc_val > BAT_3V9_ADC_VAL)
			{
				Power_Charge_Control(CHARGE_FINISH);
				if(Test.Charge_cnt > INSET_LIFE_CHARGE_MAX_CNT)
				{
					Test.EMS_RF_out_flag = 1;
					Test.Charge_flag = 0;
				}
			}
			else
			{
				Power_Charge_Control(CHARGE_START);
				Test.Charge_flag = 1;
				Test.EMS_RF_out_flag = 0;
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
	static u8 charge_100MS_cnt;

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
		charge_100MS_cnt ++;
		if(charge_100MS_cnt >= 10)
		{
			charge_100MS_cnt = 0;
			Device.charge_run_cnt ++;
		}
	}
	else if (Device.State == SYS_CHARGE)
	{
		Sys_state_Ctrl(SYS_WAKE);
		Device.charge_run_cnt = 0;
	}
}



