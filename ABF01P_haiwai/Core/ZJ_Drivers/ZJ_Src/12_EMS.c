#include "includes.h"
#include "ems.h"

extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim1;

_EMS_HANDLE_TypeDef EMS_Handle;
_PID_HANDLE_TypeDef PID;

#if (ARF001 == DEVICE_R1_RPO)				  // �»Դｵ��
#define EMS_Freq 1000	  // ���� ʱ��
#define duty 30			  // ���� 30
#define interval 35		  // �����������ʱ��
#define channel_p_start 5 //
#define channel_p_stop channel_p_start + duty
#define channel_n_start channel_p_stop + interval
#define channel_n_stop channel_n_start + duty

#elif (ARF001 == DEVICE_R1_RPO_MAX)
#define EMS_Freq 250	  // ���� ʱ��
#define duty 11			  // ����
#define interval 30		  // �����������ʱ��
#define channel_p_start 5 //
#define channel_p_stop channel_p_start + duty
#define channel_n_start channel_p_stop + interval
#define channel_n_stop channel_n_start + duty

#elif (ARF001 == DEVICE_R1_HAIWAI)
#define EMS_Freq 	1000	  //need 1khz
#define duty 		11		  //
#define interval 	30		  //
#define channel_p_start 5 //
#define channel_p_stop channel_p_start + duty
#define channel_n_start channel_p_stop + interval
#define channel_n_stop channel_n_start + duty

#endif

