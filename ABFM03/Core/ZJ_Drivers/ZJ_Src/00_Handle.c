#include "includes.h"

// extern uint32_t peer_code;


/**************************************************************************************
* FunctionName   : 电池电压转化为对应的 LED档位
* Description    :
* EntryParameter : None
* ReturnValue    : None   10MS运�?�一�?
**************************************************************************************/
static uint8_t Bat_level_check(void)
{
	LEVEL_LED_DISP_e led_level = LEVEL1_DISP;

	if(SysInfo.Batt_Value.State >= BAT_40_60_STATUS)
	{
		led_level = LEVEL3_DISP;
	}
	else if(SysInfo.Batt_Value.State >= BAT_20_30_STATUS)
	{
		led_level = LEVEL2_DISP;
	}
	else if(SysInfo.Batt_Value.State == BAT_00_20_STATUS)
	{
		led_level = LEVEL1_DISP;
	}
	else if(SysInfo.Batt_Value.State == BAT_00_00_STATUS)
	{
		led_level = LEVEL_NULL_DISP;
	}
	return led_level;
}

/**************************************************************************************
* FunctionName   : dealwith_wake_up_mode(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None   10ms进入一次， 唤醒时，led显示电量2s
**************************************************************************************/
static void dealwith_wake_up_mode(void)
{
	BAT_OUT_ENABLE();

	if(0 == Led_Value.StayTime)
	{
		Led_Value.Mode = SLEEP_MODE_DISP;
	}
	else
	{
		if(200 == Led_Value.StayTime)
		{
			// SysInfo.Save_Data.BattState  = *(__IO uint32_t*)(FLASH_BAT_STATE_ADDR);		//读取关机前的电量
			// if(SysInfo.Save_Data.BattState == BAT_00_00_STATUS)
			// {
			// 	Led_Value.Level = LEVEL_NULL_DISP;
			// }
			// else
			{
				Led_Value.Level = Bat_level_check();
			}
			// Led_Value.state = 1 ;

		}
		// if(Led_Value.StayTime % 10 == 0)
		// {
		// 	Led_Value.Level = Bat_level_check();
		// }
		Led_Value.StayTime --;
		Led_Value.Mode = WAKE_UP_DISP;
	}
}

/**************************************************************************************
* FunctionName   : dealwith_system_work_mode(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None   10ms进入一次， 唤醒时，led显示电量2s
**************************************************************************************/
static void dealwith_system_work_mode(void)
{
	if(hw_2_4G_data.mode == AUTO_MATCH_MODE)
	{
		Led_Value.Mode = AUTO_MATCH_2_4G;
		return;
	}


	switch(SysInfo.WorkState)
	{
		case repair_mode:
		{
			Led_Value.Level = SysInfo.Save_Data.repair_level;
			break;
		}
		case upkeep_mode:
		{
			Led_Value.Level = SysInfo.Save_Data.upkeep_level;
			break;
		}
		default:
		{
			Led_Value.Level = 0x5a;
			break;
		}
	}
	Led_Value.StayTime = 0;
	Led_Value.Mode = Batt_Normal;
}

/**************************************************************************************
* FunctionName   : dealwith_battery_charge_mode(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None   10ms进入一次， 唤醒时，led显示电量2s
**************************************************************************************/
static void dealwith_battery_charge_mode(void)
{
	static uint8_t  disp_Flag = 0;
	static uint16_t  bat_level_refresh_delay = 0;

	if(0 == disp_Flag)
	{
		disp_Flag = 1;
		SysInfo.Batt_Value.State = SysInfo.Save_Data.BattState;
		Led_Value.Level = Bat_level_check(); // 接入充电器后，立刻刷新led
	}

	bat_level_refresh_delay ++;
	if(bat_level_refresh_delay > 6000)  // 充电过程中，60s刷新一次，防止电池等级频繁跳动
	{
		if(SysInfo.Batt_Value.State > SysInfo.Save_Data.BattState)
		{
			SysInfo.Save_Data.BattState = SysInfo.Save_Data.BattState ++;
		}
		Led_Value.Level = Bat_level_check();
		bat_level_refresh_delay = 0;
	}

	if(LEVEL_NULL_DISP == Led_Value.Level) // 充电时，确保最低电量时，也要有颗灯在闪动
	{
		Led_Value.Level = LEVEL1_DISP;
	}

	Led_Value.Mode = Batt_Charging;
	Led_Value.StayTime = 100;
}

