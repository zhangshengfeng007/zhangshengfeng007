#include "includes.h"

// #ifdef __GNUC__

//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

// #else

//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

// #endif /* __GNUC__ */

// int fputc(int ch,FILE *f)
//{
//	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 50);
//	return ch;
// }

// 加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
	while ((USART1->ISR & 0X40) == 0)
		; // 循环发送,直到发送完毕
	USART1->TDR = (uint8_t)ch;
	return ch;
//HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ch, 1);
//return ch;
}
#endif

/**************************************************************************************
 * FunctionName   : Led_Process_Run(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Led_Process_Run(void)
{
	static uint8_t Flag, Cnt = 0;
	static uint8_t Batt_State = 1, Batt_Cnt;
	static uint16_t ChargeCnt = 0;

	if (SysInfo.Batt_Value.Usb_flag) // 充电模式
	{
		if (++ChargeCnt >= 50)
		{
			if (!Flag) // USB接入复位，检测一次电量
			{
				Led_Value.Level = SysInfo.Save_Data.BattState;
				BOOST_5V_OFF();
			}
			Flag = 1;
			Led_Value.StayTime = 50;
			Led_Value.Mode = Batt_Charging;
			if (ChargeCnt > 6000) // 60s刷新一次
			{
				if (SysInfo.Batt_Value.State >= Led_Value.Level + 1)
				{
					Led_Value.Level++;
					SysInfo.Save_Data.BattState = Led_Value.Level;
				}
				else
				{
					Led_Value.Level = SysInfo.Save_Data.BattState;
				}

				ChargeCnt = 0;
			}
		}
		Batt_Cnt = 0;
		Cnt = 0;
	}
	else
	{
		if (SysInfo.Batt_Value.Power_Display_Flag) // 查询电量
		{
			BAT_ADC_ENABLE();
			if (++Cnt >= 26)
			{
				SysInfo.Batt_Value.Power_Display_Flag = 0;
				Cnt = 0;
				SysInfo.Save_Data.BattState = *(__IO uint32_t *)(EEPROM_STRAT_ADDR + 128); // 读取关机前的电量
				if (SysInfo.Save_Data.BattState == BAT_00_00_STATUS)
				{
					Led_Value.Level = 0x01;
				}
				else
				{
					Led_Value.Level = SysInfo.Batt_Value.State;
				}

				Led_Value.Mode = Batt_Normal;
				Led_Value.StayTime = 200;
				Led_Value.state = 1;
				SysInfo.Power_Value.BattState = 0;
			}
		}
		else if (SysInfo.Power_Value.state == System_ON || SysInfo.Flag) // 档位指示灯
		{
			SysInfo.Flag = 0;
			if (SysInfo.WorkState == repair_mode)
			{
				Led_Value.Level = SysInfo.Save_Data.repair_level;
			}
			else if (SysInfo.WorkState == upkeep_mode)
			{
				Led_Value.Level = SysInfo.Save_Data.upkeep_level;
			}
			else
			{
				Led_Value.Level = 0x5a;
			}
			Led_Value.StayTime = 0;
			if (SysInfo.Batt_Value.State == BAT_00_20_STATUS)
			{
				Led_Value.Mode = Batt_Low_20;
			}
			//			else if(SysInfo.Batt_Value.State==BAT_00_00_STATUS)
			//			{
			//				Led_Value.Mode = Batt_Low_0 ;
			//			}
			else
			{
				Led_Value.Mode = Batt_Normal;
			}
		}
		else if (!SysInfo.Batt_Value.Usb_flag && Flag) // 不充电模式 工作状态下，显示档位灯
		{
			Flag = 0;
			Led_Value.StayTime = 100;
			Led_Value.Mode = Batt_Normal;
			Led_Value.state = 1;
		}
		else if (SysInfo.Power_Value.BattState) // 开机查询电量 电量缺电不开机
		{
			Batt_Cnt++;
			if (Batt_Cnt > 13)
			{
				Batt_Cnt = 0;
				//				SysInfo.Save_Data.BattState  = *(__IO uint32_t*)( EEPROM_STRAT_ADDR+128 );		//读取关机前的电量
				Batt_State = SysInfo.Batt_Value.State;
				if ((SysInfo.Save_Data.BattState == BAT_00_00_STATUS) || (Batt_State == BAT_00_00_STATUS)) // 缺电，关机
				{
					//					Led_Value.Mode = Batt_Low_0 ; //缺电，1档LED灯1Hz频率闪烁5次
					//					Led_Value.Counts =0;
					if (Batt_State == BAT_00_00_STATUS)
					{
						SysInfo.Save_Data.BattState = BAT_00_00_STATUS;
					}
				}
				SysInfo.Power_Value.BattState = 0;
			}
		}
		else
		{
			Batt_Cnt = 0;
			Cnt = 0;
		}
		ChargeCnt = 0;
	}

	if (SysInfo.Test_Mode.Test_Mode_Flag == OFF)
	{
		Led_Display(&Led_Value);
	}
}
/**************************************************************************************
 * FunctionName   : System_Data_Save(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void System_Data_Save(void)
{
	if (SysInfo.Save_Data.save_Data_flag && !SysInfo.Montor_Flag)
	{
		SysInfo.Save_Data.save_Data_flag = 0;
		Write_Parameter_To_Eeprom(); // 关机保存档位
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
	/**********关闭EMS相关函数************/
	//	Ems_Osc_Off();
	Ems_Boost_Off();
	EMS_Pwm_Stop();

	/**********关闭RF相关函数************/
	RF_Osc_Off();
	RF_Stop();
	//	VBAT_OUT_OFF();
	Set_Mbi5020_Out(0);
}
/**************************************************************************************
 * FunctionName   : System_Standby_Run(void)
 * Description    : 待机状态或离开皮肤，60s后自动触发关机，开机第一次接触到皮肤，马达振动
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void System_Standby_Run(void)
{
	uint8_t Get_60s_Flag = 0;
	static uint8_t Batt_Cnt;
	Get_60s_Flag = Cosmetic_Time_Count();
	if (Get_60s_Flag) // 60s无接触，关机
	{
		Get_60s_Flag = 0;
		SysInfo.Sleep_Flag = 1; // 关机标志位
		//			Write_Parameter_To_Eeprom();  //关机保存档位
	}
	if (SysInfo.Batt_Value.State == BAT_00_00_STATUS && SysInfo.Power_Value.state == System_ON) // 缺电，关机
	{
		if (++Batt_Cnt > 20)
		{
			SysInfo.Sleep_Flag = 1; // 关机标志位
			Batt_Cnt = 0;
			SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;
		}
	}
	else
	{
		Batt_Cnt = 0;
	}
	//		if(SysInfo.Skin_Touch_Montor_Flag && SysInfo.Skin_Touch_Flag) //开机第一次接触到皮肤
	//		{
	//			SysInfo.Skin_Touch_Montor_Flag = 0;
	//			SysInfo.Montor_Flag = 1; //换区、马达振动
	//			SysInfo.StayTime = 20;	 //振动时长0.2s
	//		}
}
/**************************************************************************************
* FunctionName   : Vibration_Reminder_Counts_Run(void)
* Description    : 震动提醒计时
1、紧致提拉模式：换区提醒：120s、240s;模式运行完成:300s，进入待机模式，60s无操作，进入休眠模式；
2、淡纹修复模式：换区提醒：240s、480s;模式运行完成:600s，进入待机模式，60s无操作，进入休眠模式；
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Vibration_Reminder_Counts_Run(void) // 10ms运行一次
{
	static uint8_t LockFlag, Error_Time_Flag;
	static uint16_t StandyCnt,NoTouch_Cnt;

	if (SysInfo.Skin_Touch_Flag)
	{
		NoTouch_Cnt = 0;
		if (RF_Handle.Run_Flag) // RF运行计时
		{
			if (++SysInfo.Mode_Cnt >= Timer_RF) // 1s
			{
				SysInfo.Mode_Cnt = 0;
				LockFlag = 0x01;

				if (SysInfo.WorkState == upkeep_mode)
				{
					SysInfo.Mode_Switch_Flag = 0x02; // EMS模式开启
				}
				else
				{
					SysInfo.Mode_Switch_Flag = 0x01; // RF模式开启
				}
			}
		}
#if ARF001
		if (EMS_Handle.Run_Flag) // EMS运行计时
		{
			if (++SysInfo.Mode_Cnt >= Timer_EMS) // 2s
			{
				SysInfo.Mode_Cnt = 0;
				LockFlag |= 0x02;
				SysInfo.Mode_Switch_Flag = 0x01; // RF模式开启
			}
			Error_Time_Flag = 1;
		}
#else
		if (LockFlag == 0x01 && EMS_Handle.Run_Flag)
		{
			LockFlag |= 0x02;
		}
#endif
	}
	else
	{
		SysInfo.Mode_Switch_Flag = 0x01; // RF模式开启
		SysInfo.Mode_Cnt = 0;
		LockFlag = 0;
    if(++NoTouch_Cnt > 1000 )
		{
			NoTouch_Cnt = 0;
      if(SysInfo.Power_Value.state == System_ON)
			{
				SysInfo.Temp_Protect_Flage = 1; //开机及离开皮肤10s 过温温度减2℃标志位
			}

		}
	}
	/************************************************************************************************
1、紧致提拉模式：换区提醒：120s、240s;模式运行完成:300s，进入待机模式，60s无操作，进入休眠模式；
2、淡纹修复模式：换区提醒：240s、480s;模式运行完成:600s，进入待机模式，60s无操作，进入休眠模式；
	*************************************************************************************************/

	if ((LockFlag == 0x03 && SysInfo.WorkState == upkeep_mode) ||
		(LockFlag == 0x01 && SysInfo.WorkState == repair_mode))
	{
		LockFlag = 0;
		SysInfo.Reminder_Cnt++;
		SysInfo.Check_Protect_Flage = 1;
//		SysInfo.Period_Flag = 1; 
		if (((SysInfo.Reminder_Cnt == EMS_Reminder_120S || SysInfo.Reminder_Cnt == EMS_Reminder_240S) && SysInfo.WorkState == upkeep_mode) ||
			((SysInfo.Reminder_Cnt == RF_Reminder_240S || SysInfo.Reminder_Cnt == RF_Reminder_480S) && SysInfo.WorkState == repair_mode))
		{
			SysInfo.Montor_Flag = 1; // 换区、马达振动
			SysInfo.StayTime = 20;	 // 振动时长0.2s
		}
		else
		{
			if ((SysInfo.Reminder_Cnt == EMS_Reminder_300S && SysInfo.WorkState == upkeep_mode) ||
				(SysInfo.Reminder_Cnt == RF_Reminder_600S && SysInfo.WorkState == repair_mode))
			{
				SysInfo.Reminder_Cnt = 0;
				SysInfo.Montor_Flag = 1; // 换区、马达振动
				SysInfo.StayTime = 100;	 // 振动时长1.0s
				SysInfo.Mode_Switch_Flag = 0x03;
				Error_Time_Flag = 0;
//				SysInfo.Skin_Touch_Flag = 0; // 系统接触标志位
			}
		}
	}

	if (SysInfo.Mode_Switch_Flag == 0x03)
	{
		if (++StandyCnt > 5)
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			SysInfo.Sleep_Flag = 1; // 关机标志位
		}

		if (SysInfo.Test_Mode.Ageing_Mode == 0x01)
		{
			StandyCnt = 0;
			SysInfo.Mode_Switch_Flag = 0x01;
			SysInfo.Sleep_Flag = 0; // 关机标志位
		}
	}
	else
	{
		StandyCnt = 0;
	}
}
/**************************************************************************************
 * FunctionName   : Set_OverTemp_Parameter(void)
 * Description    :  过温参数设置
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Set_OverTemp_Parameter(void)
{
	static uint16_t TimeCnt_30s, Mode_Cnt;
//  static uint8_t Restore_Flag;
	if (SysInfo.StayTime_30s_Flag) // 切换档位后,在该档位维持30s,避免因温度过高，导致切换档位一直维持1档运行 主要针对EMS
	{
		if (++TimeCnt_30s > 3000)
		{
			TimeCnt_30s = 0;
			SysInfo.StayTime_30s_Flag = 0;
		}
	}

	if (!SysInfo.OverTemp_Flag && !SysInfo.Check_Protect_Flage) // 过温后恢复正常，参数恢复正常
	{
		if (SysInfo.WorkState == upkeep_mode)
		{
			SysInfo.upkeep_level = SysInfo.Save_Data.upkeep_level;
			if (SysInfo.Power_Value.state == System_ON)
			{
				Upkeep_LED_ON();
			}
			else
			{
				if (SysInfo.Test_Mode.Test_Mode_Flag != ON)
				{
					Upkeep_LED_OFF();
				}
			}
		}
		else if (SysInfo.WorkState == repair_mode)
		{
			SysInfo.repair_level = SysInfo.Save_Data.repair_level;
			if (SysInfo.Power_Value.state == System_ON)
			{
				Repair_LED_ON();
			}
			else
			{
				if (SysInfo.Test_Mode.Test_Mode_Flag != ON)
				{
					Repair_LED_OFF();
				}
			}
		}
		else
			;

		if (!SysInfo.Check_Protect_Flage) // 检测到传感器移动
		{
			if (SysInfo.Power_Value.state == System_ON)
			{
				IRled_start();
			}
		}
		Mode_Cnt = 0;
    if(SysInfo.Restore_Flag && SysInfo.Power_Value.state == System_ON)
		{
			SysInfo.Restore_Flag = 0;
			SysInfo.Mode_Switch_Flag = 0x01 ;
		}
	}
	//	else if(SysInfo.OverTemp_Flag && !SysInfo.StayTime_30s_Flag)//过温参数设置
	else if (SysInfo.OverTemp_Flag || SysInfo.Check_Protect_Flage) // 过温参数设置 或 传感器未移动，完成当前周期能量输出后，RF降为1档，EMS不降档，模式灯及隧道灯闪（1S亮灭）
	{
#if TEMP_TEST_MODE
    if(SysInfo.OverTemp_Flag == 0x01 && !SysInfo.Check_Protect_Flage)
		{
				if (SysInfo.WorkState == upkeep_mode) // 紧致提拉模式
				{
					if (SysInfo.Check_Protect_Flage)
					{
						SysInfo.upkeep_level = Level_1;
					}
					else
					{
						SysInfo.upkeep_level = Level_None;
					}
				}
				else if (SysInfo.WorkState == repair_mode)
				{
					if (SysInfo.Check_Protect_Flage)
					{
						SysInfo.repair_level = Level_1;
					}
					else
					{
						SysInfo.repair_level = Level_None;
					}
				}
				else
					;
		}
#if SENSOR_MOVE

		else if(SysInfo.OverTemp_Flag == 0x02 ||SysInfo.Check_Protect_Flage)
#else
		else if(SysInfo.OverTemp_Flag == 0x02)
#endif
		{
			if(SysInfo.Power_Value.state == System_ON)
			{
				RF_DeInit();
				Ems_DeInit();
				SysInfo.Restore_Flag = 1 ;
			}
		}
#else
//		if(SysInfo.OverTemp_Flag == 0x01 ||SysInfo.Check_Protect_Flage )
//		{
			RF_DeInit();
			Ems_DeInit();
      SysInfo.Restore_Flag = 1 ;
//		}


#endif
		if (SysInfo.Power_Value.state == System_ON && SysInfo.Batt_Value.State != BAT_00_20_STATUS) // 模式灯报警提醒
		{
			if(SysInfo.Skin_Touch_Flag)
			{
				Mode_Twink1hz_Display(50);
			}
			else
			{
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
	else
		;

	if (SysInfo.Test_Mode.Ageing_Flag)
	{
		SysInfo.upkeep_level = Level_5;
	}
}

/*************************************************************************************
 * FunctionName	: SLIDE_MODE_Run
 * Description   :滑动模式
 * EntryParameter:
 * ReturnValue   :
 **************************************************************************************/
