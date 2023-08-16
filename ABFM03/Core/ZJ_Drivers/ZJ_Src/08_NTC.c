
#include "includes.h"


//��C                                       -15 -10 -5   0   5    10   15   20   25   30   35   40   45   50   55
//const uint16_t temp_table[TEMP_TAB_INDEX]={480,588,712,851,1004,1169,1345,1528,1715,1901,2085,2264,2435,2596,2746};    //5.0V

//��C                                       -15 -10 -5   0   5    10   15   20   25   30   35   40   45   50   55
//const uint16_t temp_table[TEMP_TAB_INDEX]={485,594,719,860,1014,1180,1358,1543,1732,1920,2106,2287,2459,2622,2773};    //5.05V

/***********y***************************************************************************
* FunctionName   : Get_Temp()
* Description    : ��ȡ�¶ȴ�������ֵ
* EntryParameter :
* Description    : --
* ReturnValue    : �¶�ֵ(��λ:��.)
**************************************************************************************/
const float Ka = 273.15;
const float T2 = (273.15 + 25.0);//T2
float Get_Temp(float Rt, float Rp, float Bx)
{
  float temp;
  temp = Rt/Rp;
  temp = log(temp);
  temp /= Bx;
  temp += (1/T2);
  temp = 1/(temp);
  temp -= Ka;
  return temp;
}
/*************************************************************************************
* FunctionName	 : NTC_Temp_Check()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
//int8_t NTC_Temp_Check(void)
//{
////	uint16_t ntc_ad_value;
//	uint8_t i;
//	uint16_t temp_ad_step;
//	int8_t temp;
//	static uint16_t ntc_ad_value;
//
//	temp = 0;
//	temp_ad_step = 0;

//	ntc_ad_value = filter((uint16_t *)&ADC_Value,NTC_ADC_CHANNEL,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
//
//	for(i=0;i<TEMP_TAB_INDEX;i++)
//	{
//		if(ntc_ad_value <= temp_table[i])
//		{
//			break;
//		}
//	}
//	if(i == 0)
//	{
//		temp = -15;
//	}
//	else if((i>0)&&(i<TEMP_TAB_INDEX))
//	{
//		temp_ad_step = (temp_table[i] -  temp_table[i-1])/5;
//
//		if((ntc_ad_value > temp_table[i-1])&&(ntc_ad_value <= temp_table[i-1]+temp_ad_step))
//		{
//			temp = (i-1)*5 + 1 + (-15);
//		}
//		else if((ntc_ad_value > temp_table[i-1]+temp_ad_step)&&(ntc_ad_value <= temp_table[i-1]+ 2*temp_ad_step))
//		{
//			temp = (i-1)*5 + 2 + (-15);
//		}
//		else if((ntc_ad_value > temp_table[i-1]+ 2*temp_ad_step)&&(ntc_ad_value <= temp_table[i-1]+ 3*temp_ad_step))
//		{
//			temp = (i-1)*5 + 3 + (-15);
//		}
//		else if((ntc_ad_value > temp_table[i-1]+ 3*temp_ad_step)&&(ntc_ad_value <= temp_table[i-1]+ 4*temp_ad_step))
//		{
//			temp = (i-1)*5 + 4 + (-15);
//		}
//		else
//		{
//			temp = (i-1)*5 + 5 + (-15);
//		}
//	}
//	else
//	{
//		temp = 55;
//	}
//
//	return temp;
//
//
//}

/*************************************************************************************
 * FunctionName	 : NTC_Temp_compensation()
 * Description    : �¶Ȳ���
 * EntryParameter :
 * ReturnValue    : None  MAX_OFFSET = 0.714°C
 **************************************************************************************/
void NTC_Temp_compensation(_Sys_Info_TypeDef *Temp, float Compensation_Value)
{
	if (Temp->NTC_Temp > 25 && Temp->NTC_Temp <= 42)
	{
		Temp->NTC_Temp += (Temp->NTC_Temp - 25) * Compensation_Value; // 0.042  0.7V
	}
	else if (Temp->NTC_Temp > 42)
	{
		Temp->NTC_Temp += Compensation_Value * 17;
	}
}