/**************************************************************************************
* FunctionName   : Led_Process_Run(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None   10MS运�?�一�?
**************************************************************************************/
void Led_Process_Run(void)
{
	static uint8_t  charge_mode_Flag = 0;

	if(SysInfo.Test_Mode.Test_Mode_Flag)
	{
		Led_Value.Mode = TEST_MODE_DISP; // 测试模式
	}
	else if(SysInfo.Batt_Value.Usb_flag) //充电模式
	{
		dealwith_battery_charge_mode();
		charge_mode_Flag = 1;
	}
	else if(SysInfo.Batt_Value.Power_Display_Flag) //电量查询
	{
		dealwith_wake_up_mode();
	}
	else if(System_ON == SysInfo.Power_Value.state || System_SHORT_CIRCUIT_PROTECT == SysInfo.Power_Value.state) //档位指示�?
	{
		dealwith_system_work_mode();  // 正常工作模式
	}
	else if(!SysInfo.Batt_Value.Usb_flag && charge_mode_Flag) //不充电模�? 工作状�?�下，显示档位灯
	{
		if(Led_Value.StayTime)
		{
			Led_Value.StayTime --;
			Led_Value.Mode = WAKE_UP_DISP;
		}
		else
		{
			Led_Value.Mode = SLEEP_MODE_DISP;
			charge_mode_Flag = 0;
		}
	}
	Led_Display(&Led_Value);
}
/**************************************************************************************
* FunctionName   : System_Data_Save(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void System_Data_Save(void)
{
	static uint8_t SaveDelay_Cnt;

	if(SysInfo.Save_Data.save_Data_flag && !SysInfo.Montor_Flag)
	{
		if(++SaveDelay_Cnt>=2)
		{
			Write_Parameter_To_Eeprom(); //关机保存档位
			SysInfo.Save_Data.save_Data_flag =0;
		}
	}
	else
	{
		SaveDelay_Cnt = 0;
	}
}

/**************************************************************************************
* FunctionName   : RF_Call_Data(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void RF_Call_Data(void)
{
 /********** 	关闭EMS相关函数************/
//	Ems_Osc_Off();
	Ems_Boost_Off();
	EMS_Pwm_Stop();

 /********** 	关闭RF相关函数************/
	RF_Osc_Off();
	RF_Stop();
//	VBAT_OUT_OFF();
	Set_Mbi5020_Out(CLOSE_RF_EMS_OUT_BIT);
}