void SLIDE_MODE_Run(void)
{
		static uint8_t Lis2dInit_Flag = 0;
	//	SysInfo.MotionStateFlage = LIS3DH_get_angle(ddata.AXIS_X, ddata.AXIS_Y, ddata.AXIS_Z);//解析
#if LIS2DH
  if(SysInfo.Power_Value.state==System_ON)
	{
    if(Lis2dInit_Flag)
		{
			Lis2dInit_Flag = 0;
			Lis2dh_Init();
		}
		LIS2DH_Data_Deal();
	}
	else
	{
		Lis2dInit_Flag = 0x01;
	}

#else
	LIS3DH_get_angle(ddata.AXIS_X, ddata.AXIS_Y, ddata.AXIS_Z); // 解析
#endif
	if (SysInfo.Test_Mode.Test_Mode_Flag == ON)
	{
		SysInfo.MotionStateFlage = 1; // 测试
	}
#if AGEING_TEST
	SysInfo.MotionStateFlage = 1;  //????
#endif

	if (SysInfo.MotionStateFlage)
	{
		SysInfo.Check_Protect_Flage = 0;
	}
	else
	{
//		if(SysInfo.Period_Flag)  //正常工作状态下传感器未移动，完成当前周期能量输出后，RF降为1档，EMS不降档，模式灯及隧道灯闪（1S亮灭）
//		{
//			SysInfo.Period_Flag = 0 ;
//			SysInfo.Check_Protect_Flage = 1;
//		}
	}
}
/**************************************************************************************
 * FunctionName   : EMS_Procedure_Run(void)
 * Description    : 1s RF ?? 2S EMS
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void EMS_Procedure_Run(void)
{
#if ARF001

	if (SysInfo.WorkState == upkeep_mode && SysInfo.Power_Value.state == System_ON)
	{
		System_Standby_Run();				  // 60s无接触关机、第一次接触皮肤马达振动
		if (SysInfo.Mode_Switch_Flag == 0x01) // RF初始化
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			Ems_DeInit();
			RF_Init();
		}
		else if (SysInfo.Mode_Switch_Flag == 0x02) // EMS初始化
		{
			RF_DeInit();
			Ems_Init();
		}
		else if (SysInfo.Mode_Switch_Flag == 0x03) // 进入待机模式
		{
			RF_Call_Data();
			IRled_stop(); // 关闭隧道灯
			RF_DeInit();
			Ems_DeInit();
			VBAT_DET_ON(); // 使能 V33V5020
			VBAT_OUT_OFF();
			RF_EN_DOWN();
			PID.Flag = 0;
			SysInfo.Power_Value.state = System_Standy;
		}
		else
			;
	}

#else

	if (SysInfo.WorkState == upkeep_mode && SysInfo.Power_Value.state == System_ON)
	{
		System_Standby_Run();														// 60s无接触关机、第一次接触皮肤马达振动
		if ((SysInfo.Mode_Switch_Flag == 0x01 || SysInfo.Mode_Switch_Flag == 0x02)) // RF、EMS初始化
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			RF_Init();
			if (SysInfo.Skin_Touch_Flag)
			{
				Ems_Init();
			}
		}
		else if (SysInfo.Mode_Switch_Flag == 0x03) // 进入待机模式
		{
			RF_Call_Data();
			IRled_stop();  // 关闭隧道灯
			RF_DeInit();
			Ems_DeInit();
			VBAT_DET_ON(); // 使能V33V5020
			VBAT_OUT_OFF();
			RF_EN_DOWN();
			PID.Flag = 0;
			SysInfo.Power_Value.state = System_Standy;
		}
		else
			;
	}

#endif
}
/**************************************************************************************
* FunctionName   : Cosmetic_Procedure_Run(void)
* Description    : SysInfo.Montor_Flag:马达振动标志位
				   SysInfo.StayTime：振动时长
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void RF_Procedure_Run(void)
{
#if ARF001
	if (SysInfo.WorkState == repair_mode && SysInfo.Power_Value.state == System_ON) // 淡纹修复模式
	{
		System_Standby_Run();				  // 60s无接触关机、第一次接触皮肤马达振动
		if (SysInfo.Mode_Switch_Flag == 0x01) // RF初始化
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			Ems_DeInit();
			RF_Init();
		}
		else if (SysInfo.Mode_Switch_Flag == 0x03) // 进入待机模式
		{
			RF_Call_Data();
			IRled_stop(); // 关闭隧道灯
			RF_DeInit();
			Ems_DeInit();
			VBAT_DET_ON(); // 使能 V33V5020
			VBAT_OUT_OFF();
			RF_EN_DOWN();
			PID.Flag = 0;
			SysInfo.Power_Value.state = System_Standy;
		}
		else
			;
	}

#else
	if (SysInfo.WorkState == repair_mode && SysInfo.Power_Value.state == System_ON) // 淡纹修复模式
	{
		System_Standby_Run();				  // 60s无接触关机、第一次接触皮肤马达振动
		if (SysInfo.Mode_Switch_Flag == 0x01) // RF初始化
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			Ems_DeInit();
			RF_Init();
		}
		else if (SysInfo.Mode_Switch_Flag == 0x03) // 进入待机模式
		{
			RF_Call_Data();
			IRled_stop(); // 关闭隧道灯
			RF_DeInit();
			Ems_DeInit();
			VBAT_DET_ON(); // 使能 V33V5020
			VBAT_OUT_OFF();
			RF_EN_DOWN();
			PID.Flag = 0;
			SysInfo.Power_Value.state = System_Standy;
		}
		else
			;
	}
#endif
}

void MX_GPIO_DISInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

 //	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_6 | GPIO_PIN_7);
 	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

 	HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0);

 	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_12);

 //	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_15);
 	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_7 | GPIO_PIN_15);

 //	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
 	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_13);

 //	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
 	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 );

 	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8 | GPIO_PIN_15 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14);
 //	HAL_GPIO_DeInit(GPIOF, GPIO_PIN_1);


	/*Configure GPIO pin : PF1 */
