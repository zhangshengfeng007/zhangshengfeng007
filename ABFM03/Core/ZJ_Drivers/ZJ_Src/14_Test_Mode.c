#include "includes.h"
#include "Test_Mode.h"


#ifdef __GNUC__

  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

#else

  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif /* __GNUC__ */

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 50);
	return ch;
}

/**************************************************************************************
* FunctionName   : Key_2_4G_auto_match_mode_Check(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None    10ms 调用一�?
**************************************************************************************/
void Key_2_4G_auto_match_mode_Check(void)
{
	static uint16_t enter_auto_match_delay = 0;

	if(1 == SysInfo.Batt_Value.Usb_flag)
	{
		return;
	}

	if(ON == SysInfo.Test_Mode.Test_Mode_Flag)
	{
		return;
	}

	if(IS_LEVEL_ADD_KEY_PRESS() && IS_LEVEL_DEC_KEY_PRESS() && IS_POWER_KEY_UP())
	{
		enter_auto_match_delay ++;
		if(enter_auto_match_delay > 300)
		{
			enter_auto_match_delay = 0;
			SysInfo.Montor_Flag = 1; //使能电机振动标志
			SysInfo.StayTime = 40;
		    hw_2_4G_data.mode = AUTO_MATCH_MODE;
			enter_auto_match_delay = 0;
		}
	}
	else
	{
		enter_auto_match_delay = 0;
	}
}

/**************************************************************************************
* FunctionName   : Test_Mode_Key_Process(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None    10ms 调用一�?
**************************************************************************************/

