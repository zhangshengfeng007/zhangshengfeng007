
#include "includes.h"

// ??C                                       -15 -10 -5   0   5    10   15   20   25   30   35   40   45   50   55
// const uint16_t temp_table[TEMP_TAB_INDEX]={480,588,712,851,1004,1169,1345,1528,1715,1901,2085,2264,2435,2596,2746};    //5.0V

// ??C                                       -15 -10 -5   0   5    10   15   20   25   30   35   40   45   50   55
const uint16_t temp_table[TEMP_TAB_INDEX] = {485, 594, 719, 860, 1014, 1180, 1358, 1543, 1732, 1920, 2106, 2287, 2459, 2622, 2773}; // 5.05V

/*************************************************************************************
 * FunctionName	 : NTC_Temp_Check()
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
int8_t NTC_Temp_Check(void)
{
	//	uint16_t ntc_ad_value;
	uint8_t i;
	uint16_t temp_ad_step;
	int8_t temp;
	static uint16_t ntc_ad_value;

	temp = 0;
	temp_ad_step = 0;

	ntc_ad_value = filter((uint16_t *)&ADC_Value, NTC_CHANNEL, ADC_CHANNEL_NUM, ADC_ADD_COUNT);

	for (i = 0; i < TEMP_TAB_INDEX; i++)
	{
		if (ntc_ad_value <= temp_table[i])
		{
			break;
		}
	}
	if (i == 0)
	{
		temp = -15;
	}
	else if ((i > 0) && (i < TEMP_TAB_INDEX))
	{
		temp_ad_step = (temp_table[i] - temp_table[i - 1]) / 5;

		if ((ntc_ad_value > temp_table[i - 1]) && (ntc_ad_value <= temp_table[i - 1] + temp_ad_step))
		{
			temp = (i - 1) * 5 + 1 + (-15);
		}
		else if ((ntc_ad_value > temp_table[i - 1] + temp_ad_step) && (ntc_ad_value <= temp_table[i - 1] + 2 * temp_ad_step))
		{
			temp = (i - 1) * 5 + 2 + (-15);
		}
		else if ((ntc_ad_value > temp_table[i - 1] + 2 * temp_ad_step) && (ntc_ad_value <= temp_table[i - 1] + 3 * temp_ad_step))
		{
			temp = (i - 1) * 5 + 3 + (-15);
		}
		else if ((ntc_ad_value > temp_table[i - 1] + 3 * temp_ad_step) && (ntc_ad_value <= temp_table[i - 1] + 4 * temp_ad_step))
		{
			temp = (i - 1) * 5 + 4 + (-15);
		}
		else
		{
			temp = (i - 1) * 5 + 5 + (-15);
		}
	}
	else
	{
		temp = 55;
	}
	return temp;
}
/**************************************************************************************
 * FunctionName   : Get_Temp()
 * Description    : ��ȡ�¶ȴ�������ֵ
 * EntryParameter :
 * Description    : --
 * ReturnValue    : �¶�ֵ(��λ:��.)
 **************************************************************************************/