//	GPIO_InitStruct.Pin = GPIO_PIN_1;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pins : PA4 PA7 PA11 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_4 ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

	/*Configure GPIO pins : PB10 PB11*/
//	GPIO_InitStruct.Pin = GPIO_PIN_10| GPIO_PIN_11;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10| GPIO_PIN_11, GPIO_PIN_SET);

	GPIO_InitStruct.Pin =GPIO_PIN_10| GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	Upkeep_LED_OFF();
	Repair_LED_OFF();
	GEAR_LED1_DOWN();
	GEAR_LED2_DOWN();
	GEAR_LED3_DOWN();
	GEAR_LED4_DOWN();
	GEAR_LED5_DOWN();
  BOOST_5V_OFF();
  RF_CHANNEL_OFF();
  EMS_CHANNEL_OFF();
	VCC3V3_OUT_OFF();
	Vref_EN_OFF();
//	BAT_ADC_DISABLE();
//	MOTOR_OFF();
//	RF_EN_DOWN();
//	VBAT_OUT_OFF();
}
void MX_GPIO_ONT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);

	/*Configure GPIO pins : PC13 PC14 PC15 PC6
							 PC7 */
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PF0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pins : PA0 PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PA4 PA7 PA11 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB2 PB10
							 PB13 PB14 PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PD0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : PD2 PD3 */
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
}
/**************************************************************************************
 * FunctionName   : System_Sleep(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void System_Sleep(void)
{
	static uint8_t Sleep_Flag;
	//	static uint16_t Sleep_Counts;

	if (SysInfo.Sleep_Flag && !SysInfo.Test_Mode.Test_Mode_Flag)
	{
		SysInfo.Sleep_Flag = 0;
		led_scan(0x00);
		Key_S1_Long_Str();
		Key_Call_Data();
		Sleep_Flag = 0;
		//		Sleep_Counts = 0;
		SysInfo.Power_Value.state = System_OFF;
		SysInfo.Power_Value.Enter_Sleep_Flag = 1;
	}

	//	if(((SysInfo.Power_Value.state == System_OFF ) && (!SysInfo.Batt_Value.Usb_flag)))  //进入低功耗计时，10S
	if ((SysInfo.Power_Value.Enter_Sleep_Flag) && (!SysInfo.Batt_Value.Usb_flag)) // 进入低功耗计时，10S
	{
		if (++SysInfo.Sleep_Counts > 100)
		{
			SysInfo.Sleep_Counts = 0;
			Sleep_Flag = 1;
			Led_Value.Counts = 0;
		}
	}
	else
	{
		SysInfo.Sleep_Counts = 0;
	}

	if (Sleep_Flag == 1 || SysInfo.Test_Mode.Sleep_Flag)
	{
		Sleep_Flag = 0;
		SysInfo.Test_Mode.Sleep_Flag = 0;
		printf("\n\r SLEEP\n\r");
		/****************??????????****************/
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
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	// 配置UART1的Rx引脚为EXIT模式
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // 特别注意这里要使用中断模式
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/**************************************************************************************
 * FunctionName   : Enter_Sleep_Mode(void)
 * Description    : 进入休眠模式
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Enter_Sleep_Mode(void)
{
#if ARF001

	Ems_DeInit();
	RF_DeInit();

#else
	;
#endif
	Lis2dh_Sleep_on();
RF_DeInit();
Ems_DeInit();
IRled_stop();

	BOOST_5V_OFF();
	Vref_EN_OFF();
	VBAT_OUT_OFF();
	VCC3V3_OUT_OFF();
	BAT_ADC_DISABLE();
	Upkeep_LED_OFF();
	EMS_LED_DOWN();
	SysInfo.Test_Mode.Test_Mode_Flag = 0;
	SysInfo.Batt_Value.BattCount = 0;
	SysInfo.repair_level = 0;
	SysInfo.upkeep_level = 0;
	SysInfo.WorkState = 0;

	HAL_NVIC_DisableIRQ(USART1_IRQn);
	HAL_NVIC_DisableIRQ(ADC1_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
	HAL_NVIC_DisableIRQ(TIM14_IRQn);
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);

	HAL_IWDG_Refresh(&hiwdg);
	HAL_ADC_Stop_IT(&hadc1); /// ADC转换启动
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_UART_DMAStop(&huart1);
	HAL_ADC_MspDeInit(&hadc1);
	HAL_SPI_MspDeInit(&hspi1);
	HAL_I2C_MspDeInit(&hi2c2);
	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);


	EMS_Pwm_Stop();
	RF_Osc_Off();

	MX_GPIO_DISInit();


	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn); // 使能KEY_ON_OFF_Pin中断
	Rx_Init();
		///睡眠
//		__HAL_RCC_PWR_CLK_ENABLE();   //使能PWR时钟
//		SysTick->CTRL = 0x00;//关闭定时器
//		SysTick->VAL = 0x00;//清空val,清空定时器
//		HAL_DBGMCU_EnableDBGStopMode();
//		  /* 进入停止模式，设置电压调节器为低功耗模式，等待中断唤醒 KEY2 按键下降沿唤醒*/
//		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);


