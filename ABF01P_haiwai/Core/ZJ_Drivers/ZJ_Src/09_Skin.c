#include "includes.h"
#include "skin.h"

/*************************************************************************************
 * FunctionName	: Skin_Touch_Scan(void)
 * Description		: Ƥ���Ӵ���� ��ҪΪ��Ƶ�Ӵ����
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
void Skin_Touch_Scan(void) // 10ms����һ��
{
	static uint16_t no_touch_filter_count = 0;
#if (ARF001 == DEVICE_R1_RPO)
	;
#elif ((ARF001 == DEVICE_R1_RPO_MAX)||(ARF001 == DEVICE_R1_HAIWAI))
	if (RF_Handle.Flag || SysInfo.Restore_Flag)
#endif
	{
		if (SysInfo.Skin_Touch_RF_Flag)
		{
			no_touch_filter_count = 0;
			SysInfo.Skin_Touch_Flag = 1; // ϵͳ�Ӵ���־λ
		}
		else
		{
			no_touch_filter_count++;
			if (no_touch_filter_count > 500)
			{
				no_touch_filter_count = 500;
				SysInfo.Skin_Touch_Flag = 0;
			}
		}
	}
}

/*************************************************************************************
 * FunctionName	: Cosmetic_Time_Count(void)
 * Description		: 60SƤ���޽Ӵ���ģʽ��60S�����л� ϵͳ�ػ�
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
uint8_t Cosmetic_Time_Count(void) // 10ms����
{
	uint8_t TimeOut = 0;
	//	static uint8_t Montor_Flag=0;
	static uint8_t No_Touch_Cnt;

	if (SysInfo.Power_Value.state == System_ON)
	{
		if (SysInfo.Skin_Touch_Flag) // ��ʱ��ʼ
		{
			if (!SysInfo.Montor_Flag && SysInfo.Skin_Touch_Montor_Flag)
			{
				No_Touch_Cnt++;
			}
			else
			{
				No_Touch_Cnt = 0;
			}

			if ((SysInfo.Skin_No_Touch_Timer || No_Touch_Cnt > 100) && SysInfo.Skin_Touch_Montor_Flag)
			{
				//				if(!SysInfo.Montor_Flag &&SysInfo.Skin_Touch_Montor_Flag ) //������һ�νӴ���Ƥ��
				//				{
				SysInfo.Skin_Touch_Montor_Flag = 0;
				SysInfo.Montor_Flag = 1; // ����񶯱�־
				SysInfo.StayTime = 20;
				No_Touch_Cnt = 0;

				//				}
			}

			SysInfo.Skin_No_Touch_Timer = 0;
			TimeOut = 0;
			//			Montor_Flag =0;
		}
		else
		{
			SysInfo.Skin_No_Touch_Timer++;
			//			Montor_Flag =1;
			if (SysInfo.Skin_No_Touch_Timer > 6000) // 6000
			{
				SysInfo.Skin_No_Touch_Timer = 0;
				TimeOut = 1; // ���̽���
			}
		}
	}
	else
	{
		//		Montor_Flag =0;
		;
	}
	return TimeOut;
}
/*************************************************************************************
 * FunctionName	: skin_rf_scan(void)
 * Description		: RFƤ�����
 * EntryParameter:
 * ReturnValue		:
 **************************************************************************************/
uint8_t Skin_RF_Scan(void) // 1ms����1��
{
	static uint16_t Skin_RF_Value = 0;
	static uint8_t RF_Count = 0, ShinCount = 0;
	static uint8_t RF_Over_Count = 0, SystemON_2s_Cnt;
	static uint16_t Skin_Touch_Data[BAT_ADC_COUNT];

	if (++ShinCount <= BAT_ADC_COUNT)
	{
#if (ARF001 == DEVICE_R1_RPO)
		if (!SysInfo.Montor_Flag)
#elif ((ARF001 == DEVICE_R1_RPO_MAX)||(ARF001 == DEVICE_R1_HAIWAI))
		if ((RF_Handle.ADC_Flag && !SysInfo.Montor_Flag) || SysInfo.Restore_Flag)
#endif
		{
			Skin_Touch_Data[ShinCount - 1] = filter((uint16_t *)&ADC_Value, 3, ADC_CHANNEL_NUM, ADC_ADD_COUNT);
		}
		else
		{
			ShinCount--;
		}
		return 0;
	}
	else
	{
		ShinCount = 0;
		Skin_RF_Value = Get_Battery_Read((uint16_t *)&Skin_Touch_Data, BAT_ADC_COUNT);
		HAL_ADCEx_Calibration_Start(&hadc1);
		//		Skin_RF_Value = Skin_filter((uint16_t *)&Skin_Touch_Data,BAT_ADC_COUNT);
	}

	if (SysInfo.Power_Value.state != System_OFF || SysInfo.Test_Mode.Test_Mode_Flag != OFF) // ����2s����Ƥ��
	{
		SystemON_2s_Cnt++;
	}
	else
	{
		SystemON_2s_Cnt = 0;
	}
	if (SystemON_2s_Cnt >= 15)
	{
		SystemON_2s_Cnt = 15;
		//			#if ARF001
		//				;
		//			#else
		if (RF_Handle.Flag || SysInfo.Restore_Flag) // RF�������ж��Ƿ�Ӵ�Ƥ��
		//			#endif
		{
			//			Skin_RF_Value = filter((uint16_t *)&ADC_Value,3,ADC_CHANNEL_NUM,ADC_ADD_COUNT);

			if (Skin_RF_Value >= RF_TOUCH_THRESHOLD) // �Ӵ�Ƥ��
			{
				if (++RF_Over_Count >= 2)
				{
					SysInfo.Skin_Touch_RF_Flag = 1; // RF��Ƶ�Ӵ���־λ
					RF_Over_Count = 0;
				}
				RF_Count = 0;
			}
			else
			{
				if (++RF_Count >= Skin_Remove_Cnt) // �뿪Ƥ��
				{
					SysInfo.Skin_Touch_RF_Flag = 0;
					RF_Count = 0;
					RF_Over_Count = 0;
				}
			}
		}
	}
	return 1;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
