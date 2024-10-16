
#include "includes.h"
#include "rf.h"

_RF_HANDLE_TypeDef RF_Handle;
/*************************************************************************************
 * FunctionName	: RF_Osc_ON(void)
 * Description		: RF PWM 开启
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void RF_Osc_ON(void)
{
	/* 启动通道PWM输出 */
	HAL_TIM_PWM_Start(&RF_Tim, TIM_CHANNEL_1);	  /// RF
												  /* 启动定时器互补通道PWM输出 */
	HAL_TIMEx_PWMN_Start(&RF_Tim, TIM_CHANNEL_1); /// RF
}
/*************************************************************************************
 * FunctionName	: RF_Osc_Off(void)
 * Description		: RF PWM ???
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void RF_Osc_Off(void)
{
	/* 启动通道PWM关闭 */
	HAL_TIM_PWM_Stop(&RF_Tim, TIM_CHANNEL_1);	 /// RF
												 /* 启动定时器互补通道PWM关闭 */
	HAL_TIMEx_PWMN_Stop(&RF_Tim, TIM_CHANNEL_1); /// RF
}
/*************************************************************************************
 * FunctionName	: RF_Stop(void)
 * Description		: 停止射频输出
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void RF_Stop(void)
{
	RF_Handle.Flag = 0;
	//	RF_EN_DOWN();
}
/*************************************************************************************
 * FunctionName	: RF_Start(void)
 * Description	: 开启射频输出
 * EntryParameter:
 * ReturnValue	:
 **************************************************************************************/