//  HAL_DBGMCU_EnableDBGStopMode();
//	HAL_DBGMCU_EnableDBGStandbyMode();
  HAL_IWDG_Refresh(&hiwdg);
	HAL_SuspendTick();
	HAL_PWREx_EnableFlashPowerDown(PWR_FLASHPD_STOP);
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	HAL_PWREx_DisableFlashPowerDown(PWR_FLASHPD_STOP);
	HAL_ResumeTick();
}
/**************************************************************************************
 * FunctionName   : Enter_Sleep_Mode(void)
 * Description    : 退出睡眠模式
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

	HAL_IWDG_Refresh(&hiwdg);
	SystemClock_Config();
	MX_GPIO_ONT_Init();
	HAL_SPI_MspInit(&hspi1);
	HAL_I2C_MspInit(&hi2c2);
  HAL_ADC_MspInit(&hadc1);
	SysInfo.Power_Value.Enter_Sleep_Flag = 1;
	SysInfo.Batt_Value.Usb_flag = 0;
	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn); // 关闭外部中断KEY_ON_OFF_Pin使能
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);

	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);

	SysInfo.Power_Value.BattState = 0x01; // 开机电池电压检测标志
	Read_Current_Level_For_Eeprom();
//	VBAT_OUT_ON();
  VCC3V3_OUT_ON();
	Vref_EN_ON();
	BAT_ADC_ENABLE();
	Led_Value.Counts = 0;
	HAL_ADCEx_Calibration_Start(&hadc1);
	ADC_DMA_COV_START();

	MX_TIM1_Init();
	HAL_UART_MspInit(&huart1);
//	MX_USART1_UART_Init();
	MX_I2C2_Init();
	// Lis2dh_Init();
	HAL_UART_Receive_DMA(&huart1, SysInfo.Test_Mode.Data, 6);
	SysInfo.Test_Mode.Test_Mode_Flag = 0;
	SysInfo.Restore_Flag = 0;
//HAL_NVIC_SystemReset();
}
/**************************************************************************************
 * FunctionName   : System_1S_Procedure(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void System_1mS_Procedure(void)
{

	Ems_Boost_Process();
	RF_Process(&RF_Handle);
	SysInfo.Batt_Value.State = Scan_Batter_State();
	//		Scan_Batter_State();
	Skin_RF_Scan();
	Pole_Change(); // 旋转电极
				   //		Touch_Skin_EMS_Freq_Conversion();
}
/**************************************************************************************
 * FunctionName   : System_5mS_Procedure(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void System_5mS_Procedure(void)
{
	static uint8_t TimerCnt;

	if (++TimerCnt >= 4)
	{
		TimerCnt = 0;
		if (!SysInfo.Batt_Value.Usb_flag && !SysInfo.Test_Mode.Test_Mode_Flag)
		{
			Key_Proc();
		}

		if (SysInfo.Test_Mode.Test_Mode_Flag == ON || SysInfo.Test_Mode.Auto_Mode_Level == 0x04) // 测试模式按键函数
		{
			Test_Key_Proc();
		}
	}
}
/**************************************************************************************
 * FunctionName   : System_10mS_Procedure(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void System_10mS_Procedure(void)
{
	USB_Plug_Scan();
	Montor_Drive(&SysInfo);
	Led_Process_Run(); // LED指示灯运行
	NTC_Temp_Running();
	Skin_Touch_Scan();

#if ARF001
	Vibration_Reminder_Counts_Run(); // 换区提醒
	Set_OverTemp_Parameter();		     // 过温保护，参数设置
	Set_Ems_level(&SysInfo);
#else
	Vibration_Reminder_Counts_Run(); // 换区提醒
	Set_OverTemp_Parameter();		     // 过温保护，参数设置
#endif

	EMS_Procedure_Run(); // 紧致提拉
	RF_Procedure_Run();	 // 淡纹修复

	if (SysInfo.Batt_Value.Usb_flag)
	{
		Test_Mode_Key_Process();
	}
	Test_Mode_Run_Process();
	Ageing_Test_Process();
	Test_AutoMode_Run_Process();
}
/**************************************************************************************
* FunctionName   : System_100mS_Procedure(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None
.**************************************************************************************/
void System_100mS_Procedure(void)
{

	Frequency_conversion_Process(); // 变频
	Set_RF_Level(&SysInfo);

#if ARF001
	;
#else
	Set_Ems_level(&SysInfo);
#endif

	System_Data_Save();
	System_Sleep();
	Test_UART_Deal_Process();
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
	BAT_ADC_ENABLE();
	Vref_EN_ON();
	VCC3V3_OUT_ON();
	HAL_ADCEx_Calibration_Start(&hadc1);
	ADC_DMA_COV_START();
	SysInfo.WorkState = Standy_mode;
	SysInfo.Batt_Value.State = BAT_40_60_STATUS;
	SysInfo.Power_Value.Enter_Sleep_Flag = 1;
	SysInfo.Restore_Flag = 0;
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn); // 关闭外部中断使能
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	Led_Value.Level = 0x5a;
	Read_Current_Level_For_Eeprom();
	BOOST_5V_OFF();
	PID.Kp = 1.5;
	PID.Ki = 0.8;
	PID.Kd = 0.1;
	PID.Constant = 50;
	PID.temp = 500;
	printf("\n\r ==========\n\r");
	printf("\n\r version:%s\n\r", version);
	printf("\n\r ----------\n\r");
	HAL_UART_Receive_DMA(&huart1, SysInfo.Test_Mode.Data, 20);
}
/**************************************************************************************
 * FunctionName   : HAL_UARTEx_RxEventCallback(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//	if(huart->Instance == USART1)  //?????????UART1
//	 {
//		if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)  // ???????????????
//		{
//			__HAL_UART_CLEAR_IDLEFLAG(&huart1);  // ?????????
//			HAL_UART_DMAStop(&huart1);           // ??DMA????
//			DMA1_Channel3->CNDTR = 20;
//			HAL_UART_Receive_DMA(&huart1,SysInfo.Test_Mode.Data,20);
//			SysInfo.Receive_Flag = 1;
//		}
//	 }
// }
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