/**************************************************************************************
* FunctionName   : System_Standby_Run(void)
* Description    : 待机状�?�或离开�?肤，60s后自动触发关机，�?机�??�?次接触到�?肤，�?达振�?
* EntryParameter : None
* ReturnValue    : None  10ms
**************************************************************************************/
void System_Standby_Check(void)
{
	if((AGING_TEST == SysInfo.Test_Mode.STATUS) || (INSET_LIFE_TEST == SysInfo.Test_Mode.STATUS))
	{
		return;										// �ϻ����Բ���ģʽ�����Զ��ػ�
	}
	if(1 == SysInfo.Batt_Value.Usb_flag)
	{
		return;								// USB���룬���Զ��ػ�
	}
	if(SysInfo.Power_Value.state == System_SHORT_CIRCUIT_PROTECT)
	{
		return;							// ��·���������Զ��ػ�
	}
	if(0 == SysInfo.Sleep_Flag)      // 2023 ,�����ж�������������дﵽ Ԥ��ʱ�����Ͳ����жϿ���״̬
	{
		SysInfo.Sleep_Flag = Check_Idle_Time();
	}

}
/**************************************************************************************
* FunctionName   : Vibration_Reminder_Counts_Run(void)
* Description    : 震动提醒计时
1、紧致提拉模式：换区提醒�?120s�?240s;模式运�?�完�?:300s，进入待机模式，60s无操作，进入休眠模式�?
2、淡纹修复模式：换区提醒�?240s�?480s;模式运�?�完�?:600s，进入待机模式，60s无操作，进入休眠模式�?
* EntryParameter : None
* ReturnValue    : None   10ms 运行一次
**************************************************************************************/
void Vibration_Reminder_Counts_Run(void) //10ms运�?�一�?
{
	static uint8_t  LockFlag;
//	static uint8_t Error_Time_Flag;
	static uint16_t StandyCnt;

    if(SysInfo.Test_Mode.RF_Flag ||SysInfo.Test_Mode.EMS_Flag)//测试模式
	{
		SysInfo.Skin_Touch_Flag  = 1;
		SysInfo.MotionStateFlage =1;
	}

	if(SysInfo.Skin_Touch_Flag)
	{
		if(RF_Handle.Run_Flag) //RF运�?��?�时
		{
			SysInfo.Reminder_Cnt ++;
			if(++SysInfo.Mode_Cnt >= Timer_RF) //1s
			{
				if(SysInfo.WorkState == upkeep_mode)
				{
					SysInfo.Mode_Switch_Flag = 0x02 ; //EMS模式�?�?
				}
				else
				{
					SysInfo.Mode_Switch_Flag = 0x01 ; //RF模式�?�?
				}

				SysInfo.Mode_Cnt = 0;
				LockFlag = 0x01;
			}
		}

		if(EMS_Handle.Run_Flag) //EMS运�?��?�时
		{
			SysInfo.Mode_Switch_Flag = 0x02 ; //EMS模式�?�?
			SysInfo.Reminder_Cnt ++;
			if(++SysInfo.Mode_Cnt >= Timer_EMS) //2s
			{
				SysInfo.Mode_Cnt = 0;
				LockFlag |= 0x02;
				SysInfo.Mode_Switch_Flag = 0x01; //RF模式�?�?
			}
//			Error_Time_Flag = 1;
		}
	}
	else
	{
		SysInfo.Mode_Switch_Flag = 0x01 ; //RF模式�?�?
		SysInfo.Mode_Cnt = 0;
		LockFlag=0;
		// if(!SysInfo.System_Err_status)
		// {
		// 	State_led_scan(BLACK); //离开�?肤，状�?�灯�?
		// }
	}
/************************************************************************************************
1、紧致提拉模式：换区提醒�?120s�?240s;模式运�?�完�?:300s，进入待机模式，60s无操作，进入休眠模式�?
2、淡纹修复模式：换区提醒�?240s�?480s;模式运�?�完�?:600s，进入待机模式，60s无操作，进入休眠模式�?
*************************************************************************************************/

	if( (SysInfo.WorkState == upkeep_mode) || (SysInfo.WorkState == repair_mode) )
	{
		LockFlag =0;
//		SysInfo.Reminder_Cnt++;

		if(((SysInfo.Reminder_Cnt == EMS_Reminder_Left   || SysInfo.Reminder_Cnt == EMS_Reminder_Right)&&SysInfo.WorkState == upkeep_mode)||
			((SysInfo.Reminder_Cnt == RF_Reminder_Left || SysInfo.Reminder_Cnt == RF_Reminder_Right)&&SysInfo.WorkState == repair_mode))
		{
			SysInfo.Montor_Flag = 1; //换区、马达振�?
			SysInfo.StayTime = 20;	 //�?动时�?0.2s
			SysInfo.Freq_Cnt= 0;	 //换区变�??
		}
		else
		{
			if((SysInfo.Reminder_Cnt  >= EMS_Reminder_Forehead  && SysInfo.WorkState == upkeep_mode)||
				(SysInfo.Reminder_Cnt >= RF_Reminder_Forehead && SysInfo.WorkState == repair_mode))
			{
				SysInfo.Reminder_Cnt = 0;
				SysInfo.Montor_Flag = 1; //�?达振动标志位
				SysInfo.StayTime = 40;	 //�?动时�?0.4s
//				SysInfo.Mode_Switch_Flag = 0x03 ;
//				Error_Time_Flag =0;
				SysInfo.Mode_Switch_Flag = 0x00 ;
				SysInfo.Sleep_Flag = 1;  //关机标志�?
				SysInfo.Test_Mode.Test_Mode_Flag = OFF;
				// printf (" enter sleep flag 1 ========\n\r");
			}
		}
	}

	// if(SysInfo.Mode_Switch_Flag == 0x03)
	// {
	// 	// if(++StandyCnt>5)
	// 	{
	// 		SysInfo.Mode_Switch_Flag = 0x00 ;
	// 		SysInfo.Sleep_Flag = 1;  //关机标志�?
	// 		//SysInfo.Test_Mode.Test_Mode_Flag = 0;
	//
	// 	}

	// 	// if(SysInfo.Test_Mode.Ageing_Mode==0x01)
	// 	// {
	// 	// 	StandyCnt = 0 ;
	// 	// 	SysInfo.Mode_Switch_Flag = 0x01 ;
	// 	// 	SysInfo.Sleep_Flag = 0 ;  //关机标志�?
	// 	// 	printf (" exit sleep flag 1 ========\n\r");
	// 	// }
	// }
	// else
	// {
	// 	StandyCnt = 0;
	// }
}
/**************************************************************************************
* FunctionName   : Set_OverTemp_Parameter(void)
* Description    : 过温参数设置
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Set_OverTemp_Parameter(void)
{
	static uint16_t  TimeCnt_30s;

	if(SysInfo.StayTime_30s_Flag)  //切换档位�?,在�?�档位维�?30s,避免因温度过高，导致切换档位�?直维�?1档运�? 主�?�针对EMS
	{
		if(++TimeCnt_30s>3000)
		{
			TimeCnt_30s=0;
			SysInfo.StayTime_30s_Flag =0;
		}
	}

	if(!SysInfo.OverTemp_Flag && SysInfo.WorkState == repair_mode)    //过温后恢复�?�常，参数恢复�?�常  淡纹�?�?
	{
		SysInfo.repair_level = SysInfo.Save_Data.repair_level ;
	}
	else if(SysInfo.OverTemp_Flag && SysInfo.WorkState == repair_mode)//过温参数设置
	{
		SysInfo.repair_level = Level_None ;
	}

	if(!SysInfo.OverTemp_Flag && SysInfo.WorkState == upkeep_mode)    //过温后恢复�?�常，参数恢复�?�常  紧致提拉
	{
		SysInfo.upkeep_level = SysInfo.Save_Data.upkeep_level ;
	}
	else if(SysInfo.OverTemp_Flag && SysInfo.WorkState == upkeep_mode)//过温参数设置
	{
		SysInfo.upkeep_level = Level_None ;
	}

	// if(SysInfo.Test_Mode.Ageing_Flag && SysInfo.Test_Mode.Ageing_Mode!=0x03)
	// if(SysInfo.Test_Mode.Ageing_Flag)
	// {
	// 	SysInfo.upkeep_level = Level_3;
	// }
}
/**************************************************************************************
* FunctionName   : Cosmetic_Procedure_Run(void)
* Description    : 故障处理函数
* EntryParameter : None
* ReturnValue    : None  100ms ZHI
**************************************************************************************/
void System_Error_Deal(void)
{
	static uint8_t OverTemp_Lock_Flag;
//	static uint8_t LED_UP_Flag;
	static uint8_t LowPower_Cnt,SystemError_DelayCnt;
	static uint8_t Ntc_Err_Cnt,Temp_high_Err_cnt;
	static uint16_t Err_to_sleep_delay = 0;

	if(SysInfo.Batt_Value.State <= BAT_00_20_STATUS  && SysInfo.Power_Value.state == System_ON )
	{
		SystemError_DelayCnt ++;
		if(SystemError_DelayCnt >= 40)  //切换档位�?,在�?�档位维�?30s,避免因温度过高，导致切换档位�?直维�?1档运�? 主�?�针对EMS
		{
			SysInfo.System_Err_status = System_LowPower_Fault;	 //低电故障提示
			RF_DeInit();
			Ems_DeInit();
			SystemError_DelayCnt = 0;
		}
	}
	else
	{
		SystemError_DelayCnt = 0;
	}

// //=�⹤ �汾�������߼�
	if(!SysInfo.System_Err_status)  //过温后恢复�?�常，参数恢复�?�常
	{
		if(SysInfo.Power_Value.state == System_ON && OverTemp_Lock_Flag)
		{
			SysInfo.Mode_Switch_Flag = 0x01;
			OverTemp_Lock_Flag = 0x00 ;
		}

		LowPower_Cnt = 0x00;
		Err_to_sleep_delay = 0;
	}
	else if(SysInfo.System_Err_status == System_OverTemp_Fault || SysInfo.System_Err_status == System_NTC_Fault)//过温或传感器故障
	{
		LowPower_Cnt = 0x00;
		if(SysInfo.Power_Value.state == System_ON) //模式�?提示 过温
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			RF_Call_Data();
			IRled_stop();		//关闭隧道�?
			RF_DeInit();
			Ems_DeInit();
			// PID.Flag=0;
			OverTemp_Lock_Flag	= 0x01 ;

		}
		Err_to_sleep_delay ++;
		if(Err_to_sleep_delay == Leave_Skin_Timer/10)   // Leave_Skin_Timer/10
		{
			SysInfo.Sleep_Flag = 1;  //关机标志�?
			Err_to_sleep_delay = 0;
		}
		else ;
	}
	else if(SysInfo.System_Err_status == System_LowPower_Fault) //电量�?10%~20%范围内，如果此提示在2分钟内还没有解除，则进入待机状�??
	{
		if(++LowPower_Cnt>50)//1100)
		{
			LowPower_Cnt = 0x00;
			SysInfo.Mode_Switch_Flag = 0x00;//SysInfo.Mode_Switch_Flag = 0x03;
			OverTemp_Lock_Flag	= 0x01 ;

			RF_Call_Data();
			IRled_stop();		//关闭隧道�?
			RF_DeInit();
			Ems_DeInit();
			Ems_Boost_Off();

			// PID.Flag=0;
			SysInfo.Power_Value.state = System_Standy ;
			SysInfo.Sleep_Flag = 1;  //关机标志�?
		}
		Err_to_sleep_delay = 0;
	}
	else
	{
		OverTemp_Lock_Flag	= 0x01;
	}