static uint16_t EMS_Freq_Value = EMS_Freq;
static uint16_t EMS_duty_Value = duty;
static uint16_t EMS_interval_Value = interval;
/*
static uint16_t EMS_channel_p_start = channel_p_start ;
static uint16_t EMS_channel_p_stop  = channel_p_stop ;
static uint16_t EMS_channel_n_start = channel_n_start ;
static uint16_t EMS_channel_n_stop  = channel_n_stop ;
*/
/**************************************************************************************
 * FunctionName   : EMS_Pwm_Start(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void EMS_Pwm_Start(void)
{

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	SET_BIT(TIM1->CR1, TIM_CR1_UDIS);
	__HAL_TIM_SET_AUTORELOAD(&htim1, EMS_Freq_Value);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, channel_n_start);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, channel_n_stop);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, channel_p_start);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, channel_p_stop);
	CLEAR_BIT(TIM1->CR1, TIM_CR1_UDIS);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	SysInfo.Ems_ON_Flag = 1;
}
/**************************************************************************************
 * FunctionName   : EMS_Pwm_Stop(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void EMS_Pwm_Stop(void)
{
	// HAL_TIM_PWM_Stop(&htim16,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
}

/**************************************************************************************
 * FunctionName   : Ems_Osc_Off(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ems_Osc_Off(void)
{

	SysInfo.Ems_ON_Flag = 0;
	TIM1->CNT = 20000;
#if 0
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
#endif
	TIM1->CR1 &= ~(1 << 0);
#if 0
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_2);
#endif
	TIM1->CNT = 0;
}
/**************************************************************************************
 * FunctionName   : Ems_Osc_On(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ems_Osc_On(void)
{
	//	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	//	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);
	//	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
	SysInfo.Ems_ON_Flag = 1;
}
/**************************************************************************************
 * FunctionName   : Ems_Boost_Off(void)
 * Description    : ��Դ�ر�
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ems_Boost_Off(void)
{
	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
}
/**************************************************************************************
 * FunctionName   : Ems_Boost_On(void)
 * Description    : ��Դ����
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ems_Boost_On(void)
{
	//	ems_boost_duty = duty;
	//	ems_vol_value = vol_value;

	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
	//	ems_touch_threshold = touch_threshold;
}
/**************************************************************************************
 * FunctionName   : acquire_adc_value(uint8_t channel, uint16_t* data)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
uint16_t acquire_adc_value(uint8_t channel, uint16_t *data)
{
	static uint8_t Counts, state = 0;
	static uint16_t max = 0xff, min = 0x00, data_temp = 0x00;
	static uint16_t sum;

	data_temp = filter((uint16_t *)&ADC_Value, channel, ADC_CHANNEL_NUM, ADC_ADD_COUNT);

	if (++Counts > 10)
	{
		if (data_temp > max)
		{
			max = data_temp;
		}
		if (data_temp < min)
		{
			min = data_temp;
		}
		sum += data_temp;
		state = 0;
	}
	else
	{
		sum -= min;
		sum -= max;
		*data = sum >> 3;

		Counts = 0;
		state = 1;
		max = 0x00;
		min = 0xFF;
		sum = 0x0000;
	}

	return state;
}
/*************************************************************************************
 * FunctionName	: EF_Frequency_conversion(void)
 * Description		: EMS��Ƶ
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void EMS_Frequency_conversion(uint8_t PSC)
{
	/* Set the Autoreload value */
	//	TIM1->PSC = (uint32_t)PSC ;
	//	TIM17->ARR = (uint32_t)EMS_Freq ;
	switch (PSC)
	{
	case 0:
		TIM1->PSC = (uint32_t)47;
		EMS_Freq_Value = EMS_Freq;
		break;
	case 1:
		TIM1->PSC = (uint32_t)47;
		EMS_Freq_Value = 800; // 1.25KHZ
		//				EMS_Freq_Value = 500;   //850  //2KHZ
		//				EMS_Freq_Value = 330;   //650
		break;
	case 2:
		TIM1->PSC = (uint32_t)47;
		EMS_Freq_Value = 666; // 1.5KHZ
		//				EMS_Freq_Value = 330;  //650  //3KHZ
		//				EMS_Freq_Value = 500;  //850
		break;
	case 3:
		TIM1->PSC = (uint32_t)47;
		EMS_Freq_Value = 500; // 2KHZ
		//				EMS_Freq_Value = 250;  //250  //4KHZ
		break;
	default:
		TIM1->PSC = (uint32_t)47;
		EMS_Freq_Value = EMS_Freq;
		break;
	}
}
/**************************************************************************************
 * FunctionName   : Set_Ems_level(void)
 * Description    : ����EMS��λ
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Set_Ems_level(_Sys_Info_TypeDef *EmsLevel)
{
	//	uint16_t Ems_Value;
	static uint8_t EMS_PSC = 0;
	if (SysInfo.Power_Value.state == System_ON)
	{
		if (EmsLevel->WorkState == upkeep_mode)
		{
			//			#if ARF001
			switch (SysInfo.Save_Data.upkeep_level)
			//			#else
			//			switch(EmsLevel->upkeep_level)
			//			#endif
			{
			case Level_None:
				EMS_Handle.Duty = Upkeep_Duty0;
				EMS_Handle.vol_value = EMS_VOL_LEVEL1;
				break;
			case Level_1:
				EMS_Handle.Duty = Upkeep_Duty1;
				EMS_Handle.vol_value = EMS_VOL_LEVEL1;
				break;
			case Level_2:
				EMS_Handle.Duty = Upkeep_Duty2;
				EMS_Handle.vol_value = EMS_VOL_LEVEL2;
				break;
			case Level_3:
				EMS_Handle.Duty = Upkeep_Duty3;
				EMS_Handle.vol_value = EMS_VOL_LEVEL3;
				break;
			case Level_4:
				EMS_Handle.Duty = Upkeep_Duty4;
				EMS_Handle.vol_value = EMS_VOL_LEVEL4;
				break;
			case Level_5:
				EMS_Handle.Duty = Upkeep_Duty5;
				EMS_Handle.vol_value = EMS_VOL_LEVEL5;
				break;
			default:
				EMS_Handle.Duty = Upkeep_Duty0;
				EMS_Handle.vol_value = EMS_VOL_LEVEL1;
				break;
			}
			EMS_Handle.Period = EMS_BOOST_SW_PERIOD;

#if (ARF001 == DEVICE_R1_RPO)
			if (EMS_Handle.Run_Flag)
			{
				EMS_Handle.EMS_Cnt++;
#if EMS_Mode // EMS???2s?????�� EMS_Handle.vol_value+1V -> EMS_Handle.vol_value-> EMS_Handle.vol_value-1V ???
				if (EMS_Handle.EMS_Cnt <= EMS_1000_ms)
				{
					EMS_Handle.vol_value += EMS_VOL_Constant;
				}
				else if (EMS_Handle.EMS_Cnt <= EMS_1500_ms)
				{
					;
				}
				else if (EMS_Handle.EMS_Cnt <= EMS_2000_ms)
				{
					EMS_Handle.vol_value -= EMS_VOL_Constant;
				}
				else
				{
					EMS_Handle.EMS_Cnt = 0;
				}
#else
				if (EMS_Handle.EMS_Cnt > 50) // EMS��Ƶ 1 KHZ ~ 4 KHZ
				{
					EMS_Handle.EMS_Cnt = 0;
					EMS_PSC++;
					//						EMS_PSC=0; //????
					EMS_Frequency_conversion(EMS_PSC);
				}

				//					if(EMS_Handle.EMS_Cnt<100)//EMS��Ƶ 1 KHZ ~ 3 KHZ
				//					{
				//						EMS_PSC=0;
				//					}
				//					else if(EMS_Handle.EMS_Cnt<150)
				//					{
				//						EMS_PSC=1;
				//					}
				//					else if(EMS_Handle.EMS_Cnt>175)
				//					{
				//						EMS_PSC=2;
				//					}
				//					else if(EMS_Handle.EMS_Cnt<200)
				//					{
				//						EMS_PSC=3;
				//					}
				//					else
				//					{
				//						EMS_Handle.EMS_Cnt = 0;
				//						EMS_PSC = 0 ;
				//					}
				//					EMS_Frequency_conversion(EMS_PSC);
#endif
			}
			else
			{
				EMS_Handle.EMS_Cnt = 0;
				//					if(EMS_PSC)
				//					{
				EMS_PSC = 0;
				EMS_Frequency_conversion(EMS_PSC);
				//					}
			}
#else
			;
#endif
		}
	}
}
/**************************************************************************************
 * FunctionName   : Ems_Init(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ems_Init(void)
{
	//	IRled_start();
	Ems_Boost_On();
	EMS_Handle.Run_Flag = 1;
	PID.Flag = 1;
	//	VBAT_DET_ON(); //ʹ�� V33V5020
	VBAT_OUT_ON(); //
	EMS_Pwm_Start();
}
/**************************************************************************************
 * FunctionName   : Ems_DeInit(void)
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ems_DeInit(void)
{
	//IRled_stop();
	Ems_Boost_Off();
	EMS_Handle.Run_Flag = 0;
	PID.Flag = 0;
	//	VBAT_DET_OFF(); //ʹ�� V33V5020
	//	VBAT_OUT_OFF(); //
	EMS_Pwm_Stop();
}
/**************************************************************************************
 * FunctionName   : Touch_Skin_EMS_Freq_Conversion(void)
 * Description    : Ƥ���Ӵ���EMS������������
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Touch_Skin_EMS_Freq_Conversion(void)
{
	static uint8_t EMS_Counts, Lock_Flag;
	static uint8_t EMS_TimerCnt;

	if (EMS_Handle.Run_Flag)
	{
		if (SysInfo.Skin_Touch_Flag)
		{
			if (++EMS_TimerCnt > 20)
			{
				Lock_Flag = 0;
				if (++EMS_Counts > 25)
				{
					EMS_Counts = 25;
					Lock_Flag = 1;
				}
				EMS_TimerCnt = 0;
			}
		}
		else
		{
			EMS_TimerCnt = 0;
			EMS_Counts = 10;
		}
		if (!Lock_Flag)
		{
			TIM1->ARR = EMS_Counts;
		}
	}
}

/*********************************************************
 ������: PID_IncreaseCalc
 ��  ��: ����ʽPID
 ����ֵ:
 ���ֵ: ��
 ����ֵ: ��
 **********************************************************/
