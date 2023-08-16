#include "includes.h"
#include "skin.h"


#define REPAIR_RF_SHOR_POWER  1400//1000//1500 //150000
#define UPKEEP_RF_SHOR_POWER  1400//1000//1500 //50000

/*************************************************************************************
* FunctionName	: Skin_Touch_Scan(void)
* Description		: Ƥ���Ӵ���� ��ҪΪ��Ƶ�Ӵ����
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void Skin_Touch_Scan(void)  //10ms����һ��
{
	static uint16_t no_touch_filter_count=0 ;
#if ARF001
	;
#else
	if(RF_Handle.Flag)
#endif
	{
		if(SysInfo.Skin_Touch_RF_Flag)
		{
			no_touch_filter_count = 0;
			SysInfo.Skin_Touch_Flag = 1; //ϵͳ�Ӵ���־λ
		}
		else
		{
			no_touch_filter_count++;
			if(no_touch_filter_count > 10) //
			{
				no_touch_filter_count = 500;
				SysInfo.Skin_Touch_Flag = 0;
			}
		}
	}
	SysInfo.Skin_Touch_Flag = 1; //2023 08 02  debug ,测试波形
}

/*************************************************************************************
* FunctionName	: Check_Idle_Time(void)
* Description		: 60SƤ���޽Ӵ���ģʽ��60S�����л� ϵͳ�ػ�
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
uint8_t  Check_Idle_Time(void) //10ms����
{
	uint8_t TimeOut =0;
//	static uint8_t Montor_Flag=0;
	static uint8_t No_Touch_Cnt;

	if(SysInfo.Power_Value.state == System_ON)
	{
		if(SysInfo.Skin_Touch_Flag) //��ʱ��ʼ
		{
			if(!SysInfo.Montor_Flag &&SysInfo.Skin_Touch_Montor_Flag)
			{
				No_Touch_Cnt++;
			}
			else
			{
				No_Touch_Cnt = 0;
			}

			if((SysInfo.Idle_Run_Cnt|| No_Touch_Cnt > 100) && SysInfo.Skin_Touch_Montor_Flag)
			{
				SysInfo.Skin_Touch_Montor_Flag = 0;
				SysInfo.Montor_Flag =1; //����񶯱�־
				SysInfo.StayTime = 20;
				No_Touch_Cnt = 0;
			}
		}
		if(SysInfo.MotionStateFlage)   // 2023 03 20 g_sensor �˶���ʱ��
		{                                    //  �Ž�idle��0. ����Ӵ����أ����ƶ�ʱ��һֱ����0
			SysInfo.Idle_Run_Cnt = 0;
		}
		else
		{
			SysInfo.Idle_Run_Cnt ++;
			if(SysInfo.Idle_Run_Cnt > Leave_Skin_Timer)  // 6000
			{
				SysInfo.Idle_Run_Cnt = Leave_Skin_Timer;
				TimeOut = 1;	//���̽���
			}
		}
	}
	return TimeOut;
}



/*************************************************************************************
* FunctionName	: skin_rf_scan(void)
* Description		: ��ȡADC��ֵ
* EntryParameter:
* ReturnValue		: 1ms����1��
**************************************************************************************/
static void read_RF_ADC_value(void)
{
	static uint16_t Skin_Touch_Data[10];  // BAT_ADC_COUNT
	static uint8_t read_adc_cnt = 0;
	static  uint8_t print_cnt =0;
	uint8_t print_buff[2] = 0;
	uint16_t tmp;
	uint8_t  i;

	if(0 == RF_Handle.Read_ADC_Flag)
	{
		return;
	}
	if((0 == RF_Handle.Flag) &&(SysInfo.Power_Value.state == System_ON))
	{
		return;
	}

// if(SysInfo.Power_Value.state != System_SHORT_CIRCUIT_PROTECT)

	if(read_adc_cnt < 10)
	{
		if(!SysInfo.Montor_Flag)
		{
			Skin_Touch_Data[read_adc_cnt]=ADC_Value[read_adc_cnt][Skin_ADC_CHANNEL];
			read_adc_cnt ++;
		}
	}
	else
	{
		tmp = 0;
		for(i = 0; i < 10; i++)
		{
			tmp +=  Skin_Touch_Data[i];
		}
		tmp /= 10;
		read_adc_cnt = 0;
		RF_Handle.ADC_value = tmp;

		if(dac_param.self_cal == 0x7F)  // 2023 08 03 新增，adc额外自动校准
		{
			if(RF_Handle.ADC_value > dac_param.offset)
			{
				RF_Handle.ADC_value -= dac_param.offset;
			}
		}


	}

}