// //=====


// -----�Լ��޸ĵ���ʱ����
	// switch(SysInfo.System_Err_status)
	// {
	// 	case System_Normal:
	// 	{
	// 		// if(SysInfo.Power_Value.state == System_ON && OverTemp_Lock_Flag)
	// 		// {
	// 		// 	SysInfo.Mode_Switch_Flag = 0x01;
	// 		// 	OverTemp_Lock_Flag = 0x00 ;
	// 		// }
	// 		LowPower_Cnt = 0x00;
	// 		Temp_high_Err_cnt = 0;
	// 		Ntc_Err_Cnt = 0;
	// 		break;
	// 	}

	// 	case System_OverTemp_Fault:
	// 	case System_NTC_Fault:
	// 	{
	// 		LowPower_Cnt = 0x00;
	// 		if(SysInfo.Power_Value.state == System_ON) //模式�?提示 过温
	// 		{
	// 			SysInfo.Mode_Switch_Flag = 0x00;
	// 			RF_Call_Data();
	// 			IRled_stop();		//关闭隧道�?
	// 			RF_DeInit();
	// 			Ems_DeInit();
	// 			PID.Flag=0;
	// 			OverTemp_Lock_Flag	= 0x01 ;

	// 			Ntc_Err_Cnt++;
	// 			if(Ntc_Err_Cnt > 50)
	// 			{
	// 				Ntc_Err_Cnt = 0;
	// 				SysInfo.Power_Value.state = System_Standy ;
	// 				SysInfo.Sleep_Flag = 1;  //关机标志�?
	// 			}
	// 		}
	// 		break;
	// 	}

	// 	case System_LowPower_Fault:
	// 	{
	// 		LowPower_Cnt ++;
	// 		if(LowPower_Cnt > 50)//1100)
	// 		{
	// 			LowPower_Cnt = 0x00;
	// 			SysInfo.Mode_Switch_Flag = 0x00;//SysInfo.Mode_Switch_Flag = 0x03;
	// 			OverTemp_Lock_Flag	= 0x01 ;

	// 			RF_Call_Data();
	// 			IRled_stop();		//关闭隧道�?
	// 			RF_DeInit();
	// 			Ems_DeInit();
	// 			Ems_Boost_Off();

	// 			PID.Flag=0;
	// 			SysInfo.Power_Value.state = System_Standy ;
	// 			SysInfo.Sleep_Flag = 1;  //关机标志�?
	// 		}
	// 		break;
	// 	}

	// }