void Key_Enter_Test_Mode_Check(void)
{
	static uint8_t  Enter_Test_STEP = 0;
	static uint16_t Test_timer_cnt, Key_Cnt;

	if(0 == SysInfo.Batt_Value.Usb_flag)
	{
		return;
	}

	if(ON == SysInfo.Test_Mode.Test_Mode_Flag)
	{
		return;
	}

	SysInfo.Test_Mode.Quit_Test_30s_Cnt ++;

	if(SysInfo.Test_Mode.Quit_Test_30s_Cnt >= 3000) //接入usb后前30秒内，按�?有效
	{
		SysInfo.Test_Mode.Quit_Test_30s_Cnt = 3000;
		return;
	}

	switch(Enter_Test_STEP)
	{
		case 0:
		{
			Test_timer_cnt ++ ;
			if(Test_timer_cnt < 50)
			{
				if(IS_POWER_KEY_UP() && IS_LEVEL_ADD_KEY_UP() && IS_LEVEL_DEC_KEY_UP())
				{
					Enter_Test_STEP = 1;
				}
			}
			break;
		}
		case 1:
		{

			if((IS_LEVEL_ADD_KEY_PRESS()) && (IS_LEVEL_DEC_KEY_UP()) && (SysInfo.Batt_Value.Usb_flag))
			{
				Enter_Test_STEP = 2;
				Test_timer_cnt = 0;
			}
			break;
		}
		case 2:
		{
			if((IS_LEVEL_ADD_KEY_PRESS()) && (IS_LEVEL_DEC_KEY_PRESS()) &&(SysInfo.Batt_Value.Usb_flag))		//���UP&DWN����3S
			{
				Test_timer_cnt++;
				if(Test_timer_cnt > 300)
				{
					Enter_Test_STEP = 3;
					Test_timer_cnt = 0;
					SysInfo.Test_Mode.Test_Mode_Flag = Standy; //�������ģ�?
					SysInfo.Power_Value.Enter_Sleep_Flag=0;
					SysInfo.Test_Mode.STATUS = NONE_TEST;
					//Level_led_scan(LEVEL_NULL_DISP);
					//State_led_scan(0x04);
				}
			}
			else
			{
				Test_timer_cnt = 0;
			}
			break;
		}

		case 3:
		{
			Test_timer_cnt ++;

			if(Test_timer_cnt < 300)
			{
				if(IS_LEVEL_ADD_KEY_UP() && IS_LEVEL_DEC_KEY_UP())	//���UP&DWN���ɿ�
				{
					Enter_Test_STEP = 4;
					Test_timer_cnt = 0;
					Key_Cnt =0;
				}
			}
			else
			{
				SysInfo.Test_Mode.Test_Mode_Flag = OFF;         //�˳�����ģʽ
			}
			break;
		}
		case 4:
		{
			Test_timer_cnt ++;

			if(Test_timer_cnt < 6000)
			{
				if(IS_POWER_KEY_PRESS())
				{
					Key_Cnt++;
				}
				if(Key_Cnt >= 4 && IS_POWER_KEY_UP())				//���˶̰�KEY_ON����⵽���������˳�?
				{
					Enter_Test_STEP = 0;
					SysInfo.Test_Mode.Test_Mode_Flag = ON;      //�������ģ�?
					// State_led_scan(Purple);
					SysInfo.Test_Mode.STATUS = TILA_MODE_RF_TEST;
					Key_ctrl_test_mode_func();
				}
			}
			else
			{
				Enter_Test_STEP = 0;
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

/**************************************************************************************
* FunctionName   : Test_LED_Display_Process(void)
* Description    : ����ģʽLED��˸����
* EntryParameter : None
* ReturnValue    : None 10ms
**************************************************************************************/
void Test_mode_ctrl_Process(void)
{
	static uint8_t G_sensor_state = 0;
	static uint8_t motor_run_delay;
	static uint8_t thread_10ms_cnt = 0;

	switch(SysInfo.Test_Mode.STATUS)
	{
		case Gsensor_TEST:
		{
			if(G_sensor_state != SysInfo.MotionStateFlage)
			{
				if(SysInfo.MotionStateFlage)
				{
					G_SENSOR_run_send_data();
				}
				else
				{
					G_SENSOR_idle_send_data();
				}
			}
			G_sensor_state = SysInfo.MotionStateFlage;
			break;
		}

		case AGING_TEST:
		case INSET_LIFE_TEST:
		{
			SysInfo.MotionStateFlage = 1;
			SysInfo.Skin_Touch_Flag = 1;
			Agine_mode_output_ctrl();
			Agine_mode_charge_ctrl();
			thread_10ms_cnt ++;
			if(0 == thread_10ms_cnt % 100)
			{
				thread_10ms_cnt = 0;
				Write_Parameter_To_Eeprom();  // 寿命测试，增加 flash 压力测试
			}
			break;
		}

		case NTC_WITH_OUTPUT_TEST:
		{
			SysInfo.MotionStateFlage = 1;
			SysInfo.Skin_Touch_Flag = 1;
			break;
		}

		case ADJUST_NTC_TEST:
		{
			thread_10ms_cnt ++;
			if(0 == thread_10ms_cnt % 100)
			{
				thread_10ms_cnt = 0;
				SysInfo.Test_Mode.ADJUST_NTC_Delay ++;
				if(SysInfo.Test_Mode.ADJUST_NTC_Delay > 120) // 120 秒后进行校准
				{
					SysInfo.Test_Mode.ADJUST_NTC_Delay = 0;

					if (abs(SysInfo.Test_Mode.NTC_TARGET - SysInfo.NTC_Temp) >= 8) // 超过5°C不再校准
					{
						// send_ntc_temp
						cali_ntc_result(False);
					}
					else
					{
						SysInfo.NTC_offset = SysInfo.Test_Mode.NTC_TARGET - SysInfo.NTC_Temp;
						SysInfo.NTC_Temp = SysInfo.Test_Mode.NTC_TARGET;
						Write_Parameter_To_Eeprom();
						cali_ntc_result(True);
						send_ntc_temp();
						// tx_buf[5] = 0x01;
					}

				}
				else   // 发送当前时间
				{
					send_data_to_usart(&SysInfo.Test_Mode.ADJUST_NTC_Delay, 1);
				}
			}
			break;
		}

		default:
		{
			break;
		}
	}
}


/**************************************************************************************
* FunctionName   : Test_RF_Waveform_Process(void)
* Description    : ����ģʽRF��ѹ��������
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Test_RF_Waveform_Process(void)
{

}
/**************************************************************************************
* FunctionName   : Test_EMS_Waveform_Process(void)
* Description    : ����ģʽRF��ѹ��������
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Test_EMS_Waveform_Process(void)
{


}

/**************************************************************************************
* FunctionName   : Test_Pole_(void)
* Description    : ����ģʽ���ݷ���
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Test_Pole_run(uint8_t Pole_Counts)
{
//	switch(Pole_Counts)
//	{
//		case 0:
//			Set_Mbi5020_Out(RF_CH1_ON_BIT);break ;
//		case 1:
//			Set_Mbi5020_Out(RF_CH2_ON_BIT);break ;
//		case 2:
//			Set_Mbi5020_Out(RF_CH3_ON_BIT);break ;
//		default:
//			Set_Mbi5020_Out(0);
//			Pole_Counts	=0;
//		break ;
//	}
////	return Pole_Counts;
}

/**************************************************************************************
* FunctionName   : EMS_Procedure_Run(void)
* Description    : 1s RF �� 2S EMS
* EntryParameter : None
* ReturnValue    : None  10ms ����һ��
**************************************************************************************/
void Test_Mode_EMS_RF_ctrl(void)
{
	// static uint8_t change_mode_delay = 0;

	if(ON != SysInfo.Test_Mode.Test_Mode_Flag)
	{
		return;
	}

	if(System_ON != SysInfo.Power_Value.state)
	{
		return;
	}

	switch(SysInfo.Mode_Switch_Flag)
	{
		case OUT_PUT_RF:
		{
			Ems_DeInit();
			Ems_Boost_On();

			SysInfo.Mode_Switch_Flag = OUT_PUT_NONE;
			if(!SysInfo.Test_Mode.EMS_Flag) // ��������EMS��־λ
			{
				if(!SysInfo.OverTemp_Flag)
				{
					RF_Init();
				}
			}

			RF_Handle.Run_Flag = 1;
			EMS_Handle.Run_Flag = 0;
			break;
		}

		case OUT_PUT_EMS:
		{
			RF_DeInit();

			SysInfo.Mode_Switch_Flag = OUT_PUT_NONE;
			if(!SysInfo.Test_Mode.RF_Flag)
			{
				if(!SysInfo.OverTemp_Flag)
				{
					Ems_Boost_On();
					Ems_Init();
				}
			}
			RF_Handle.Run_Flag = 0;
			EMS_Handle.Run_Flag = 1;
		}

		case OUT_PUT_NONE:
		{
			break;
		}

		default:
		{
			Ems_DeInit();
			RF_DeInit();
			break;
		}
	}

}
/**************************************************************************************
* FunctionName   : void NTC_Temp_SendData(void)
* Description    : ����ģʽ NTC���ݷ���
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void NTC_Temp_SendData(void)
{
	static uint16_t NTC_Cnt;

	if(0 == SysInfo.Test_Mode.Printf_NTC_Flag)
	{
		return;
	}
	if((READ_NTC_TEST == SysInfo.Test_Mode.STATUS) || (TILA_MODE_RF_TEST == SysInfo.Test_Mode.STATUS)\
		||(XIUFU_MODE_RF_TEST == SysInfo.Test_Mode.STATUS))
	{
		BOOST_5V_ON();
		NTC_Cnt ++;
		if(NTC_Cnt > 50)
		{
			NTC_Cnt = 0;
			printf ("\n\r NTC:%.1f��C \n\r",SysInfo.NTC_Temp);
		}
	}
	else
	{
		NTC_Cnt = 0;
	}
}

/**************************************************************************************
* FunctionName   : Key_ctrl_test_mode_func(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void enter_tila_rf_test(void)
{
	SysInfo.Test_Mode.RF_Flag = 0x01;
    SysInfo.Test_Mode.EMS_Flag = 0x00;
    SysInfo.WorkState = upkeep_mode;
	SysInfo.Test_Mode.Test_Mode_Flag = ON;
	SysInfo.Power_Value.state = System_ON;
	BOOST_5V_ON();
	SysInfo.Sleep_Counts = 0;
	SysInfo.Power_Value.Enter_Sleep_Flag = 0;
	SysInfo.Sleep_Flag = 0;
	SysInfo.Mode_Switch_Flag = 0x01;
  	// SysInfo.Test_Mode.NTC_With_Alarm_Flag = 0x00;
}

/**************************************************************************************
* FunctionName   : Key_ctrl_test_mode_func(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void enter_xiufu_rf_test(void)
{
	SysInfo.Test_Mode.RF_Flag = 0x01;
	SysInfo.Test_Mode.EMS_Flag = 0x00;
	SysInfo.WorkState=repair_mode;

	SysInfo.Test_Mode.Test_Mode_Flag = ON;
	SysInfo.Power_Value.state = System_ON ;
	BOOST_5V_ON();
	SysInfo.Sleep_Counts =0;
	SysInfo.Power_Value.Enter_Sleep_Flag = 0;
	SysInfo.Sleep_Flag = 0;

	SysInfo.Mode_Switch_Flag = 0x01;
  	// SysInfo.Test_Mode.NTC_With_Alarm_Flag = 0x00;

}


/**************************************************************************************
* FunctionName   : Key_ctrl_test_mode_func(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void enter_tila_ems_test(void)
{
	SysInfo.Test_Mode.EMS_Flag =0x01;
 	SysInfo.Test_Mode.RF_Flag =0x00;
	SysInfo.WorkState=upkeep_mode;
	SysInfo.Test_Mode.Test_Mode_Flag = ON;
	SysInfo.Power_Value.state = System_ON ;
	BOOST_5V_ON();
	SysInfo.Sleep_Counts =0;
	SysInfo.Power_Value.Enter_Sleep_Flag =0;
	SysInfo.Sleep_Flag = 0;
	SysInfo.Mode_Switch_Flag = 0x01;

}

/**************************************************************************************
* FunctionName   : Key_ctrl_test_mode_func(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
static void enter_wireless_test(void)
{
	SysInfo.WorkState=upkeep_mode;
	SysInfo.Power_Value.state = System_ON ;
	SysInfo.Test_Mode.Test_Mode_Flag = ON ;
	SysInfo.Test_Mode.EMS_Flag =0x00;
	SysInfo.Test_Mode.RF_Flag =0x00;
}

/**************************************************************************************
* FunctionName   : Key_ctrl_test_mode_func(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
static void enter_read_ntc_test(void)
{
	RF_DeInit();
	Ems_DeInit();
	SysInfo.Test_Mode.RF_Flag = 0x00;
	SysInfo.Test_Mode.EMS_Flag = 0x00;
	SysInfo.WorkState = Standy_mode;
	SysInfo.Power_Value.state = System_Standy;
	SysInfo.Test_Mode.Test_Mode_Flag = ON;
}

/******************************************************
* FunctionName   : Key_ctrl_test_mode_func(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None
********************************************************/
void Key_ctrl_test_mode_func(void)
{
	static uint8_t Lock_Flag,Test_Last_Mode=0;

	switch(SysInfo.Test_Mode.STATUS)
	{
		case TILA_MODE_RF_TEST:
		{
			SysInfo.Test_Mode.Printf_NTC_Flag = 1;
			SysInfo.Test_Mode.Set_RF_Freq = RF_ALL_FREQ;
			enter_tila_rf_test();
			break;
		}

		case XIUFU_MODE_RF_TEST:
		{
			SysInfo.Test_Mode.Printf_NTC_Flag = 1;
			SysInfo.Test_Mode.Set_RF_Freq = RF_ALL_FREQ;
			enter_xiufu_rf_test();
			break;
		}

		case TILA_MODE_EMS_TEST:
		{
			SysInfo.Test_Mode.Set_EMS_Freq = EMS_ALL_FREQ;
			SysInfo.Test_Mode.Printf_NTC_Flag = 0;
			enter_tila_ems_test();
   		 	break;
		}

		case WIRELESS_TEST:
		{
			SysInfo.Test_Mode.Printf_NTC_Flag = 0;
			enter_wireless_test();
			break;
		}

		case READ_NTC_TEST:
		{
			SysInfo.Test_Mode.Printf_NTC_Flag = 1;
			enter_read_ntc_test();
			break;
		}

		default:
		{
			SysInfo.Test_Mode.STATUS = NONE_TEST;
			break;
		}
	}
}

/**************************************************************************************
* FunctionName   : crc16_compute(uint8_t const * p_data, uint32_t size, uint16_t const * p_crc)
* Description    : CRCУ��
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
uint16_t crc16_compute(uint8_t const * p_data, uint32_t size)
{
    uint16_t crc =0xFFFF;

    for (uint32_t i = 0; i < size; i++)
    {
        crc  = (uint8_t)(crc >> 8) | (crc << 8);
        crc ^= p_data[i];
        crc ^= (uint8_t)(crc & 0xFF) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xFF) << 4) << 1;
    }
    return crc;
}

/*************************************************************************************
* FunctionName	 : BKP_SRAM_Init()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void BKP_SRAM_Init(void)
{
  //  TAMP->BKP0R = 0x55;
  HAL_RTCEx_BKUPWrite(&hrtc,0,0x55);  //���ݼĴ���д��0x55
  //HAL_RTCEx_BKUPRead(&hrtc,0);
}

/**************************************************************************************
* FunctionName   : Test_UART_Deal_Process(void)
* Description    : ����ģʽ �ϻ�ģʽ���ݳ�ʼ��
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Ageing_TestData_Init(void)
{
	SysInfo.Power_Value.Enter_Sleep_Flag = 0;
	SysInfo.Power_Value.state = System_ON ;
	BOOST_5V_ON();
	SysInfo.Heating_Flag = 0x00; //����ǰ����5�����У�ʹ�缫Ѹ�����±�־λ
	SysInfo.Idle_Run_Cnt = 0;
	SysInfo.Freq_Cnt = 0;

	SysInfo.repair_level  = Level_3;//Level_5;
	SysInfo.upkeep_level  = Level_3;//Level_5;
	SysInfo.WorkState = upkeep_mode;
	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;

}
/**************************************************************************************
* FunctionName   : Test_UART_Deal_Process(void)
* Description    : ����ģʽ
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Ageing_TestData_DeInit(void)
{
	SysInfo.Power_Value.Enter_Sleep_Flag = 0;
	SysInfo.Power_Value.state = System_Standy ;
	BOOST_5V_OFF();
	SysInfo.Heating_Flag = 0x00; //����ǰ����5�����У�ʹ�缫Ѹ�����±�־λ
	SysInfo.Idle_Run_Cnt = 0;
	SysInfo.Freq_Cnt = Frequency_1M0_CNT; //�ϻ�ģʽ�£�RF��1mhz����

	SysInfo.repair_level  = Level_None;
	SysInfo.upkeep_level  = Level_None;
	SysInfo.WorkState = Standy_mode;
	RF_Call_Data(); //�ر�RF��EMS
	IRled_stop();
}

/***********************************************
 * 老化测试逻辑�?
 *  1. 先将电池的点充满 >=BAT_80_100_STATUS
 *  2. 按紧致提拉模�?3档输出，输出5min，暂�?2min。防止内部期间过�?
 *  3. 运�?�到低电，开始充电，�?70%电量  BAT_20_30_STATUS�? BAT_30_40_STATUS
***********************************************/
void Test_USB_Plug_Scan(void)
{
	static uint8_t usb_plug_time;
	static uint8_t usb_no_plug_time;

	if(VIN_DET_IN() && BAT_CHARGE_STATE_IN())
	{
		usb_no_plug_time ++;
		// BOOST_5V_ON();          // 充电器不在线，电池电压供�?
		if(usb_no_plug_time>=5)
		{
			if(SysInfo.Batt_Value.Usb_flag)
			{
				//SysInfo.Save_Data.save_Data_flag =1;
				SysInfo.Save_Data.BattState	 =SysInfo.Batt_Value.State;
			}
			SysInfo.Batt_Value.Usb_flag = 0;
			usb_no_plug_time = 5;
		}
		usb_plug_time = 0;
	}
	else
	{
		usb_plug_time ++;
//		BOOST_5V_OFF();
		if(usb_plug_time >= 5)
		{
			SysInfo.Batt_Value.Usb_flag = 1;
			usb_plug_time = 5;
		}
		usb_no_plug_time = 0;
	}
}

/************************************************************
*  //10ms 调用一�?
*****************************************************************/
void Agine_mode_charge_ctrl(void)
{
	static uint16_t DC_5V_not_online_cnt = 0;
	static uint8_t charge_10ms_cnt = 0;

	if(0 == SysInfo.Test_Mode.Charge_flag)
	{
		return;
	}

	SysInfo.Mode_Switch_Flag = OUT_PUT_IDLE;
	switch(SysInfo.Test_Mode.STATUS)
	{
		case AGING_TEST:
		{
			if(0 == SysInfo.Test_Mode.Aging_run_once_over_flag)
			{
				SysInfo.Test_Mode.display_Charge_flag = 1;
				if((SysInfo.Batt_Value.Usb_flag)&&((SysInfo.Batt_Value.State < BAT_60_70_STATUS)))
				{
					BOOST_5V_OFF();   			// 开始充�?
				}
				if(SysInfo.Batt_Value.State >= BAT_60_70_STATUS)
				{
					//BOOST_5V_ON();          // 电池电压供电
					SysInfo.Test_Mode.Charge_flag = 0;
					SysInfo.Test_Mode.EMS_RF_out_flag = 1;
					SysInfo.Test_Mode.display_Charge_flag = 0;
					SysInfo.Mode_Switch_Flag = OUT_PUT_RF;
					RF_Handle.Run_Flag = 1;
				}

			}
			else
			{
				if(SysInfo.Batt_Value.State > BAT_30_40_STATUS)
				{
					SysInfo.Test_Mode.Charge_cnt ++;
					if(SysInfo.Test_Mode.Charge_cnt > 6000) // 多充�? 1min
					{
						//BOOST_5V_ON();          // 电池电压供电
						SysInfo.Test_Mode.Charge_cnt = 0;
						SysInfo.Test_Mode.display_Charge_flag = 0;
					}

				}
				else if(SysInfo.Batt_Value.State < BAT_20_30_STATUS)
				{
					SysInfo.Test_Mode.Charge_cnt = 0;
					BOOST_5V_OFF();   			// 开始充�?
					SysInfo.Test_Mode.display_Charge_flag = 1;
				}
			}
			break;
		}

		case INSET_LIFE_TEST:
		{
			SysInfo.Test_Mode.display_Charge_flag = 1;
			if((SysInfo.Batt_Value.Usb_flag)&&((SysInfo.Batt_Value.State < BAT_80_100_STATUS)))
			{
				BOOST_5V_OFF();   			// 开始充�?
			}
			if(SysInfo.Batt_Value.State >= BAT_80_100_STATUS)
			{
				//BOOST_5V_ON();          // 电池电压供电，因电池管理IC的原因，采用DC5v直接供电
				SysInfo.Test_Mode.Charge_flag = 0;
				SysInfo.Test_Mode.EMS_RF_out_flag = 1;
				SysInfo.Test_Mode.display_Charge_flag = 0;
				SysInfo.Mode_Switch_Flag = OUT_PUT_RF;
				RF_Handle.Run_Flag = 1;
			}
			break;
		}
	}

	charge_10ms_cnt ++;
	if(0 == charge_10ms_cnt % 100)
	{
		charge_10ms_cnt = 0;
		if(SysInfo.Batt_Value.Usb_flag == 0)
		{
			DC_5V_not_online_cnt ++;
			if(DC_5V_not_online_cnt > 80)  // 需要充电时�?80s没接入充电器，关机�?�理
			{
				BOOST_5V_OFF();   			// 掉电，关�?
				SysInfo.Sleep_Flag = 1;
				SysInfo.Test_Mode.Test_Mode_Flag = 0;
			}
		}
		else
		{
			DC_5V_not_online_cnt = 0;
		}
	}
}

/**********************************************
 * ---老化 完成标位 记录到flash
********************************************/
static void Agine_run_finish(void)
{
	SysInfo.Test_Mode.EMS_RF_out_flag = 0;
	SysInfo.Test_Mode.Aging_run_once_over_flag = 1;

	if((0 == SysInfo.Test_Mode.Ageing_finish_Flag) && (AGING_TEST == SysInfo.Test_Mode.STATUS))
	{
		SysInfo.Test_Mode.Ageing_finish_Flag = 1;
		Write_Parameter_To_Eeprom();
	}
	SysInfo.Test_Mode.Charge_flag = 1;
	SysInfo.Mode_Switch_Flag = OUT_PUT_IDLE;
}

/***********************************************************
 * 老化测试逻辑�?
 *  1. 先将电池的点充满 >=BAT_80_100_STATUS
 *  2. 按紧致提拉模�?3档输出，输出5min，暂�?2min。防止内部期间过�?
 *  3. 运�?�到低电，开始充电，�?70%电量  BAT_20_30_STATUS�? BAT_30_40_STATUS
 *
 *  // 如果接usb ---老化一个小时，即显示老化完毕
 *  // 如果未接usb---老化至电池电量低于 20%，即老化程序结束
 *
 * //10ms 调用一�?
*****************************************************************/
void Agine_mode_output_ctrl(void) //10ms????
{
	static uint8_t  LockFlag;
	static uint8_t  agine_10ms_cnt;
	static uint16_t StandyCnt;

	if((SysInfo.Test_Mode.Aging_run_once_over_flag) && (AGING_TEST == SysInfo.Test_Mode.STATUS))
	{
		SysInfo.Mode_Switch_Flag = OUT_PUT_IDLE;
		return;
	}

	if(0 == SysInfo.Test_Mode.EMS_RF_out_flag)
	{
		SysInfo.Mode_Switch_Flag = OUT_PUT_IDLE;
		return;
	}

	if(0 == SysInfo.Batt_Value.Usb_flag)
	{
		BOOST_5V_ON();          // 电池电压供电
	}
	else
	{							//-------- 因为 接入usb 5v时，充电IC硬件无法暂停充电。该芯片工作在边充电，边放电模式
		BOOST_5V_OFF();         // �?5V供电,防止继电器频繁动作
	}

	SysInfo.MotionStateFlage = 1;
	agine_10ms_cnt ++;

	if(AGING_TEST == SysInfo.Test_Mode.STATUS)
	{
		if(SysInfo.Test_Mode.Ageing_total_run_cnt > AGING_TOTAL_Run_CNT)
		{
			SysInfo.Test_Mode.Ageing_total_run_cnt = AGING_TOTAL_Run_CNT;
			Agine_run_finish();
			return;
		}
	}

	if(agine_10ms_cnt >= 100)
	{
		agine_10ms_cnt = 0;

		SysInfo.Test_Mode.Ems_RF_run_cnt ++;
		SysInfo.Test_Mode.Ageing_total_run_cnt ++;

		if(SysInfo.Test_Mode.Ems_RF_run_cnt > AGING_EMS_RF_Continue_CNT)
		{
			SysInfo.Test_Mode.EMS_RF_stop_cnt ++;
		}
	}

	if(SysInfo.Test_Mode.Ems_RF_run_cnt > AGING_EMS_RF_Continue_CNT)
	{
		SysInfo.Mode_Switch_Flag = OUT_PUT_IDLE;

		if(SysInfo.Test_Mode.EMS_RF_stop_cnt > AGING_EMS_RF_Stop_CNT)
		{
			SysInfo.Test_Mode.Ems_RF_run_cnt = 0;
			SysInfo.Test_Mode.EMS_RF_stop_cnt = 0;
			SysInfo.Mode_Switch_Flag = OUT_PUT_RF;
		}
		return;
	}

	if(SysInfo.Batt_Value.State < BAT_00_20_STATUS)
	{
		Agine_run_finish();
		return;
	}

	if(RF_Handle.Run_Flag)
	{
		if(++SysInfo.Mode_Cnt >= Timer_RF) //1s
		{
			SysInfo.Mode_Switch_Flag = OUT_PUT_EMS; //EMS????
			SysInfo.Mode_Cnt = 0;
		}
	}

	if(EMS_Handle.Run_Flag) //EMS????
	{
		if(++SysInfo.Mode_Cnt >= Timer_EMS) //2s
		{
			SysInfo.Mode_Cnt = 0;
			SysInfo.Mode_Switch_Flag = OUT_PUT_RF; //RF????
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
