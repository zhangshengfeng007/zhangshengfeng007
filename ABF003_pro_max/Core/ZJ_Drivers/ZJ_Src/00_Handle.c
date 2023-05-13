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

// �������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
// ��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
// ����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
// �ض���fputc����
int fputc(int ch, FILE *f)
{
	while ((USART1->ISR & 0X40) == 0)
		; // ѭ������,ֱ���������
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

	if (SysInfo.Batt_Value.Usb_flag) // ���ģʽ
	{
		if (++ChargeCnt >= 50)
		{
			if (!Flag) // USB���븴λ�����һ�ε���
			{
				Led_Value.Level = SysInfo.Save_Data.BattState;
				BOOST_5V_OFF();
			}
			Flag = 1;
			Led_Value.StayTime = 50;
			Led_Value.Mode = Batt_Charging;
			if (ChargeCnt > 6000) // 60sˢ��һ��
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
		if (SysInfo.Batt_Value.Power_Display_Flag) // ��ѯ����
		{
			BAT_ADC_ENABLE();
			if (++Cnt >= 26)
			{
				SysInfo.Batt_Value.Power_Display_Flag = 0;
				Cnt = 0;
				SysInfo.Save_Data.BattState = *(__IO uint32_t *)(EEPROM_STRAT_ADDR + 128); // ��ȡ�ػ�ǰ�ĵ���
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
		else if (SysInfo.Power_Value.state == System_ON || SysInfo.Flag) // ��λָʾ��
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
		else if (!SysInfo.Batt_Value.Usb_flag && Flag) // �����ģʽ ����״̬�£���ʾ��λ��
		{
			Flag = 0;
			Led_Value.StayTime = 100;
			Led_Value.Mode = Batt_Normal;
			Led_Value.state = 1;
		}
		else if (SysInfo.Power_Value.BattState) // ������ѯ���� ����ȱ�粻����
		{
			Batt_Cnt++;
			if (Batt_Cnt > 13)
			{
				Batt_Cnt = 0;
				//				SysInfo.Save_Data.BattState  = *(__IO uint32_t*)( EEPROM_STRAT_ADDR+128 );		//��ȡ�ػ�ǰ�ĵ���
				Batt_State = SysInfo.Batt_Value.State;
				if ((SysInfo.Save_Data.BattState == BAT_00_00_STATUS) || (Batt_State == BAT_00_00_STATUS)) // ȱ�磬�ػ�
				{
					//					Led_Value.Mode = Batt_Low_0 ; //ȱ�磬1��LED��1HzƵ����˸5��
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
		Write_Parameter_To_Eeprom(); // �ػ����浵λ
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
	/**********�ر�EMS��غ���************/
	//	Ems_Osc_Off();
	Ems_Boost_Off();
	EMS_Pwm_Stop();

	/**********�ر�RF��غ���************/
	RF_Osc_Off();
	RF_Stop();
	//	VBAT_OUT_OFF();
	Set_Mbi5020_Out(0);
}
/**************************************************************************************
 * FunctionName   : System_Standby_Run(void)
 * Description    : ����״̬���뿪Ƥ����60s���Զ������ػ���������һ�νӴ���Ƥ����������
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void System_Standby_Run(void)
{
	uint8_t Get_60s_Flag = 0;
	static uint8_t Batt_Cnt;
	Get_60s_Flag = Cosmetic_Time_Count();
	if (Get_60s_Flag) // 60s�޽Ӵ����ػ�
	{
		Get_60s_Flag = 0;
		SysInfo.Sleep_Flag = 1; // �ػ���־λ
		//			Write_Parameter_To_Eeprom();  //�ػ����浵λ
	}
	if (SysInfo.Batt_Value.State == BAT_00_00_STATUS && SysInfo.Power_Value.state == System_ON) // ȱ�磬�ػ�
	{
		if (++Batt_Cnt > 20)
		{
			SysInfo.Sleep_Flag = 1; // �ػ���־λ
			Batt_Cnt = 0;
			SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;
		}
	}
	else
	{
		Batt_Cnt = 0;
	}
	//		if(SysInfo.Skin_Touch_Montor_Flag && SysInfo.Skin_Touch_Flag) //������һ�νӴ���Ƥ��
	//		{
	//			SysInfo.Skin_Touch_Montor_Flag = 0;
	//			SysInfo.Montor_Flag = 1; //������������
	//			SysInfo.StayTime = 20;	 //��ʱ��0.2s
	//		}
}
/**************************************************************************************
* FunctionName   : Vibration_Reminder_Counts_Run(void)
* Description    : �����Ѽ�ʱ
1����������ģʽ���������ѣ�120s��240s;ģʽ�������:300s���������ģʽ��60s�޲�������������ģʽ��
2�������޸�ģʽ���������ѣ�240s��480s;ģʽ�������:600s���������ģʽ��60s�޲�������������ģʽ��
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Vibration_Reminder_Counts_Run(void) // 10ms����һ��
{
	static uint8_t LockFlag, Error_Time_Flag;
	static uint16_t StandyCnt,NoTouch_Cnt;

	if (SysInfo.Skin_Touch_Flag)
	{
		NoTouch_Cnt = 0;
		if (RF_Handle.Run_Flag) // RF���м�ʱ
		{
			if (++SysInfo.Mode_Cnt >= Timer_RF) // 1s
			{
				SysInfo.Mode_Cnt = 0;
				LockFlag = 0x01;

				if (SysInfo.WorkState == upkeep_mode)
				{
					SysInfo.Mode_Switch_Flag = 0x02; // EMSģʽ����
				}
				else
				{
					SysInfo.Mode_Switch_Flag = 0x01; // RFģʽ����
				}
			}
		}
#if ARF001
		if (EMS_Handle.Run_Flag) // EMS���м�ʱ
		{
			if (++SysInfo.Mode_Cnt >= Timer_EMS) // 2s
			{
				SysInfo.Mode_Cnt = 0;
				LockFlag |= 0x02;
				SysInfo.Mode_Switch_Flag = 0x01; // RFģʽ����
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
		SysInfo.Mode_Switch_Flag = 0x01; // RFģʽ����
		SysInfo.Mode_Cnt = 0;
		LockFlag = 0;
    if(++NoTouch_Cnt > 1000 )
		{
			NoTouch_Cnt = 0;
      if(SysInfo.Power_Value.state == System_ON)
			{
				SysInfo.Temp_Protect_Flage = 1; //�������뿪Ƥ��10s �����¶ȼ�2���־λ
			}

		}
	}
	/************************************************************************************************
1����������ģʽ���������ѣ�120s��240s;ģʽ�������:300s���������ģʽ��60s�޲�������������ģʽ��
2�������޸�ģʽ���������ѣ�240s��480s;ģʽ�������:600s���������ģʽ��60s�޲�������������ģʽ��
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
			SysInfo.Montor_Flag = 1; // ������������
			SysInfo.StayTime = 20;	 // ��ʱ��0.2s
		}
		else
		{
			if ((SysInfo.Reminder_Cnt == EMS_Reminder_300S && SysInfo.WorkState == upkeep_mode) ||
				(SysInfo.Reminder_Cnt == RF_Reminder_600S && SysInfo.WorkState == repair_mode))
			{
				SysInfo.Reminder_Cnt = 0;
				SysInfo.Montor_Flag = 1; // ������������
				SysInfo.StayTime = 100;	 // ��ʱ��1.0s
				SysInfo.Mode_Switch_Flag = 0x03;
				Error_Time_Flag = 0;
//				SysInfo.Skin_Touch_Flag = 0; // ϵͳ�Ӵ���־λ
			}
		}
	}

	if (SysInfo.Mode_Switch_Flag == 0x03)
	{
		if (++StandyCnt > 5)
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			SysInfo.Sleep_Flag = 1; // �ػ���־λ
		}

		if (SysInfo.Test_Mode.Ageing_Mode == 0x01)
		{
			StandyCnt = 0;
			SysInfo.Mode_Switch_Flag = 0x01;
			SysInfo.Sleep_Flag = 0; // �ػ���־λ
		}
	}
	else
	{
		StandyCnt = 0;
	}
}
/**************************************************************************************
 * FunctionName   : Set_OverTemp_Parameter(void)
 * Description    :  ���²�������
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Set_OverTemp_Parameter(void)
{
	static uint16_t TimeCnt_30s, Mode_Cnt;
//  static uint8_t Restore_Flag;
	if (SysInfo.StayTime_30s_Flag) // �л���λ��,�ڸõ�λά��30s,�������¶ȹ��ߣ������л���λһֱά��1������ ��Ҫ���EMS
	{
		if (++TimeCnt_30s > 3000)
		{
			TimeCnt_30s = 0;
			SysInfo.StayTime_30s_Flag = 0;
		}
	}

	if (!SysInfo.OverTemp_Flag && !SysInfo.Check_Protect_Flage) // ���º�ָ������������ָ�����
	{
		if (SysInfo.WorkState == upkeep_mode)
		{
			SysInfo.upkeep_level = SysInfo.Save_Data.upkeep_level;
			// if (SysInfo.Power_Value.state == System_ON)
			// {
			// 	Upkeep_LED_ON();
			// }
			// else
			// {
			// 	if (SysInfo.Test_Mode.Test_Mode_Flag != ON)
			// 	{
			// 		Upkeep_LED_OFF();
			// 	}
			// }
		}
		else if (SysInfo.WorkState == repair_mode)
		{
			SysInfo.repair_level = SysInfo.Save_Data.repair_level;
			// if (SysInfo.Power_Value.state == System_ON)
			// {
			// 	Repair_LED_ON();
			// }
			// else
			// {
			// 	if (SysInfo.Test_Mode.Test_Mode_Flag != ON)
			// 	{
			// 		Repair_LED_OFF();
			// 	}
			// }
		}
		// else
		// 	;

		if (!SysInfo.Check_Protect_Flage) // ��⵽�������ƶ�
		{
			// if (SysInfo.Power_Value.state == System_ON)
			// {
			// 	IRled_start();
			// }
		}
		Mode_Cnt = 0;
    if(SysInfo.Restore_Flag && SysInfo.Power_Value.state == System_ON)
		{
			SysInfo.Restore_Flag = 0;
			SysInfo.Mode_Switch_Flag = 0x01 ;
		}
	}
	//	else if(SysInfo.OverTemp_Flag && !SysInfo.StayTime_30s_Flag)//���²�������
	else if (SysInfo.OverTemp_Flag || SysInfo.Check_Protect_Flage) // ���²������� �� ������δ�ƶ�����ɵ�ǰ�������������RF��Ϊ1����EMS��������ģʽ�Ƽ�����������1S����
	{
#if TEMP_TEST_MODE
    if(SysInfo.OverTemp_Flag == 0x01 && !SysInfo.Check_Protect_Flage)
		{
				if (SysInfo.WorkState == upkeep_mode) // ��������ģʽ
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
		else if(SysInfo.OverTemp_Flag == 0x02)
		{
			if(SysInfo.Power_Value.state == System_ON)
			{
				RF_DeInit();
				Ems_DeInit();
				SysInfo.Restore_Flag = 1 ;
			}
		}
	#if SENSOR_MOVE
		else if(SysInfo.Check_Protect_Flage)
		{
			if(SysInfo.Power_Value.state == System_ON)
			{
				RF_DeInit();
				Ems_DeInit();
				SysInfo.Restore_Flag = 1 ;
			}
		}
	#endif


#else
//		if(SysInfo.OverTemp_Flag == 0x01 ||SysInfo.Check_Protect_Flage )
//		{
			RF_DeInit();
			Ems_DeInit();
      SysInfo.Restore_Flag = 1 ;
//		}


#endif
		if (SysInfo.Power_Value.state == System_ON && SysInfo.Batt_Value.State != BAT_00_20_STATUS) // ģʽ�Ʊ�������
		{
			// if(SysInfo.Skin_Touch_Flag)
			// {
			// if(SysInfo.OverTemp_Flag == 0x02) // 20230512 ?????????45??c?????u???
			// {
			// 	Mode_Twink1hz_Display(50);
			// }

			// }
			// else
			// {
				// if (SysInfo.WorkState == repair_mode)
				// {
				// 	Repair_LED_ON();
				// }
				// else
				// {
				// 	Upkeep_LED_ON();
				// }
					// IRled_start();
			// }

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
 * Description   :����ģʽ
 * EntryParameter:
 * ReturnValue   :
 **************************************************************************************/
void SLIDE_MODE_Run(void)
{
		static uint8_t Lis2dInit_Flag = 0;
	//	SysInfo.MotionStateFlage = LIS3DH_get_angle(ddata.AXIS_X, ddata.AXIS_Y, ddata.AXIS_Z);//����
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
	LIS3DH_get_angle(ddata.AXIS_X, ddata.AXIS_Y, ddata.AXIS_Z); // ����
#endif
	if (SysInfo.Test_Mode.Test_Mode_Flag == ON)
	{
		SysInfo.MotionStateFlage = 1; // ����
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
//		if(SysInfo.Period_Flag)  //��������״̬�´�����δ�ƶ�����ɵ�ǰ�������������RF��Ϊ1����EMS��������ģʽ�Ƽ�����������1S����
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
		System_Standby_Run();				  // 60s�޽Ӵ��ػ�����һ�νӴ�Ƥ��������
		if (SysInfo.Mode_Switch_Flag == 0x01) // RF��ʼ��
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			Ems_DeInit();
			RF_Init();
		}
		else if (SysInfo.Mode_Switch_Flag == 0x02) // EMS��ʼ��
		{
			RF_DeInit();
			Ems_Init();
		}
		else if (SysInfo.Mode_Switch_Flag == 0x03) // �������ģʽ
		{
			RF_Call_Data();
			IRled_stop(); // �ر�������
			RF_DeInit();
			Ems_DeInit();
			VBAT_DET_ON(); // ʹ�� V33V5020
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
		System_Standby_Run();														// 60s�޽Ӵ��ػ�����һ�νӴ�Ƥ��������
		if ((SysInfo.Mode_Switch_Flag == 0x01 || SysInfo.Mode_Switch_Flag == 0x02)) // RF��EMS��ʼ��
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			RF_Init();
			if (SysInfo.Skin_Touch_Flag)
			{
				Ems_Init();
			}
		}
		else if (SysInfo.Mode_Switch_Flag == 0x03) // �������ģʽ
		{
			RF_Call_Data();
			IRled_stop();  // �ر�������
			RF_DeInit();
			Ems_DeInit();
			VBAT_DET_ON(); // ʹ��V33V5020
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
* Description    : SysInfo.Montor_Flag:�����񶯱�־λ
				   SysInfo.StayTime����ʱ��
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void RF_Procedure_Run(void)
{
#if ARF001
	if (SysInfo.WorkState == repair_mode && SysInfo.Power_Value.state == System_ON) // �����޸�ģʽ
	{
		System_Standby_Run();				  // 60s�޽Ӵ��ػ�����һ�νӴ�Ƥ��������
		if (SysInfo.Mode_Switch_Flag == 0x01) // RF��ʼ��
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			Ems_DeInit();
			RF_Init();
		}
		else if (SysInfo.Mode_Switch_Flag == 0x03) // �������ģʽ
		{
			RF_Call_Data();
			IRled_stop(); // �ر�������
			RF_DeInit();
			Ems_DeInit();
			VBAT_DET_ON(); // ʹ�� V33V5020
			VBAT_OUT_OFF();
			RF_EN_DOWN();
			PID.Flag = 0;
			SysInfo.Power_Value.state = System_Standy;
		}
		else
			;
	}

#else
	if (SysInfo.WorkState == repair_mode && SysInfo.Power_Value.state == System_ON) // �����޸�ģʽ
	{
		System_Standby_Run();				  // 60s�޽Ӵ��ػ�����һ�νӴ�Ƥ��������
		if (SysInfo.Mode_Switch_Flag == 0x01) // RF��ʼ��
		{
			SysInfo.Mode_Switch_Flag = 0x00;
			Ems_DeInit();
			RF_Init();
		}
		else if (SysInfo.Mode_Switch_Flag == 0x03) // �������ģʽ
		{
			RF_Call_Data();
			IRled_stop(); // �ر�������
			RF_DeInit();
			Ems_DeInit();
			VBAT_DET_ON(); // ʹ�� V33V5020
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

	//	if(((SysInfo.Power_Value.state == System_OFF ) && (!SysInfo.Batt_Value.Usb_flag)))  //����͹��ļ�ʱ��10S
	if ((SysInfo.Power_Value.Enter_Sleep_Flag) && (!SysInfo.Batt_Value.Usb_flag)) // ����͹��ļ�ʱ��10S
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
	// ����UART1��Rx����ΪEXITģʽ
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // �ر�ע������Ҫʹ���ж�ģʽ
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/**************************************************************************************
 * FunctionName   : Enter_Sleep_Mode(void)
 * Description    : ��������ģʽ
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
	HAL_ADC_Stop_IT(&hadc1); /// ADCת������
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_UART_DMAStop(&huart1);
	HAL_ADC_MspDeInit(&hadc1);
	HAL_SPI_MspDeInit(&hspi1);
	HAL_I2C_MspDeInit(&hi2c2);
	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);


	EMS_Pwm_Stop();
	RF_Osc_Off();

	MX_GPIO_DISInit();


	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn); // ʹ��KEY_ON_OFF_Pin�ж�
	Rx_Init();
		///˯��
//		__HAL_RCC_PWR_CLK_ENABLE();   //ʹ��PWRʱ��
//		SysTick->CTRL = 0x00;//�رն�ʱ��
//		SysTick->VAL = 0x00;//���val,��ն�ʱ��
//		HAL_DBGMCU_EnableDBGStopMode();
//		  /* ����ֹͣģʽ�����õ�ѹ������Ϊ�͹���ģʽ���ȴ��жϻ��� KEY2 �����½��ػ���*/
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
 * Description    : �˳�˯��ģʽ
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
	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn); // �ر��ⲿ�ж�KEY_ON_OFF_Pinʹ��
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);

	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);

	SysInfo.Power_Value.BattState = 0x01; // ������ص�ѹ����־
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
	Pole_Change(); // ��ת�缫
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

		if (SysInfo.Test_Mode.Test_Mode_Flag == ON || SysInfo.Test_Mode.Auto_Mode_Level == 0x04) // ����ģʽ��������
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
	Led_Process_Run(); // LEDָʾ������
	NTC_Temp_Running();
	Skin_Touch_Scan();

#if ARF001
	Vibration_Reminder_Counts_Run(); // ��������
	Set_OverTemp_Parameter();		     // ���±�������������
	Set_Ems_level(&SysInfo);
#else
	Vibration_Reminder_Counts_Run(); // ��������
	Set_OverTemp_Parameter();		     // ���±�������������
#endif

	EMS_Procedure_Run(); // ��������
	RF_Procedure_Run();	 // �����޸�

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

	Frequency_conversion_Process(); // ��Ƶ
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
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn); // �ر��ⲿ�ж�ʹ��
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	Led_Value.Level = 0x5a;
	Read_Current_Level_For_Eeprom();
	BOOST_5V_OFF();
	PID.Kp = 1.5;
	PID.Ki = 0.8;
	PID.Kd = 0.1;
	PID.Constant = 50;
	PID.temp = 500;
	//printf("\n\r ==========\n\r");
	printf("\n\r version:%s\n\r", version);
	//printf("\n\r ----------\n\r");
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