void RF_Start(void)
{
	RF_Handle.Flag = 1;
	RF_EN_UP();
	VBAT_DET_ON(); // 使能 V33V5020
	VBAT_OUT_ON(); //
}
/*************************************************************************************
 * FunctionName	: Set_RF_Level(_Sys_Info_TypeDef* Level)
 * Description		: 射频档位设置
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void Set_RF_Level(_Sys_Info_TypeDef *Level)
{
	uint8_t Vol_Level = 0;

	if (SysInfo.Power_Value.state == System_ON)
	{
		if (Level->WorkState == repair_mode) // 修复模式档位
		{
			Vol_Level = Level->repair_level;
		}
		else // 紧致提拉模式档位
		{
			switch (Level->upkeep_level)
			{
#if ARF001
			case Level_None:
				Vol_Level = Level_3;
				break;
			case Level_1:
				Vol_Level = Level_4;
				break;
			case Level_2:
				Vol_Level = Level_4;
				break;
			case Level_3:
				Vol_Level = Level_5;
				break;
			case Level_4:
				Vol_Level = Level_5;
				break;
			case Level_5:
				Vol_Level = Level_5;
				break;
			default:
				Vol_Level = Level_3;
				break;
#else
			case Level_None:
				Vol_Level = Level_3;  //淡纹修复模式过温及不移动降档
				break;
			case Level_1:
				Vol_Level = Level_3;
				break;
			case Level_2:
				Vol_Level = Level_4;
				break;
			case Level_3:
				Vol_Level = Level_4;
				break;
			case Level_4:
				Vol_Level = Level_5;
				break;
			case Level_5:
				Vol_Level = Level_5;
				break;
			default:
				Vol_Level = Level_3;
				break;
#endif
			}
		}
		RF_Handle.Period = RF_PERIOD;
		RF_Handle.Flag = 1;
		RF_Handle.Mode = 1;

		switch (Vol_Level) // 档位设置
		{
		case Level_None: // 过温参数
			RF_Level_1();
			RF_Handle.Duty = Repair_Duty0;
			break;
		case Level_1:
			RF_Level_1();
			RF_Handle.Duty = Repair_Duty1;
			break;
		case Level_2:
			RF_Level_2();
			RF_Handle.Duty = Repair_Duty2;
			break;
		case Level_3:
			RF_Level_3();
			RF_Handle.Duty = Repair_Duty3;
			break;
		case Level_4:
			RF_Level_4();
			RF_Handle.Duty = Repair_Duty4;
			break;
		case Level_5:
			//										RF_Level_5();
			RF_Level_6();
			RF_Handle.Duty = Repair_Duty5;
			break;
		default:
			RF_Level_1();
			RF_Handle.Duty = Repair_Duty1;
			break;
		}
	}
}
/*************************************************************************************
 * FunctionName	: EF_Frequency_conversion(void)
 * Description		: 射频变频
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void RF_Frequency_conversion(uint8_t level)
{
	switch (level)
	{
	case 0:
		/* Set the Autoreload value */
		TIM17->ARR = (uint32_t)23;
		TIM17->CCR1 = (uint32_t)12;
		break;
	case 1:
		TIM17->ARR = (uint32_t)31;
		TIM17->CCR1 = (uint32_t)16;
		break;
	case 2:
		TIM17->ARR = (uint32_t)47;
		TIM17->CCR1 = (uint32_t)23;
		break;
	default:
		TIM17->ARR = (uint32_t)47;
		TIM17->CCR1 = (uint32_t)23;
		break;
	}
}
/*************************************************************************************
 * FunctionName	: Frequency_conversion_Process(void)
 * Description		: 射频变频运行函数
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void Frequency_conversion_Process(void)
{
	//	static uint16_t Time_Counts;
	static uint8_t Counts = 0;
	static uint16_t Heating_Counts = 0;

	if (SysInfo.Skin_Touch_Flag)
	{
#if 0		
		if(SysInfo.NTC_Temp<39)			
		{
			Counts=0;
		}
		else if(SysInfo.NTC_Temp<41&& SysInfo.NTC_Temp>=39)			
		{
			Counts=1;
		}
		else if(SysInfo.NTC_Temp>=41)			
		{
			Counts=2;			
		}
		else 
		{
//			Time_Counts = Frequency_1M0_CNT ;
			Counts=2;	
		}
#else
		SysInfo.Freq_Cnt++;
		if (SysInfo.Freq_Cnt < Frequency_2M0_CNT)
		{
			Counts = 0;
		}
		else if (SysInfo.Freq_Cnt < Frequency_1M5_CNT && SysInfo.Freq_Cnt >= Frequency_2M0_CNT)
		{
			Counts = 1;
		}
#if ARF001 // 德辉达降本
		else if (SysInfo.Freq_Cnt < Frequency_1M0_CNT)
		{
			Counts = 2;
		}
		else
		{
			Counts = 0;
			SysInfo.Freq_Cnt = 0;
		}

		if (SysInfo.WorkState == upkeep_mode) // 紧致提拉模式，RF 2MHZ运行
		{
			//			if(SysInfo.Freq_Cnt<4)
			//			{
			//				Counts=0;
			//			}
			//			else if(SysInfo.Freq_Cnt<7)
			//			{
			//				Counts=1;
			//			}
			//			else if(SysInfo.Freq_Cnt<10)
			//			{
			//				Counts=2;
			//			}
			//	        else
			//			{
			Counts = 0;
			SysInfo.Freq_Cnt = 0;
			//			}
		}
#else
		else
		{
			//			Time_Counts = Frequency_1M0_CNT ;
			Counts = 2;
			SysInfo.Freq_Cnt = Frequency_1M0_CNT;
		}
#endif
#endif
	}
	else
	{
		//		Time_Counts = 0 ;
		Counts = 0;
	}

	if (SysInfo.Heating_Flag && SysInfo.Skin_Touch_Flag)
	{
		if (++Heating_Counts < Heating_CNT) // 开机前3秒5档运行，使电极迅速升温
		{
			SysInfo.repair_level = Level_5;
			SysInfo.upkeep_level = Level_5;
		}
		else
		{
			SysInfo.repair_level = SysInfo.Save_Data.repair_level;
			SysInfo.upkeep_level = SysInfo.Save_Data.upkeep_level;
			Heating_Counts = 0;
			SysInfo.Heating_Flag = 0;
		}
	}
	else
	{
		Heating_Counts = 0;
	}
#if ARF001 // 德辉达降本
	;
#else
	if (SysInfo.OverTemp_Flag) // 过温
	{
		SysInfo.Freq_Cnt = Frequency_1M0_CNT;
		Counts = 2;
	}
#endif

	if (!SysInfo.Test_Mode.Test_Mode_Flag)
	{
		RF_Frequency_conversion(Counts);
	}
	else
	{
		RF_Frequency_conversion(SysInfo.Test_Mode.Set_RF_Freq); // 测试模式
	}
}
/*************************************************************************************
 * FunctionName	: RF_Process(_RF_HANDLE_TypeDef* RfHandle)
 * Description		: 射频运行
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void RF_Process(_RF_HANDLE_TypeDef *RfHandle)
{
	static uint8_t Pwm_Count = 0;

	if (RfHandle->Flag && RF_Handle.Run_Flag)
	{
		if (Pwm_Count < (RfHandle->Duty))
		{
			if (Pwm_Count == 0)
			{
				RF_Osc_ON(); // 开启射频PWM输出
			}
			if (Pwm_Count > 4)
			{
				RfHandle->ADC_Flag = 1;
			}
		}
		else
		{
			if (Pwm_Count == (RfHandle->Duty)) //
			{
				RF_Osc_Off(); // 关闭射频PWM输出
				RfHandle->ADC_Flag = 0;
			}
		}

		Pwm_Count++;
		if (Pwm_Count >= (RfHandle->Period))
		{
			Pwm_Count = 0;
		}
	}
	else
	{
		Pwm_Count = 0;
		RF_Osc_Off();
	}
}
/*************************************************************************************
 * FunctionName	: RF_Pole_Change(_RF_HANDLE_TypeDef* RfHandle)
 * Description		:  旋转电极 170ms变一次  10ms运行一次函数
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void Pole_Change(void)
{
#if ARF001
	if (SysInfo.Test_Mode.Test_Mode_Flag == OFF || SysInfo.Test_Mode.Ageing_Flag)
	{
		if (SysInfo.WorkState == repair_mode)
		{
			Set_Mbi5020_Out(RF_CH1_ON_BIT);
		}
		else if (SysInfo.WorkState == upkeep_mode)
		{
			if (RF_Handle.Run_Flag)
			{
				Set_Mbi5020_Out(RF_CH1_ON_BIT);
			}
			else
			{
				Set_Mbi5020_Out(EMS_CH2_ON_BIT);
			}
		}
		else
		{
			Set_Mbi5020_Out(0);
		}
	}
#else
	static uint8_t Rotate_Count = 0, Pole_Counts = 0, Last_Pole_Counts = 0;

	//	if(SysInfo.Power_Value.state == System_ON ||SysInfo.Test_Mode.Test_Mode_Flag!=OFF)
	if (SysInfo.Power_Value.state == System_ON || SysInfo.Test_Mode.Ageing_Flag)
	{
		if (RF_Handle.Run_Flag || EMS_Handle.Run_Flag)
		{
			Rotate_Count++;

			if (Rotate_Count == Pole_Change_Time)
			{
				Set_Mbi5020_Out(0);
			}
			else if (Rotate_Count > Pole_Change_Time + 2) // 延时2ms再改变电极输出
			{
				Rotate_Count = 0;
				if (++Pole_Counts > 2)
				{
					Pole_Counts = 0;
				}
			}
			else
				;
		}
		else
		{
			Rotate_Count = 0;
			Pole_Counts = 0;
			Set_Mbi5020_Out(0);
		}
		//    if(SysInfo.Test_Mode.Test_Mode == 0x03 || SysInfo.Test_Mode.Test_Mode == 0x04 || SysInfo.Test_Mode.Ageing_Flag)
		//		{
		//		   Pole_Counts = 0x0a;
		//		}
		//		if(Pole_Counts==Last_Pole_Counts && (SysInfo.Test_Mode.Test_Mode==0 ||SysInfo.Test_Mode.Test_Mode==0x0a))
		if (Pole_Counts == Last_Pole_Counts)
		{
			Last_Pole_Counts = (Pole_Counts + 1) % 3;
			if (SysInfo.Heating_Flag && SysInfo.WorkState == repair_mode)
			{
				Pole_Counts = 0x03;
				Last_Pole_Counts = 0;
			}

			switch (Pole_Counts)
			{
			case 0:
				if (SysInfo.WorkState == repair_mode)
				{
					Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH2_ON_BIT);
				}
				else
				{
					Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH2_ON_BIT | EMS_CH2_ON_BIT);
				}
				break;
			case 1:
				if (SysInfo.WorkState == repair_mode)
				{
					Set_Mbi5020_Out(RF_CH2_ON_BIT | RF_CH3_ON_BIT);
				}
				else
				{
					Set_Mbi5020_Out(RF_CH2_ON_BIT | RF_CH3_ON_BIT | EMS_CH3_ON_BIT);
				}
				break;
			case 2:
				if (SysInfo.WorkState == repair_mode)
				{
					Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH3_ON_BIT);
				}
				else
				{
					Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH3_ON_BIT | EMS_CH1_ON_BIT);
				}
				break;
			case 3: // 开机前3S 3对电极同时打开，提高加热速度
				Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH2_ON_BIT | RF_CH3_ON_BIT);

				break;
			default:
				Set_Mbi5020_Out(0);
				Pole_Counts = 0;
				break;
			}
		}
	}
	else
	{
		Rotate_Count = 0;
		Pole_Counts = 0;
		Last_Pole_Counts = 0;
	}
#endif
}
/**************************************************************************************
 * FunctionName   : Ems_Init(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void RF_Init(void)
{
	//	IRled_start();
	RF_Osc_ON();
	RF_Start();	   // RF射频开启
	VBAT_DET_ON(); // 使能 V33V5020
	VBAT_OUT_ON(); //
	RF_Handle.Run_Flag = 1;
}
/**************************************************************************************
 * FunctionName   : Ems_DeInit(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void RF_DeInit(void)
{
	RF_Osc_Off();
	RF_Stop(); // RF射频关闭
	//	VBAT_DET_OFF(); //关闭 V33V5020
	//	VBAT_OUT_OFF();
	RF_Handle.Run_Flag = 0;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE**********************/
