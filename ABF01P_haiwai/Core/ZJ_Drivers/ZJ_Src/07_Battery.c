#include "includes.h"

uint16_t ADC_Value[ADC_ADD_COUNT][ADC_CHANNEL_NUM] = {0};
uint16_t Bat_ADC_Value[BAT_ADC_COUNT] = {0};
/**************************************************************************************
 * FunctionName   : USB_Plug_Scan(void)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void USB_Plug_Scan(void) // 10ms����һ��
{
	static uint8_t usb_plug_time;
	static uint8_t usb_no_plug_time;

	if (VIN_DET_IN() && BAT_CHARGE_STATE_IN())
	{
		usb_no_plug_time++;
		if (usb_no_plug_time >= 5)
		{
			if (SysInfo.Batt_Value.Usb_flag)
			{
				SysInfo.Save_Data.save_Data_flag = 1;
				SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;
				//				BOOST_5V_ON();
			}
			SysInfo.Batt_Value.Usb_flag = 0;
			usb_no_plug_time = 5;
		}
		usb_plug_time = 0;
	}
	else
	{
		usb_plug_time++;
		if (usb_plug_time >= 5)
		{
			SysInfo.Batt_Value.Usb_flag = 1;
			usb_plug_time = 5;
			//			BOOST_5V_OFF();
		}
		usb_no_plug_time = 0;
	}
}

/*************************************************************************************
 * FunctionName	 : filter()
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint16_t filter(uint16_t *pData, uint8_t channel, uint8_t len, uint8_t count)
{
	uint16_t value_buf1[ADC_ADD_COUNT] = {0};
	uint8_t i, j, k;
	uint16_t temp;

	pData = pData + channel;
	for (k = 0; k < count; k++)
	{
		value_buf1[k] = (uint16_t)*pData;
		pData = pData + len;
	}
	for (j = 0; j < count - 1; j++)
	{
		for (i = 0; i < count - j - 1; i++)
		{
			if (value_buf1[i] > value_buf1[i + 1])
			{
				temp = value_buf1[i];
				value_buf1[i] = value_buf1[i + 1];
				value_buf1[i + 1] = temp;
			}
		}
	}
	return value_buf1[(count - 1) / 2];
}

/*************************************************************************************
 * FunctionName	:
 * Description		:
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void ADC_DMA_COV_START(void)
{
	BAT_ADC_ENABLE();
	Vref_EN_ON();
	HAL_ADCEx_Calibration_Start(&hadc1);
	// HAL_Delay(10);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&ADC_Value, ADC_CHANNEL_NUM * ADC_ADD_COUNT);
	//	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_Value,ADC_CHANNLE_NUM);
}

/*************************************************************************************
 * FunctionName	:
 * Description		:
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
#if 0
  DMA_Transfer_Complete_Count++;
#endif
	// CHECK_BATT_ADCIN_DIS;
	//  HAL_ADC_Stop_DMA(&hadc1);
	//	SysInfo.ADC_Value.ADC_VEMS_Feedback_VALUE  	= filter((uint16_t *)&ADC_Value,0,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
	////	SysInfo.ADC_Value.ADC_TEMP_VALUE       			= filter((uint16_t *)&ADC_Value,1,ADC_CHANNLE_NUM,ADC_ADD_COUNT);
	////  SysInfo.ADC_Value.ADC_BAT_VALUE       			= filter((uint16_t *)&ADC_Value,2,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
	//  SysInfo.ADC_Value.ADC_RF_VALUE        			= filter((uint16_t *)&ADC_Value,3,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
	//  SysInfo.ADC_Value.ADC_EMS_DET_VALUE        	= filter((uint16_t *)&ADC_Value,4,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
	//  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_Value,ADC_CHANNEL_NUM*ADC_ADD_COUNT);
}
/*************************************************************************************
 * FunctionName	:
 * Description		:
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
uint16_t Get_Battery_Read(uint16_t *ADC_Source, uint16_t Counts)
{
	uint8_t i;
	uint32_t max, min, sum, ad_temp;

	max = 0x00;
	min = 0x0FFF;
	sum = 0x00;

	/* ��ͨ��������10��ת��������ͨ������ */
	for (i = 0; i < Counts; i++)
	{
		ad_temp = ADC_Source[i];
		if (ad_temp > max)
		{
			max = ad_temp;
		}
		if (ad_temp < min)
		{
			min = ad_temp;
		}
		sum += ad_temp;
	}
	sum -= min;
	sum -= max;
  sum /=(Counts-2);
	return sum ; // ȡ128��ƽ��ֵ
}
/**************************************************************************************
 * FunctionName   : uint8_t Charge_Batter_State(uint16_t ChargeBatValue)
 * Description    : 130ms����һ��
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t CheckCharge_Batter_State(uint16_t ChargeBatValue)
{
	static uint16_t TimeCount;
	uint8_t ChargeBattState = 0;

	if (ChargeBatValue <= BAT_VOL_3V55 && (ChargeBatValue > BAT_VOL_3V05)) // 0   2499
	{
		ChargeBattState = BAT_00_00_STATUS;
	}
	else if (ChargeBatValue <= BAT_VOL_3V95 && (ChargeBatValue > BAT_VOL_3V55)) // 0~20   2499 ~ 3154
	{
		ChargeBattState = BAT_00_20_STATUS;
	}
	else if (ChargeBatValue <= BAT_VOL_4V00 && (ChargeBatValue > BAT_VOL_3V95)) // 20~40  3154 ~ 3277
	{
		ChargeBattState = BAT_20_40_STATUS;
	}
	else if (ChargeBatValue <= BAT_VOL_4V05 && (ChargeBatValue > BAT_VOL_4V00)) // 40~60  3277 ~ 3317
	{
		ChargeBattState = BAT_40_60_STATUS;
	}
	else if (ChargeBatValue <= BAT_VOL_4V10 && (ChargeBatValue > BAT_VOL_4V05)) // 60~80  3317 ~ 3359
	{
		ChargeBattState = BAT_60_80_STATUS;
	}
	else if (ChargeBatValue > BAT_VOL_4V10) // 80~100   3359
	{
		ChargeBattState = BAT_80_100_STATUS;

		if (((VIN_DET_IN() == 0) && (BAT_CHARGE_STATE_IN())))
		{
			if (++TimeCount >= 200) // 26s 200*130MS
			{
				TimeCount = 200;
				ChargeBattState = BAT_100_100_STATUS;
			}
		}
	}
	else
		;

	return ChargeBattState;
}
/**************************************************************************************
 * FunctionName   : uint8_t CheckIdle_Batter_State(uint16_t CheckBatValue)
 * Description    : 130ms����һ��
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t CheckIdle_Batter_State(uint16_t CheckBatValue)
{
	uint8_t IdleBattState = 0;
	// #if ARF001
	if (CheckBatValue <= BAT_VOL_3V55 && (CheckBatValue > BAT_VOL_3V05)) // 0
	{
		IdleBattState = BAT_00_00_STATUS;
	}
	else if (CheckBatValue <= BAT_VOL_3V75 && (CheckBatValue > BAT_VOL_3V55)) // 0~20   2908 ~ 2990
	{
		IdleBattState = BAT_00_20_STATUS;
	}
	else if (CheckBatValue <= BAT_VOL_3V85 && (CheckBatValue > BAT_VOL_3V75)) // 20~40  2990 ~ 3072
	{
		IdleBattState = BAT_20_40_STATUS;
	}
	else if (CheckBatValue <= BAT_VOL_3V95 && (CheckBatValue > BAT_VOL_3V85)) // 40~60  3072 ~ 3154
	{
		IdleBattState = BAT_40_60_STATUS;
	}
	else if (CheckBatValue <= BAT_VOL_4V10 && (CheckBatValue > BAT_VOL_3V95)) // 60~80  3154 ~ 3277
	{
		IdleBattState = BAT_60_80_STATUS;
	}
	else if (CheckBatValue <= BAT_VOL_4V40 && (CheckBatValue > BAT_VOL_4V10)) // 80~100 3277 ~ 3400
	{
		IdleBattState = BAT_80_100_STATUS;
	}
	else
		;
	return IdleBattState;
}
/**************************************************************************************
 * FunctionName   : uint8_t CheckWorking_Batter_State(uint16_t WorkBatValue)
 * Description    : 130ms����һ��
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t CheckWorking_Batter_State(uint16_t WorkBatValue)
{
	static uint8_t WorkBattState = BAT_40_60_STATUS;
	static uint8_t BattState_Cnt;

	if (WorkBatValue < BAT_VOL_3V20)
	{
		if (WorkBattState ==BAT_00_20_STATUS || WorkBattState == BAT_00_00_STATUS)
		{
			WorkBattState = BAT_00_00_STATUS;
		}
		else
		{
			WorkBattState = BAT_00_20_STATUS;
		}
	}
	else if (WorkBatValue < BAT_VOL_3V30)
	{
		if ((WorkBatValue >= BAT_VOL_3V20 - BAT_VOL_HYS) && (WorkBatValue <= BAT_VOL_3V30))
		{
			if (++BattState_Cnt > 50)
			{
				BattState_Cnt = 50;
				if (WorkBattState > BAT_00_20_STATUS)
				{
					WorkBattState = BAT_00_20_STATUS;
				}
			}
		}
	}
	else
	{
		if (WorkBattState != BAT_00_20_STATUS && WorkBattState != BAT_00_00_STATUS)
		{
			WorkBattState = BAT_40_60_STATUS;
		}
		BattState_Cnt = 0;
	}
	return WorkBattState;
}

/**************************************************************************************
 * FunctionName   : uint8_t Check_Batter_State(void)
 * Description    : 1ms???????
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t Scan_Batter_State(void)
{
	static uint16_t BatValue;
	static uint8_t BattState, BatteryFlag = 1;
	static uint16_t TimeCount;
	static uint16_t BattCount = 0;

	if (++BattCount <= BAT_ADC_COUNT)
	{
		if (!SysInfo.Montor_Flag)
		{
			Bat_ADC_Value[BattCount - 1] = filter((uint16_t *)&ADC_Value, 2, ADC_CHANNEL_NUM, ADC_ADD_COUNT);
		}
		else
		{
			BattCount--;
		}
		return BattState;
	}
	else
	{
		BattCount = 0;
		BatValue = Get_Battery_Read((uint16_t *)&Bat_ADC_Value, BAT_ADC_COUNT);
		SysInfo.Test_Mode.BatValue = BatValue;
		if (BatteryFlag && SysInfo.Batt_Value.Usb_flag)
		{
			BatteryFlag = 0;
			if (SysInfo.Save_Data.BattState != BAT_00_00_STATUS)
			{
				SysInfo.Save_Data.BattState = CheckIdle_Batter_State(BatValue);
			}

//			printf("\n\r SaveBatValue=%d\n\r", SysInfo.Save_Data.BattState);
		}
		//		printf ("\n\r BatValue=%d\n\r",BatValue);
	}

	//	if(SysInfo.Power_Value.state == System_ON || SysInfo.Test_Mode.Test_Mode_Flag!=OFF)  //ϵͳ����״̬��ؼ��
	if (SysInfo.Power_Value.state == System_ON) // ϵͳ����״̬��ؼ��
	{
		BattState = CheckWorking_Batter_State(BatValue);
		if(SysInfo.Save_Data.BattState <= BAT_00_20_STATUS)
		{
			if(BattState!=BAT_00_00_STATUS)
			{
				BattState = SysInfo.Save_Data.BattState;
			}
		}
	}
	else if (SysInfo.Batt_Value.Usb_flag) // ����ؼ��
	{
		//		printf ("\n\r charge\n\r");
		//		BattState = CheckCharge_Batter_State(BatValue);
		if (BatValue <= BAT_VOL_3V55 && (BatValue > BAT_VOL_3V05)) // 0   2499
		{
			BattState = BAT_00_00_STATUS;
		}
		else if (BatValue <= BAT_VOL_3V95 && (BatValue > BAT_VOL_3V55)) // 0~20   2499 ~ 3154
		{
			BattState = BAT_00_20_STATUS;
		}
		else if (BatValue <= BAT_VOL_4V05 && (BatValue > BAT_VOL_3V95)) // 20~40  3154 ~ 3277
		{
			BattState = BAT_20_40_STATUS;
			TimeCount = 0;
		}
		else if (BatValue <= BAT_VOL_4V10 && (BatValue > BAT_VOL_4V05)) // 40~60  3277 ~ 3317
		{
			BattState = BAT_40_60_STATUS;
		}
		else if (BatValue <= BAT_VOL_4V17 && (BatValue > BAT_VOL_4V10)) // 60~80  3317 ~ 3359
		{
			BattState = BAT_60_80_STATUS;
		}
		else if (BatValue > BAT_VOL_4V17) // 80~100   3359
		{
			BattState = BAT_80_100_STATUS;
			//			TimeCount = 0 ;
		}
		else
			;

		if (((VIN_DET_IN() == 0) && (BAT_CHARGE_STATE_IN())) && BatValue > BAT_60_80_STATUS)
		{
			if (++TimeCount >= 200) // 26s 200*130MS
			{
				TimeCount = 200;
				BattState = BAT_100_100_STATUS;
			}
		}
	}
	else // ������ѯ
	{
		if (BatValue <= BAT_VOL_3V55 && (BatValue > BAT_VOL_3V05)) // 0
		{
			BattState = BAT_00_00_STATUS;
		}
		else if (BatValue <= BAT_VOL_3V75 && (BatValue > BAT_VOL_3V55)) // 0~20   2908 ~ 3113
		{
			BattState = BAT_00_20_STATUS;
		}
		else if (BatValue <= BAT_VOL_3V85 && (BatValue > BAT_VOL_3V75)) // 20~40  3113 ~ 3236
		{
			BattState = BAT_20_40_STATUS;
		}
		else if (BatValue <= BAT_VOL_3V95 && (BatValue > BAT_VOL_3V85)) // 40~60  3236 ~ 3317
		{
			BattState = BAT_40_60_STATUS;
		}
		else if (BatValue <= BAT_VOL_4V10 && (BatValue > BAT_VOL_3V95)) // 60~80  3317 ~ 3400
		{
			BattState = BAT_60_80_STATUS;
		}
		else if (BatValue <= BAT_VOL_4V40 && (BatValue > BAT_VOL_4V10)) // 80~100 3400 ~ 3604
		{
			BattState = BAT_80_100_STATUS;
		}
		else
			;
	}

	return BattState;
}
/**************************************************************************************
 * FunctionName   : uint8_t Check_Batter_State(void)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t Check_Batter_State(void) // 1ms???????
{
	//	static uint8_t BattCnt=0;
	static uint16_t State;
	static uint32_t TimeCnt1;

	if (++TimeCnt1 >= 1000) // 10s
	{
		State = 1;
		TimeCnt1 = 0;
	}
	else
	{
		State = 0;
	}

	//	if((SysInfo.Power_Value.state == System_ON) || (SysInfo.Batt_Value.Power_Display_Flag)) //������ѯ����״̬�������
	if (SysInfo.Power_Value.state == System_ON || SysInfo.Test_Mode.Test_Mode_Flag != OFF) // ����״̬�������
	{
		SysInfo.Batt_Value.State = Scan_Batter_State();
	}
	else if (SysInfo.Batt_Value.Usb_flag && State) // ���״̬ �������  10s����һ��״̬
	{
		State = 0;
		TimeCnt1 = 0;
		SysInfo.Batt_Value.State = Scan_Batter_State();
	}
	else
		;
	return SysInfo.Batt_Value.State;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