int PID_IncreaseCalc(_PID_HANDLE_TypeDef *vPID)
{
	int PID_Out = 0;
	float pValue = 0;
	float iValue = 0;
	float dValue = 0;

	vPID->CurErr = vPID->TargetVal - vPID->CurrentVal; // ��ǰƫ��

	pValue = (vPID->Kp) * (vPID->CurErr - vPID->LastErr);
	iValue = (vPID->Ki) * vPID->CurErr;
	dValue = (vPID->Kd) * (vPID->CurErr - 2 * vPID->LastErr + vPID->PreErr);

	PID_Out = pValue + iValue + dValue;

	vPID->PreErr = vPID->LastErr;
	vPID->LastErr = vPID->CurErr; // ��ǰƫ������һ��ƫ��

	return PID_Out;
}

//		PID.Kp = 0.9;
//		PID.Ki = 0.1;
//		PID.Kd = 0.05;
/**************************************************************************************
 * FunctionName   : Ems_Boost_Process(void)
 * Description    : EMS ��ѹ
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ems_Boost_Process(void)
{
	//	uint16_t adc_value;
	//	uint8_t flag;
	static float PID_Value;
	//	static uint16_t PID_LastValue;

	PID_Value = 0;
	if ((SysInfo.WorkState == upkeep_mode || SysInfo.Test_Mode.Test_Mode_Flag == ON || SysInfo.Test_Mode.Test_Mode_Flag == Auto_ON) && PID.Flag)
	{
		PID.CurrentVal = filter((uint16_t *)&ADC_Value, 0, ADC_CHANNEL_NUM, ADC_ADD_COUNT);
		PID.TargetVal = EMS_Handle.vol_value;
		// 				// 2023 05 06 当电池电压偏低 + 当前档位为5档时，自动降低为4档ems输出
		// if((5 == SysInfo.Save_Data.upkeep_level) && (BAT_20_40_STATUS ==SysInfo.Batt_Value.State))
		// {
		// 	PID.TargetVal = EMS_VOL_LEVEL4;
		// }				 //---------------------------------------

		PID_Value = PID_IncreaseCalc(&PID);
		PID.temp += PID_Value;

		if (PID.temp < 0)
		{
			PID.temp = 0;
		}

		//if (PID.temp > 2000) //调试时发现当 temp=1950时，升压值会很低
		if (PID.temp > 1900)
		{

			PID.temp = 1700;
			//			#if ARF001
			switch (SysInfo.Save_Data.upkeep_level)
			//			#else
			//			switch(SysInfo.upkeep_level)
			//			#endif
			{
			case 1:
				PID.temp = 260;
				break;
			case 2:
				PID.temp = 360;
				break;
			case 3:
				PID.temp = 470;
				break;
			case 4:
				PID.temp = 580;
				break;
			case 5:
				PID.temp = 580;
				break;
			default:
				break;
			}
		}
	}
	else
	{
		PID.temp = 0;
		PID.CurErr = 0;
		PID.CurrentVal = 0;
		PID.LastErr = 0;
		PID.PreErr = 0;
		//		PID_LastValue = 0;
	}
	EMS_VALUE_SET((uint16_t)PID.temp);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