//---------------

}
/*************************************************************************************
* FunctionName	: SLIDE_MODE_Run
* Description   : 滑动模式
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void SLIDE_MODE_Run(void)
{
	static uint8_t time_cnt = 0;

//	if(SysInfo.Power_Value.state == System_ON)
//	{
////		printf ("\n\r***************start*******************\n\r");
//		SysInfo.MotionStateFlage = AttitudeProcessing();    //6轴加速度
////		printf ("\n\r***************end*******************\n\r");
//
//			SysInfo.MotionStateFlage = 0x01; //测试
//	}

//    if(SysInfo.Test_Mode.RF_Flag ||SysInfo.Test_Mode.EMS_Flag)//测试模式
//	{
//		SysInfo.Skin_Touch_Flag  =1;
//		SysInfo.MotionStateFlage =1;
//	}

	if(SysInfo.MotionStateFlage)
	{
		time_cnt = 0;
	}
	else
	{
		time_cnt++;
		if(time_cnt>5)//20) //主机维持输出0.2s，之后停止输出，主机重新移动时，RF再恢复输�?
		{
			time_cnt = 5;//20;
			RF_DeInit();
			Ems_DeInit();
			SysInfo.Mode_Switch_Flag = 0x01 ; //RF模式�?�?
			//State_led_scan(BLACK); //离开�?肤，状�?�灯灭D
			SysInfo.Mode_Cnt = 0;
		}
		else
		{
			if(RF_Handle.Run_Flag ||EMS_Handle.Run_Flag)
			{
				SysInfo.Reminder_Cnt++; //用户使用时间计时
			}
		}
	}
}
/**************************************************************************************
* FunctionName   : Cosmetic_Procedure_Run(void)
* Description    : SysInfo.Montor_Flag:�?达振动标志位
				   SysInfo.StayTime：振动时�?
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void RF_Procedure_Run(void)
{
	if(SysInfo.WorkState == repair_mode && SysInfo.Power_Value.state == System_ON) //淡纹�?复模�?
	{

		if(SysInfo.Mode_Switch_Flag == 0x01 && SysInfo.MotionStateFlage && SysInfo.System_Err_status == System_Normal) //RF初�?�化
		{
			SysInfo.Mode_Switch_Flag = 0x00;
//			Ems_DeInit();
			if(!SysInfo.OverTemp_Flag)
				RF_Init();
		}
		else if(SysInfo.Mode_Switch_Flag == 0x03) //进入待机模式
		{
			RF_Call_Data();
			IRled_stop();		//关闭隧道�?
			RF_DeInit();
			Ems_DeInit();
//			VBAT_OUT_OFF();
			// PID.Flag=0;
			SysInfo.Power_Value.state = System_Standy ;
		}
	}
}
/**************************************************************************************
* FunctionName   : EMS_Procedure_Run(void)
* Description    : 1s RF �? 2S EMS
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void EMS_Procedure_Run(void)
{

	if(SysInfo.Power_Value.state == System_SHORT_CIRCUIT_PROTECT)   // �?�?保护状�?�不进�?�输出ems
	{
		return;
	}

	if(SysInfo.WorkState == upkeep_mode && SysInfo.Power_Value.state == System_ON)
	{

		if(SysInfo.Mode_Switch_Flag == 0x01 && SysInfo.MotionStateFlage && SysInfo.System_Err_status == System_Normal)      //RF初�?�化
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			Ems_DeInit();
			Ems_Boost_On();
			if(!SysInfo.Test_Mode.EMS_Flag)
			{
				if(!SysInfo.OverTemp_Flag)
					RF_Init();
			}
			else
			{
				RF_Handle.Run_Flag=1;
			}
		}
		else if(SysInfo.Mode_Switch_Flag == 0x02 && SysInfo.MotionStateFlage) //EMS初�?�化
		{
			RF_DeInit();

			if(!SysInfo.Test_Mode.RF_Flag)
			{
				Ems_Boost_On();
				Ems_Init();

			}
			else
			{
				EMS_Handle.Run_Flag = 1;
			}
		}
		else if(SysInfo.Mode_Switch_Flag == 0x03) //进入待机模式
		{
			RF_Call_Data();
			IRled_stop();		//关闭隧道�?
			RF_DeInit();
			Ems_DeInit();
			Ems_Boost_Off();
//			VBAT_OUT_OFF();
			// PID.Flag=0;
			SysInfo.Power_Value.state = System_Standy ;
		}
		else ;
	}
}
/**************************************************************************************
* FunctionName   : System_Sleep(void)
* Description    : 100ms运�??1�?
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void System_Sleep(void)
{
	static uint8_t Sleep_Flag;
//	static uint16_t Sleep_Counts;

	if(SysInfo.Sleep_Flag && !SysInfo.Test_Mode.Test_Mode_Flag)
	{
		//State_led_scan(false);
		//Level_led_scan(BLACK);
		//Mode_led_scan(SysInfo.WorkState);
		Dealwith_power_key_long();
		SysInfo.Sleep_Flag = 0;
		Key_Call_Data();
		Sleep_Flag = 0;
		SysInfo.Power_Value.state = System_OFF;
		SysInfo.Power_Value.Enter_Sleep_Flag = 1;
		SysInfo.Sleep_Counts =0;
	}

//	if(((SysInfo.Power_Value.state == System_OFF ) && (!SysInfo.Batt_Value.Usb_flag)))  //进入低功耗�?�时�?10S
	if((SysInfo.Power_Value.Enter_Sleep_Flag) && (!SysInfo.Batt_Value.Usb_flag))
	{
		if(++ SysInfo.Sleep_Counts > 100) // 10s ֮�����sleepģʽ
		{
			SysInfo.Sleep_Counts = 0;
			Sleep_Flag = 1;
			Led_Value.Counts = 0;
		}
	}
	else
	{
		SysInfo.Sleep_Counts =0;
	}

	if(Sleep_Flag == 1 || SysInfo.Test_Mode.Sleep_Flag)
	{
		Sleep_Flag = 0;
		SysInfo.Test_Mode.Sleep_Flag = 0;
		printf ("\n\r SLEEP\n\r");
		/****************进入休眠模式****************/
		Enter_Sleep_Mode();
		exit_sleep_mode();
	}

}
/**************************************************************************************
* FunctionName   : void Rx_Init(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/

void Rx_Init(void)
{
// 	GPIO_InitTypeDef GPIO_InitStruct;
// 	__HAL_RCC_GPIOA_CLK_ENABLE();
//   // 配置UART1的Rx引脚为EXIT模式
//   GPIO_InitStruct.Pin = GPIO_PIN_10;
//   GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // 特别注意这里要使用中�?模式
//   GPIO_InitStruct.Pull = GPIO_PULLUP;
//   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//   HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
//   HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}


/* USER CODE BEGIN 2 */
void MX_GPIO_DISInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();


                  // 未关闭串口 和 swd，clk 时，待机功耗约为 100uA
  	HAL_GPIO_DeInit(GPIOA, UART_TX_Pin | UART_RX_Pin);
  	HAL_GPIO_DeInit(GPIOA, SW_DIO_Pin | SW_CLK_Pin);  // 烧录口关闭

   	LED_IR_OFF();
	MOTOR_OFF();
    HW_CE_LOW();

	VBAT_OUT_OFF();

	BOOST_5V_OFF();
	BOOST_3V3_OFF();
	RF_CHANNEL_OFF();

	GEAR2_DOWN();
	GEAR3_DOWN();
	GEAR4_DOWN();
	HW_CSN_LOW();
	EMS_CHANNEL_OFF();
	EMS_PWR_EN_OFF();
	RF_EN_DOWN();

}