#define ENABLE_AUTO_ADJ_NTC_CHECK
/*************************************************************************************
* FunctionName	 : NTC_Temp_Running() 123
* Description    :
* EntryParameter :
* ReturnValue    : None  ----
* NTC �쳣ʱ�¶�ֵ��
*               �Ͽ�ʱ ----- �¶� ԼΪ-87��C
*               ��·ʱ ----- �¶�ԼΪ 197��C
* �ж��쳣��׼�� ���¶� <= -25��C ���¶� >=100��Cʱ����ΪNTC�쳣�¶�

**************************************************************************************/
//	static uint8_t Error_state;
void NTC_Temp_Running(void)		//1ms����һ��
{
	static uint32_t test_count=0;
//	int8_t current_temp;
	static uint16_t NTC_Error_Cnt;
	uint32_t  ntc_value,Rt_Value;
	static float Last_NTC_Temp=0,diff_NTC_Temp=0;
	static uint32_t diff_value_check_count=0;

	ntc_value = filter((uint16_t *)&ADC_Value, NTC_ADC_CHANNEL, ADC_CHANNEL_NUM, ADC_ADD_COUNT)*0.61035; // 2500/4096
	Rt_Value =((5110 - ntc_value) * 3600 / ntc_value) - 3600; // 10K B=3380

	SysInfo.NTC_Temp = Get_Temp(Rt_Value, 10000, 3380);  //10K B=3380

	SysInfo.NTC_Temp += SysInfo.NTC_offset;   // 增加偏差量
	NTC_Temp_compensation(&SysInfo, 0.042);  // MAX_OFFSET = 0.714°C

	if(SysInfo.Skin_Touch_Flag)
	{
		SysInfo.NTC_Temp += 5;  // 20230719 实测 电极片温度升温特别快，
							 //NTC 在壳体表面的中心点，正上方没有电极片
	}

#ifdef ENABLE_AUTO_ADJ_NTC_CHECK
	//����NTC�¶ȱ仯�������Ȳ�ǿ�������ӱ仯�ʷ����жϣ����ӷ�Ӧ�ָ���
	if(++diff_value_check_count>=500)
	{
		diff_NTC_Temp=SysInfo.NTC_Temp-Last_NTC_Temp;
		Last_NTC_Temp=SysInfo.NTC_Temp;
		diff_value_check_count=0;
	}

	if((SysInfo.NTC_Temp > OVER_TEMP && SysInfo.NTC_Temp < 100 ) && (diff_NTC_Temp>0 || SysInfo.NTC_Temp >= 43))//if(SysInfo.NTC_Temp >= OVER_TEMP && SysInfo.NTC_Temp < 200 )
	{
		SysInfo.OverTemp_Flag = 1; //����

		if(SysInfo.NTC_Temp >= ERROR_TEMP) //�����¶ȱ���
		{
			SysInfo.System_Err_status = System_OverTemp_Fault;
		}
	}
	else if(((SysInfo.NTC_Temp <= NORMAL_TEMP) && (SysInfo.NTC_Temp > (-25))) ||((diff_NTC_Temp<=0) && (SysInfo.NTC_Temp < 43)&&(SysInfo.NTC_Temp > 10)))//else if((SysInfo.NTC_Temp < NORMAL_TEMP) && (SysInfo.NTC_Temp > (-25)))
	{
		if(SysInfo.OverTemp_Flag)
		{
			SysInfo.OverTemp_Flag = 0; //�ָ�����
			RF_Handle.Run_Flag = 1;
		}

		if(SysInfo.System_Err_status != System_LowPower_Fault)
		{
			SysInfo.System_Err_status = System_Normal;
		}
		NTC_Error_Cnt = 0;
	}
	else ;
#else

	if(SysInfo.NTC_Temp >= OVER_TEMP && SysInfo.NTC_Temp < 200 )
	{
		SysInfo.OverTemp_Flag = 1; //����

		if(SysInfo.NTC_Temp >= ERROR_TEMP) //�����¶ȱ���
		{
			SysInfo.System_Err_status = System_OverTemp_Fault;
		}
	}
	else if((SysInfo.NTC_Temp < NORMAL_TEMP) && (SysInfo.NTC_Temp > (-25)))
	{
		SysInfo.OverTemp_Flag = 0; //�ָ�����
		if(SysInfo.System_Err_status != System_LowPower_Fault)
		{
			SysInfo.System_Err_status = System_Normal;
		}
		NTC_Error_Cnt = 0;
	}
	else ;
#endif



	//if((SysInfo.NTC_Temp <= (-25) || SysInfo.NTC_Temp>=100 ) && SysInfo.Power_Value.state == System_ON) //����������
	if((SysInfo.NTC_Temp <= (-25) || SysInfo.NTC_Temp>=100 ))
	{
		if(++NTC_Error_Cnt > 100)
		{
			NTC_Error_Cnt = 0;
			SysInfo.System_Err_status = System_NTC_Fault;
		}
		// printf (" SysInfo.NTC_Temp =%f, err_cnt = %d\n\r", SysInfo.NTC_Temp, NTC_Error_Cnt);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