/*************************************************************************************
* FunctionName	: skin_rf_scan(void)
* Description		: RFƤ�����
* EntryParameter:
* ReturnValue		:1ms����1��
**************************************************************************************/
static void skin_Touch_check(void)
{
	static uint8_t RF_Count = 0;
	static uint8_t RF_Over_Count = 0,SystemON_2s_Cnt;
	static uint8_t task_10ms_cnt = 0;


	if(SysInfo.Power_Value.state !=System_OFF|| SysInfo.Test_Mode.Test_Mode_Flag != OFF)  //����2s����Ƥ��
	{
		task_10ms_cnt ++;
		if(0 !=task_10ms_cnt % 10)   // 10ms cnt
		{
			return;
		}
		task_10ms_cnt = 0;
		SystemON_2s_Cnt++;

		if(SystemON_2s_Cnt >= 200)   // 2s cnt
		{
			SystemON_2s_Cnt = 200;

			if(RF_Handle.Flag) //RF�������ж��Ƿ�Ӵ�Ƥ��
			{
				if(RF_Handle.ADC_value >= RF_TOUCH_THRESHOLD) //�Ӵ�Ƥ��
				{
					if(++RF_Over_Count>= 20)
					{
						SysInfo.Skin_Touch_RF_Flag  = 1; //RF��Ƶ�Ӵ���־λ
						RF_Over_Count =0;
					}
					RF_Count = 0;
				}
				else
				{
					if(++RF_Count >= Skin_Remove_Cnt * 10) //�뿪Ƥ��
					{
						SysInfo.Skin_Touch_RF_Flag = 0;
						RF_Count = 0;
						RF_Over_Count = 0;
					}
				}
			}
		}
	}
	else
	{
		SystemON_2s_Cnt = 0;
	}
}
/*************************************************************************************
* FunctionName	: skin_rf_scan(void)
* Description		:
  RF ��·�����������
*   �����أ��޸е���С�� 20ŷķ��ʱ���ж�Ϊ��·
*     ���أ��޸е���> = 50ŷķ��ʱ�� �ָ����

*           25ŷķʱ-----------ADֵԼΪ��1500
* EntryParameter:       2.5v  --- 4096
*
* ReturnValue		: 1ms����1��
**************************************************************************************/
static void skin_short_circuit_check(void)
{
	static uint8_t short_circuit_cnt = 0;
	static uint16_t normal_circuit_cnt = 0;

	static uint8_t last_station =0;

	if(RF_Handle.ADC_value >= REPAIR_RF_SHOR_POWER) //��·����
	{
		short_circuit_cnt ++;
		if(short_circuit_cnt > 200)
		{
			short_circuit_cnt = 200;
			if(SysInfo.Power_Value.state != System_SHORT_CIRCUIT_PROTECT)
			{

				 /********** 	�ر�EMS��غ���************/
					Ems_Osc_Off();
					Ems_Boost_Off();
					EMS_Pwm_Stop();
					RF_Osc_Off();	 //�ر���ƵPWM���
					//RF_Stop();
				last_station = SysInfo.Power_Value.state;
				SysInfo.Power_Value.state = System_SHORT_CIRCUIT_PROTECT;
			}
			//printf (" RF_ADC_value =%d, enter _short _circu \n\r", RF_Handle.ADC_value);
			normal_circuit_cnt = 0;
		}
	}
	else if(SysInfo.Power_Value.state == System_SHORT_CIRCUIT_PROTECT)     // ��·�����ָ� 1s��Żָ�
	{
		normal_circuit_cnt ++;
		if(normal_circuit_cnt > 1000)
		{
			normal_circuit_cnt = 1000;
			SysInfo.Power_Value.state = last_station;
		}
		short_circuit_cnt = 0;
	}
}

/*************************************************************************************
* FunctionName	: skin_rf_scan(void)
* Description		: RFƤ�����
* EntryParameter:
* ReturnValue		://1ms����1��
**************************************************************************************/
void Skin_RF_Scan(void)
{
	read_RF_ADC_value();
	skin_Touch_check();
	skin_short_circuit_check();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