void adc_param_self_check(void)
{
	int i, sum_offset = 0;

	dac_param.self_cal = HAL_ADCEx_Calibration_GetValue(&hadc1);

	if(dac_param.self_cal != 0x7F)
	{
		dac_param.offset = 0;
		return;
	}


	for(i = 0; i < ADC_ADD_COUNT; i++)
	{

		sum_offset +=  ADC_Value[i][Skin_ADC_CHANNEL];

	}

	dac_param.offset = sum_offset/24;

}


/**************************************************************************************
* FunctionName   : Enter_Sleep_Mode(void)
* Description    : 进入休眠模式
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Enter_Sleep_Mode(void)
{
	Ems_DeInit();
	RF_DeInit();
	adc_param_self_check();


	BOOST_5V_OFF();
	BOOST_3V3_OFF();
	VBAT_OUT_OFF();
	BAT_OUT_DISABLE();
//	Upkeep_LED_OFF();
//	EMS_LED_DOWN();
//	Repair_LED_OFF();
	SysInfo.Test_Mode.Test_Mode_Flag = OFF;
	SysInfo.Test_Mode.STATUS =0x00;
	SysInfo.Batt_Value.BattCount = 0;
	SysInfo.repair_level=0;
	SysInfo.upkeep_level=0;
	SysInfo.WorkState=0;
	SysInfo.System_Err_status = System_Normal;
	SysInfo.OverTemp_Flag = 0; //恢�?��?�常
#ifdef ENABLE_IWDG_FUNC
	HAL_IWDG_Refresh(&hiwdg);
#endif
	HAL_ADC_Stop_IT(&hadc1);   //ADC�?换启�?
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_UART_DMAStop(&huart2);
	HAL_UART_MspDeInit(&huart2);
	HAL_SPI_DMAStop(&hspi1);
//	HAL_ADC_MspDeInit(&hadc1);
	HAL_SPI_MspDeInit(&hspi1);
	HAL_SPI_MspDeInit(&hspi2);
	HAL_I2C_MspDeInit(&hi2c1);
	HAL_I2C_MspDeInit(&hi2c2);
	HAL_TIM_Base_MspDeInit(&htim1);
	HAL_TIM_Base_MspDeInit(&htim17);

//	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
	EMS_Pwm_Stop();
	RF_Osc_Off();
	MX_GPIO_DISInit();

//	SysTick->CTRL = 0x00;//关闭定时�?
//	SysTick->VAL = 0x00;//清空val,清空定时�?

	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn); //使能POWER_KEY_PIN�?�?
//	Rx_Init();
	///睡眠
	__HAL_RCC_PWR_CLK_ENABLE();   //使能PWR时钟
	  /* 进入停�?�模式，设置电压调节器为低功耗模式，等待�?�?唤醒 KEY2 按键下降沿唤�?*/
