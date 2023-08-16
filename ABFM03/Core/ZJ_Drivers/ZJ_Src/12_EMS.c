#include "includes.h"
#include "ems.h"

// 123
DAC_DEVICE_t EMS_dac_dev ={
	.iic_addr = MCP4726_A0_ADDR,
	.dac_probe.p_init = DAC_MCP4726_Init,
	.dac_probe.p_read = DAC_Device_ReadData,
	.dac_probe.p_write = DAC_MCP4726_OUT,
};
//extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim1;

_EMS_HANDLE_TypeDef EMS_Handle;
//_PID_HANDLE_TypeDef PID;
extern I2C_HandleTypeDef hi2c2;

#if ARF001 //�»Դｵ��
	#define EMS_Freq        500     //���� ʱ��
	#define duty            25		      //���� 30
	#define interval        35       //�����������ʱ��
	#define channel_p_start 5 //
	#define channel_p_stop  channel_p_start+duty
	#define channel_n_start channel_p_stop+interval
	#define channel_n_stop  channel_n_start+duty
#else
	#define EMS_Freq 500     //���� ʱ��
	#define duty 30		      //���� 30
	#define interval 35       //�����������ʱ��
	#define channel_p_start 5 //
	#define channel_p_stop  channel_p_start+duty
	#define channel_n_start channel_p_stop+interval
	#define channel_n_stop  channel_n_start+duty

#endif