const float Ka = 273.15;
const float T2 = (273.15 + 25.0); // T2
float Get_Temp(float Rt, float Rp, float Bx)
{
	float temp;
	temp = Rt / Rp;
	temp = log(temp);
	temp /= Bx;
	temp += (1 / T2);
	temp = 1 / (temp);
	temp -= Ka;
	return temp;
}
/*************************************************************************************
 * FunctionName	 : NTC_Temp_compensation()
 * Description    : �¶Ȳ���
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t NTC_Temp_compensation(_Sys_Info_TypeDef *Temp, float Compensation_Value)
{
	if (Temp->NTC_Temp > 25 && Temp->NTC_Temp <= 42)
	{
		Temp->NTC_Temp += (Temp->NTC_Temp - 25) * Compensation_Value; // 0.042  0.7V
	}
	else if (Temp->NTC_Temp > 42)
	{
		Temp->NTC_Temp += Compensation_Value * 17;
	}
	else
		;
	return 1;
}

/*************************************************************************************
 * FunctionName	  : NTC_Temp_compensation()
 * Description    : ģʽ����λ�¶Ȳ���
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
uint8_t Gear_NTC_Temp_compensation(_Sys_Info_TypeDef *Temp)
{
	if (Temp->WorkState == upkeep_mode)
	{
		switch (Temp->EMS_level)
		{
		case Level_None:
			NTC_Temp_compensation(Temp, 0.042);
			break;
		case Level_1:;
			break;
		case Level_2:;
			break;
		case Level_3:;
			break;
		case Level_4:;
			break;
		case Level_5:;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (Temp->repair_level)
		{
		case Level_None:
			NTC_Temp_compensation(Temp, 0.042);
			break;
		case Level_1:;
			break;
		case Level_2:;
			break;
		case Level_3:;
			break;
		case Level_4:;
			break;
		case Level_5:;
			break;
		default:
			break;
		}
	}
	return 1;
}
/*************************************************************************************
 * FunctionName	 : NTC_Temp_Running()
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void NTC_Temp_Running(void) // 1ms����һ��
{
	//	int8_t current_temp;
	uint32_t ntc_value, Rt_Value;
//  static uint8_t OverTemp=42,StopTemp=45,NormalTemp =40;
//	static uint16_t over_temp_cnt = 0;
	uint8_t NormalTemp =40;

	ntc_value = filter((uint16_t *)&ADC_Value, NTC_CHANNEL, ADC_CHANNEL_NUM, ADC_ADD_COUNT) * 0.61035; // 2500/4096
	Rt_Value = ((5110 - ntc_value) * 3600 / ntc_value) - 3600;										   // ����ͬһ��·��������ȣ�����ó�NTC����ֵ

	SysInfo.NTC_Temp = Get_Temp(Rt_Value, 10000, 3380); // 10K B=3380
	NTC_Temp_compensation(&SysInfo, 0.042);				// �¶Ȳ���

  if(SysInfo.Temp_Protect_Flage) //�������뿪Ƥ��10S�������¶ȼ�1��
	{
		SysInfo.NTC_Temp += 1;
		if(SysInfo.NTC_Temp<NORMAL_TEMP)
		{
			SysInfo.Temp_Protect_Flage =0;
		}
	}
#if TEMP_TEST_MODE
	if ((SysInfo.NTC_Temp > OVER_TEMP) && (SysInfo.NTC_Temp < STOP_TEMP))
	{
    if(SysInfo.OverTemp_Flag != 2) //����45��ֹͣ�����ֱ���¶�С��42�㣬�ٿ�ʼ�������
    {
			SysInfo.OverTemp_Flag = 1; // ����
		}
		SysInfo.NTC_higher_45_cnt = 0;
	}
  else if((SysInfo.NTC_Temp < OVER_TEMP) && (SysInfo.NTC_Temp >= NORMAL_TEMP))
	{
    if(SysInfo.OverTemp_Flag == 2) //����45��ֹͣ�����ֱ���¶�С��42�㣬�ٿ�ʼ�������
    {
				SysInfo.OverTemp_Flag = 1; // ????
		}
		SysInfo.NTC_higher_45_cnt = 0;
	}
	else if(SysInfo.NTC_Temp > STOP_TEMP)
	{
		if(SysInfo.OverTemp_Flag != 2)
		{
			SysInfo.NTC_higher_45_cnt ++;
			if(SysInfo.NTC_higher_45_cnt > 500)
			{
				SysInfo.NTC_higher_45_cnt = 0;
				if(SysInfo.OverTemp_Flag != 2)
				{
					SysInfo.OverTemp_Flag = 2;
				}
			}
		}
	}
	else if ((SysInfo.NTC_Temp < NORMAL_TEMP) && (SysInfo.OverTemp_Flag == 1))
	{
		SysInfo.OverTemp_Flag = 0; // ???????
		SysInfo.NTC_higher_45_cnt = 0;
	}
	else
		;
#else

	if ((SysInfo.NTC_Temp > OVER_TEMP) && (SysInfo.OverTemp_Flag == 0))
	{
		SysInfo.OverTemp_Flag = 1; // ����
	}
	else if ((SysInfo.NTC_Temp < NORMAL_TEMP) && (SysInfo.OverTemp_Flag == 1))
	{
		SysInfo.OverTemp_Flag = 0; // �ָ�����
	}
	else
		;

#endif
#if AGEING_TEST
 	SysInfo.MotionStateFlage = 1;  //????
  SysInfo.OverTemp_Flag = 0;     // �ָ�����
#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