//	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);

	HAL_PWREx_EnableFlashPowerDown(PWR_FLASHPD_STOP);
	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	HAL_PWREx_DisableFlashPowerDown(PWR_FLASHPD_STOP);
	HAL_ResumeTick();
}
/**************************************************************************************
* FunctionName   : Enter_Sleep_Mode(void)
* Description    : �?出睡眠模�?
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void exit_sleep_mode(void)
{
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
#ifdef ENABLE_IWDG_FUNC
	HAL_IWDG_Refresh(&hiwdg);
#endif
    SystemClock_Config();
	MX_GPIO_Init();
//	MX_ADC1_Init();
//	MX_SPI1_Init();
//	MX_I2C2_Init();
//	MX_USART2_UART_Init();
//	MX_I2C1_Init();
//	MX_SPI2_Init();

	HAL_SPI_MspInit(&hspi1);
	HAL_SPI_MspInit(&hspi2);
	HAL_I2C_MspInit(&hi2c1);
	HAL_I2C_MspInit(&hi2c2);
	HAL_TIM_Base_MspInit(&htim1);
	HAL_TIM_Base_MspInit(&htim17);

	// BOOST_5V_ON();

	SysInfo.Power_Value.Enter_Sleep_Flag =1;
	SysInfo.Batt_Value.Usb_flag = 0;
//	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);     //关闭外部�?断POWER_KEY_PIN使能
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);

	SysInfo.Power_Value.BattState = 0x01 ; //�?机电池电压�??测标�?
	Read_Current_Level_For_Eeprom();
	BAT_OUT_ENABLE();
	BOOST_3V3_ON();
	Led_Value.Counts=0;
	HAL_ADCEx_Calibration_Start(&hadc1);
	ADC_DMA_COV_START();

    MX_TIM1_Init();
	HAL_UART_MspInit(&huart2);
	MX_USART2_UART_Init();
    MX_I2C1_Init();


	SysInfo.Test_Mode.Test_Mode_Flag = OFF;
	RGB_Init();
	BOOST_5V_OFF();
	BOOST_3V3_OFF();

}

/**************************************************************************************
* FunctionName   : System_Data_Init(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void System_Data_Init(void)
{

   	Sys_IWDG_STOP_Mode();
	BAT_OUT_ENABLE();
	HAL_ADCEx_Calibration_Start(&hadc1);
	ADC_DMA_COV_START();
	IRled_stop();

	SysInfo.WorkState= Standy_mode;
	SysInfo.Batt_Value.State=BAT_40_60_STATUS;
	SysInfo.Power_Value.Enter_Sleep_Flag=1;
//	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn); //关闭外部�?�?使能
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	//Led_Value.Level =0x5a;
	Read_Current_Level_For_Eeprom();
	// BOOST_5V_ON();
	// PID.Kp = 1.5;
	// PID.Ki = 0.8;
	// PID.Kd = 0.1;
	// PID.Constant=50;
	// PID.temp = 500;

	GY6550_INIT();   //�?螺仪初�?�化
	RGB_Init();
	hw2000b_init_250k();

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