static uint16_t EMS_Freq_Value =  EMS_Freq ;
uint8_t data[3];
//static uint16_t EMS_duty_Value =  duty ;
//static uint16_t EMS_interval_Value =  interval ;
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
  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
 // HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

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
//HAL_TIM_PWM_Stop(&htim16,TIM_CHANNEL_1);
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
	TIM1->CR1 &= ~(1<<0);
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
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);
	//HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
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
	EMS_PWR_EN_OFF();
}
/**************************************************************************************
* FunctionName   : Ems_Boost_On(void)
* Description    : ��Դ����
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Ems_Boost_On(void )
{
	EMS_PWR_EN_ON();
}

/**************************************************************************************
* FunctionName   : acquire_adc_value(uint8_t channel, uint16_t* data)
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
uint16_t acquire_adc_value(uint8_t channel, uint16_t* data)
{
	static uint8_t Counts ,state=0;
	static uint16_t max =0xff, min=0x00, data_temp =0x00;
	static uint16_t sum;

	data_temp = filter((uint16_t *)&ADC_Value, channel, ADC_CHANNEL_NUM, ADC_ADD_COUNT);

  	if(++Counts > 10)
	{
		if(data_temp > max)
		{
			max = data_temp;
		}
		if (data_temp < min)
		{
			min = data_temp;
		}
		sum += data_temp;
		state =0;
	}
	else
	{
		sum -= min;
		sum -= max;
		*data = sum>>3;

		Counts =0;
		state =1;
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
	switch(PSC)
	{
		case EMS_1KHZ_FREQ:
		{
			TIM1->PSC = (uint32_t)31;
			EMS_Freq_Value = 1000;    // 1k HZ
			break;

		}

		case EMS_2KHZ_FREQ:
		{
			TIM1->PSC = (uint32_t)31;
			EMS_Freq_Value = 500;  //850  2K HZ
	//				EMS_Freq_Value = 330;  //650
			break;

		}

		case EMS_3KHZ_FREQ:
		{
			TIM1->PSC = (uint32_t)31;
			EMS_Freq_Value = 333;  //   3K HZ
//				EMS_Freq_Value = 500;  //850
			break;

		}

		case EMS_4KHZ_FREQ:
		{
			TIM1->PSC = (uint32_t)31;
			EMS_Freq_Value = 250;  //500
			break;
		}

		default:
		{
			TIM1->PSC = (uint32_t)31;
			EMS_Freq_Value = EMS_Freq;
			break;
		}
	}
}
/**************************************************************************************
* FunctionName   : Set_Ems_level(void)
* Description    : ����EMS��λ
* EntryParameter : None
* ReturnValue    : None  25ms 调用一次
**************************************************************************************/
void Set_Ems_Value(_EMS_HANDLE_TypeDef* EmsLevel,uint8_t StepValue)
{
	static uint16_t Sum_Value;
	if(SysInfo.Power_Value.state == System_ON && SysInfo.WorkState == upkeep_mode)
	{
		if(100==StepValue)
		{
			Sum_Value = (EmsLevel->vol_value)*(StepValue -2);
		}
		else
		{
			Sum_Value = (EmsLevel->vol_value-20)*(StepValue+20) + 1950;
		}
		EMS_Handle.CurrentVal = filter((uint16_t *)&ADC_Value, Vems_ADC_CHANNEL, ADC_CHANNEL_NUM, ADC_ADD_COUNT);
		if(EMS_Handle.CurrentVal < 80 && SysInfo.Skin_Touch_Flag) //
		{
			EMS_dac_dev.dac_probe.p_init();
		}
		EMS_dac_dev.dac_probe.p_write(Sum_Value);
	}
	else
	{
		Sum_Value =0; //
	}
}
/**************************************************************************************
* FunctionName   : Change_EMS_Freq(void)
* Description    : ����EMS��λ
* EntryParameter : None
* ReturnValue    : None  10ms 运行一次
**************************************************************************************/
void Change_EMS_Freq(void)
{
	static uint8_t EMS_FREQ = 0, Last_EMS_FREQ = 1;

	if(EMS_Handle.Run_Flag)
	{
		EMS_Handle.EMS_Cnt++;
		 //EMS��ѹ2s�ڷ�ֵ�仯 EMS_Handle.vol_value+1V -> EMS_Handle.vol_value-> EMS_Handle.vol_value-1V ��ѯ
		if(EMS_Handle.EMS_Cnt <= EMS_1000_ms)
		{
			EMS_FREQ = EMS_1KHZ_FREQ;
		}
		else if(EMS_Handle.EMS_Cnt <= EMS_1500_ms)
		{
			EMS_FREQ = EMS_2KHZ_FREQ;
		}
		else if(EMS_Handle.EMS_Cnt <= EMS_2000_ms)
		{
			EMS_FREQ = EMS_3KHZ_FREQ;
		}
		else
		{
			EMS_FREQ = EMS_1KHZ_FREQ;
			EMS_Handle.EMS_Cnt = 0;
		}
	}
	else
	{
		EMS_FREQ = 0;
		EMS_Handle.EMS_Cnt = 0;
	}

	if((SysInfo.Test_Mode.Test_Mode_Flag)&&(TILA_MODE_EMS_TEST == SysInfo.Test_Mode.STATUS))
	{
		switch(SysInfo.Test_Mode.Set_EMS_Freq)  //ems 频率测试时，变频测试
		{
			case EMS_1KHZ_FREQ:
			{
				EMS_FREQ = EMS_1KHZ_FREQ;
				break;
			}
			case EMS_2KHZ_FREQ:
			{
				EMS_FREQ = EMS_2KHZ_FREQ;
				break;
			}
			case EMS_3KHZ_FREQ:
			{
				EMS_FREQ = EMS_3KHZ_FREQ;
				break;
			}

			case EMS_ALL_FREQ:
			default:
			{
				break;
			}
		}
	}

	if(Last_EMS_FREQ != EMS_FREQ)
	{
		Last_EMS_FREQ = EMS_FREQ;
		EMS_Frequency_conversion(EMS_FREQ);
	}

}
/**************************************************************************************
* FunctionName   :Ems_Value_Add(_EMS_HANDLE_TypeDef* EmsLevel)
* Description    : ��EMS��ѹ�ֵ�
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Ems_Value_Add(_EMS_HANDLE_TypeDef* EmsLevel)
{
	if(EmsLevel->vol_value >= Level_1 && EmsLevel->vol_value < Level_20)
	{
		EmsLevel->vol_value = Level_20;
		SysInfo.upkeep_level = Level_1;
	}
	else if(EmsLevel->vol_value >= Level_20 && EmsLevel->vol_value < Level_25)
	{
		EmsLevel->vol_value = Level_25;
		SysInfo.upkeep_level = Level_2;
	}
	else if(EmsLevel->vol_value >= Level_25 && EmsLevel->vol_value <= Level_30)
	{
		EmsLevel->vol_value = Level_30;
		SysInfo.upkeep_level = Level_3 ;
	}
	else;
}
/**************************************************************************************
* FunctionName   :Ems_Value_Add(_EMS_HANDLE_TypeDef* EmsLevel)
* Description    : ��EMS��ѹ�ֵ�
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Ems_Value_Reduce(_EMS_HANDLE_TypeDef* EmsLevel)
{
    if(EmsLevel->vol_value >= Level_21 && EmsLevel->vol_value <= Level_25)
	{
		EmsLevel->vol_value = Level_20;
		SysInfo.upkeep_level = Level_1 ;
	}
	else if(EmsLevel->vol_value > Level_25 && EmsLevel->vol_value <= Level_30)
	{
		EmsLevel->vol_value = Level_25;
		SysInfo.upkeep_level = Level_2 ;
	}
	else;
}

/**************************************************************************************
* FunctionName   :upkeep_RF_Level(_EMS_HANDLE_TypeDef* EmsLevel)
* Description    : RF��λ����
* EntryParameter : None
* ReturnValue    : Nonep
**************************************************************************************/
void upkeep_RF_Level(_EMS_HANDLE_TypeDef* EmsLevel)
{
    if(EmsLevel->vol_value >= Level_None && EmsLevel->vol_value <= Level_20)
	{
		SysInfo.upkeep_level = Level_1;

	}
    else if(EmsLevel->vol_value >= Level_21 && EmsLevel->vol_value <= Level_25)
	{
		SysInfo.upkeep_level = Level_2;

	}
	else if(EmsLevel->vol_value > Level_25 && EmsLevel->vol_value <= Level_30)
	{
		SysInfo.upkeep_level = Level_3;

	}
	else;
	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
}

