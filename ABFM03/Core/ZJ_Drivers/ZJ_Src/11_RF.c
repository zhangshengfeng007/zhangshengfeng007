
#include "includes.h"
#include "rf.h"

_RF_HANDLE_TypeDef RF_Handle;
/*************************************************************************************
* FunctionName	: RF_Osc_ON(void)
* Description		: RF PWM ����
* EntryParameter:
* ReturnValue		: 123
**************************************************************************************/
void RF_Osc_ON(void)
{
 /* ����ͨ��PWM��� */
  HAL_TIM_PWM_Start(&RF_Tim, TIM_CHANNEL_1);  ///RF
  /* ������ʱ������ͨ��PWM��� */
  HAL_TIMEx_PWMN_Start(&RF_Tim,TIM_CHANNEL_1);///RF
}
/*************************************************************************************
* FunctionName	: RF_Osc_Off(void)
* Description		: RF PWM �ر�
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void RF_Osc_Off(void)
{
 /* ����ͨ��PWM��� */
  HAL_TIM_PWM_Stop(&RF_Tim, TIM_CHANNEL_1);  ///RF
  /* ������ʱ������ͨ��PWM��� */
  HAL_TIMEx_PWMN_Stop(&RF_Tim,TIM_CHANNEL_1);///RF
}
/*************************************************************************************
* FunctionName	: RF_Stop(void)
* Description		: ֹͣ��Ƶ���
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void RF_Stop(void)
{
	RF_Handle.Flag = 0;
	RF_EN_DOWN();
//	VBAT_DET_OFF() ;
//	VBAT_OUT_OFF() ;
}
/*************************************************************************************
* FunctionName	: RF_Start(void)
* Description		: ������Ƶ���
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void RF_Start(void)
{
	RF_Handle.Flag = 1;
	RF_EN_UP();
//	VBAT_DET_ON(); //ʹ�� V33V5020
	VBAT_OUT_ON(); //
}
/*************************************************************************************
* FunctionName	: Set_RF_Level(_Sys_Info_TypeDef* Level)
* Description		: ��Ƶ��λ����
* EntryParameter:
* ReturnValue		: 100ms
**************************************************************************************/
void Set_RF_Level(_Sys_Info_TypeDef* Level)
{
//  uint8_t Vol_Level = 0;

	if(SysInfo.Power_Value.state == System_ON )
	{
		RF_Handle.Period = RF_PERIOD;
		//RF_Handle.Flag =1 ;
		//RF_Handle.Mode =1 ;

		if(Level->WorkState == repair_mode) //�޸�ģʽ��λ
		{
			switch(Level->repair_level) //��λ����
			{
				case Level_None:  //���²���
								RF_Level_1();
								RF_Handle.Duty = Repair_Duty0;
								RF_Handle.Period = 0;
								break ;
				case Level_1:
					      		// RF_Handle.Period = (RF_PERIOD-1);  // 2023 03 18 ͳһʱ��
								RF_Level_1();
								RF_Handle.Duty = Repair_Duty1;
								break ;
				case Level_2:
								RF_Level_3();
								RF_Handle.Duty = Repair_Duty2;
								break ;
				case Level_3:
								RF_Level_3();//RF_Level_5();
								RF_Handle.Duty = Repair_Duty3;
								break ;
				default :
								RF_Level_1();
								RF_Handle.Duty = Repair_Duty1;
								break ;
			}
		}
		else  if(Level->WorkState== upkeep_mode)//��������ģʽ��λ
		{
			switch(Level->upkeep_level) //��λ����
			{
				case Level_None:  //���²���
								EMS_RF_Level_5();
								RF_Handle.Duty = UpkeepRF_Duty0;
								RF_Handle.Period = 0;
								break ;
				case Level_1:
								EMS_RF_Level_5();
								RF_Handle.Duty = UpkeepRF_Duty1;
								break ;
				case Level_2:
								EMS_RF_Level_5();
								RF_Handle.Duty = UpkeepRF_Duty2;
								break ;
				case Level_3:
								EMS_RF_Level_5();
								RF_Handle.Duty = UpkeepRF_Duty3;
								break ;
				default :
								EMS_RF_Level_5();
								RF_Handle.Duty = UpkeepRF_Duty1;//UpkeepRF_Duty0;
								break ;
			}

		}

	}
	else if(SysInfo.Power_Value.state == System_SHORT_CIRCUIT_PROTECT)
	{
		if(Level->WorkState== repair_mode)
		{
			RF_Level_1();
		}
		else
		{
			EMS_RF_Level_5();
		}

		RF_Handle.Duty = 5;  // 2023 03 21 ��·����Ӳ��--�˷�--��ʱ
		RF_Handle.Period = RF_PERIOD;
	}

}
/*************************************************************************************
* FunctionName	: EF_Frequency_conversion(void)
* Description		: ��Ƶ��Ƶ
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void RF_Frequency_conversion(uint8_t level)
{
	switch(level)
	{
		case RF_2MHZ_FREQ:
				/* Set the Autoreload value */
				TIM17->ARR = (uint32_t)16 - 1;  //2MHZ
				TIM17->CCR1 = (uint32_t)8;
				break;
		case RF_1_5MHZ_FREQ:
				TIM17->ARR = (uint32_t)22 - 1; //1.5MHZ
				TIM17->CCR1 = (uint32_t)11 ;
				break;
		case RF_1MHZ_FREQ:
				TIM17->ARR = (uint32_t)32 -1;  //1MHZ
				TIM17->CCR1 = (uint32_t)16 ;
				break;
		default :
				TIM17->ARR = (uint32_t)32 - 1;
				TIM17->CCR1 = (uint32_t)16 ;
				break;
	}
}
/*************************************************************************************
* FunctionName	: Frequency_conversion_Process(void)
* Description		: ��Ƶ��Ƶ���к���
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void Frequency_conversion_Process(void)
 {
	static uint8_t Counts = RF_1MHZ_FREQ;

	if(SysInfo.Skin_Touch_Flag)
	{
		switch(SysInfo.WorkState)
		{
			case upkeep_mode:
			{
				SysInfo.Freq_Cnt = 0;
				break;
			}
			case repair_mode:
			{
				SysInfo.Freq_Cnt ++;
				if(SysInfo.Freq_Cnt < Frequency_2M0_CNT)
				{
					Counts = RF_2MHZ_FREQ;
				}
				else if(SysInfo.Freq_Cnt < Frequency_1M5_CNT && SysInfo.Freq_Cnt >= Frequency_2M0_CNT)
				{
					Counts = RF_1_5MHZ_FREQ;
				}
				else if(SysInfo.Freq_Cnt < Frequency_1M0_CNT)
				{
					Counts = RF_1MHZ_FREQ;
				}
				else
				{
					Counts = RF_1MHZ_FREQ;
					SysInfo.Freq_Cnt= Frequency_1M0_CNT;
				}
			}
		}

		if(SysInfo.Test_Mode.STATUS == TILA_MODE_RF_TEST)
		{
			switch(SysInfo.Test_Mode.Set_RF_Freq)
			{
				case RF_1MHZ_FREQ:
				{
					Counts = RF_1MHZ_FREQ;
					break;
				}
				case RF_1_5MHZ_FREQ:
				{
					Counts = RF_1_5MHZ_FREQ;
					break;
				}
				case  RF_2MHZ_FREQ:
				{
					Counts = RF_2MHZ_FREQ;
					break;
				}

				default:
				{
					break;
				}
			}

		}



	}
	RF_Frequency_conversion(Counts);
}
/*************************************************************************************
* FunctionName	: RF_Process(_RF_HANDLE_TypeDef* RfHandle)
* Description		: ��Ƶ����
* EntryParameter:
* ReturnValue		:   1ms
**************************************************************************************/
void RF_Process(_RF_HANDLE_TypeDef* RfHandle)
{
	static uint8_t Pwm_Count = 0 ;
	static uint8_t Read_adc_delay = 0 ;

  if((RfHandle->Flag && RF_Handle.Run_Flag)||(SysInfo.Power_Value.state == System_SHORT_CIRCUIT_PROTECT))
	{
		if(Pwm_Count < (RfHandle->Duty))
		{
			if(Pwm_Count==0 && !SysInfo.Test_Mode.EMS_Flag)
			{
				RF_Osc_ON();	 //������ƵPWM���
			}

			if(Read_adc_delay == 2) // 20230321 ---RF adc��ȡ����Ҫ2ms����ʱ��Ӳ���˷�2ms����ȶ�
			{
				RfHandle->Read_ADC_Flag =1;
			}
			Read_adc_delay ++;
		}
		else
		{
			if(Pwm_Count == (RfHandle->Duty)) //
			{
				RF_Osc_Off();	 //�ر���ƵPWM���
				RfHandle->Read_ADC_Flag =0;
			}
		}

		Pwm_Count ++;
		if(Pwm_Count >= (RfHandle->Period))
		{
			Pwm_Count = 0;
		}
	}
	else
	{
		Read_adc_delay = 0;
		Pwm_Count = 0;
		RF_Osc_Off();
	}
}
/*************************************************************************************
* FunctionName	: RF_Pole_Change(_RF_HANDLE_TypeDef* RfHandle)
* Description		: ��ת�缫 170ms��һ��  10ms����һ�κ���
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void Pole_Change(void)
{
//	if(SysInfo.Test_Mode.Test_Mode_Flag==OFF || SysInfo.Test_Mode.Ageing_Flag)
//	{
		if(SysInfo.WorkState == repair_mode)
		{
			Set_Mbi5020_Out(RF_CH1_ON_BIT);
		}
		else if (SysInfo.WorkState == upkeep_mode)
		{
			if(RF_Handle.Run_Flag)
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
			Set_Mbi5020_Out(CLOSE_RF_EMS_OUT_BIT);
		}
//	}
}
/**************************************************************************************
* FunctionName   : Ems_Init(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void RF_Init(void)
{
	IRled_start();
	RF_Osc_ON();
	RF_Start(); //RF��Ƶ����
//	VBAT_OUT_ON(); //

	RF_Handle.Run_Flag=1;
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
	RF_Stop(); //RF��Ƶ�ر�
//	VBAT_OUT_OFF();
	RF_Handle.Run_Flag=0;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE**********************/