/**************************************************************************************
* FunctionName   : Ems_DeInit(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Ems_DeInit(void)
{
	EMS_Handle.Run_Flag = 0;
	EMS_Pwm_Stop();
}

/**************************************************************************************
* FunctionName   : Ems_dac_probe(void)
* Description    :
* EntryParameter : None  首先probe_iic芯片
* ReturnValue    : None
**************************************************************************************/
void Ems_dac_probe(void)
{
	DAC_MCP4726_probe();
	DAC_SGM5355_Probe();
}
/**************************************************************************************
* FunctionName   : Ems_Init(void)
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Ems_Init(void)
{
	IRled_start();
	Ems_Boost_On();
	EMS_Handle.Run_Flag = 1;
	// PID.Flag = 1;
	VBAT_OUT_ON(); //
	EMS_Pwm_Start();
	EMS_dac_dev.dac_probe.p_init();
}

// /*********************************************************
//  ������: PID_IncreaseCalc
//  ��  ��: ����ʽPID
//  ����ֵ:
//  ���ֵ: ��
//  ����ֵ: ��
//  **********************************************************/
// int PID_IncreaseCalc(_PID_HANDLE_TypeDef *vPID)
// {
// 	// int PID_Out=0;
// 	// float pValue=0;
// 	// float iValue=0;
// 	// float dValue=0;

// 	// vPID->CurErr = vPID->TargetVal-vPID->CurrentVal;   //��ǰƫ��

// 	// pValue = (vPID->Kp)*(vPID->CurErr-vPID->LastErr);
// 	// iValue = (vPID->Ki)*vPID->CurErr;
// 	// dValue = (vPID->Kd)*(vPID->CurErr-2*vPID->LastErr+vPID->PreErr);

// 	// PID_Out = pValue+iValue+dValue;

// 	// vPID->PreErr = vPID->LastErr;
// 	// vPID->LastErr = vPID->CurErr;  //��ǰƫ������һ��ƫ��

//     return PID_Out;
// }

//		PID.Kp = 0.9;
//		PID.Ki = 0.1;
//		PID.Kd = 0.05;
/**************************************************************************************
* FunctionName   : Ems_Boost_Process(void)
* Description    : EMS ��ѹ
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
// void Ems_Boost_Process(void)
// {
////	uint16_t adc_value;
////	uint8_t flag;
//	static float PID_Value;
////	static uint16_t PID_LastValue;
//
//	PID_Value = 0;
//	if((SysInfo.WorkState == upkeep_mode || SysInfo.Test_Mode.Test_Mode_Flag == ON\
//		||SysInfo.Test_Mode.Test_Mode_Flag == Auto_ON)&& PID.Flag)
//	{
//		PID.CurrentVal = 	filter((uint16_t *)&ADC_Value,Vems_ADC_CHANNEL,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
//		PID.TargetVal = EMS_Handle.vol_value;
//		PID_Value = PID_IncreaseCalc(&PID);
//		PID.temp += PID_Value;
//
//		if(PID.temp<0)
//		{
//			PID.temp = 0;
//		}
//
//		if(PID.temp>2000)
//		{

////			PID.temp = 1700;
////			#if ARF001
//			switch(SysInfo.Save_Data.upkeep_level)
////			#else
////			switch(SysInfo.upkeep_level)
////			#endif
//			{
//				case 1:	PID.temp = 260; break;
//				case 2: PID.temp = 360; break;
//				case 3: PID.temp = 470; break;
//				case 4: PID.temp = 580; break;
//				case 5: PID.temp = 700; break;
//				default :break;
//			}
//		}

//	}
//	else
//	{
//		PID.temp = 0;
//		PID.CurErr=0;
//		PID.CurrentVal=0;
//		PID.LastErr=0;
//		PID.PreErr=0;
////		PID_LastValue = 0;
//	}
//		EMS_VALUE_SET((uint16_t)PID.temp);
// }

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/




