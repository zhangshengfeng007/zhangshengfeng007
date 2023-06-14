#include "includes.h"

_Sys_Info_TypeDef SysInfo;
uint8_t Continue_Times = 5;

typedef enum
{
	_KEY_NONE,
	_REENTER,
	_NO_REENTER,
} _Key_REENTER;

void Key_Press_Process(_Key_Input key);
void Key_PutUp_Process(_Key_Input key);
void Key_Press_Long_Process(_Key_Input key);
void Key_Press_Continue(_Key_Input key);
/**************************************************************************************
 * FunctionName   : Key_Scan
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
_Key_Input Key_Scan(void)
{
	_Key_Input key_set = KEY_None;

	if (Key_Scap_Read_In == GPIO_PIN_RESET)
	{
		key_set = KEY_MSCAP;
	}
	else if (Key_Plus_Read_In == GPIO_PIN_RESET)
	{
		key_set = KEY_PLUS;
	}
	else if (Key_minus_Read_In == GPIO_PIN_RESET)
	{
		key_set = KEY_MINUS;
	}
	else
	{
		key_set = KEY_None;
	}
	return key_set;
}
/**************************************************************************************
 * FunctionName   : KeyDownCallBack
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
uint8_t KeyDownCallBack(uint16_t Key, uint16_t Times)
{
	switch (Key)
	{
	case KEY_MSCAP:
		if (Times < 100)
		{
			return _REENTER;
		}
		break;
	case KEY_PLUS:
		if (Times < 150)
		{
			return _REENTER;
		}
		break;
	case KEY_MINUS:
		if (Times < 50)
		{
			return _REENTER;
		}
		break;
	default:
		break;
	}
	return _NO_REENTER;
}
/**************************************************************************************
 * FunctionName   : Key_Press_Process
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Key_Press_Process(_Key_Input key)
{
	switch (key)
	{
	default:
		break;
	}
}
/**************************************************************************************
 * FunctionName   : Key_PutUp_Process
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Key_PutUp_Process(_Key_Input key)
{

	switch (key)
	{
	case KEY_MSCAP:
		Key_S1_Str();
		break;
	case KEY_PLUS:
		Key_S2_Str();
		break;
	case KEY_MINUS:
		Key_S3_Str();
		break;
	default:
		break;
	}
}
/**************************************************************************************
 * FunctionName   : Key_Press_Long_Process
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Key_Press_Long_Process(_Key_Input key)
{
	switch (key)
	{
	case KEY_MSCAP:
		Key_S1_Long_Str();
		break;
	default:
		break;
	}
}
/**************************************************************************************
 * FunctionName   : Key_Press_Continue
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Key_Press_Continue(_Key_Input key)
{
	switch (key)
	{
	case KEY_PLUS:
		Key_S2_Str();
		break;
	case KEY_MINUS:
		Key_S3_Str();
		break;
	default:
		break;
	}
}
/**************************************************************************************
 * FunctionName   : Key_Call_Data
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Key_Call_Data(void)
{
	SysInfo.Save_Data.save_mode_count = 0;
	SysInfo.upkeep_level = 0;
	SysInfo.repair_level = 0;
	SysInfo.Skin_No_Touch_Timer = 0;
}
/**************************************************************************************
 * FunctionName   : Key_S1_Str
 * Description    : ?????????????????????????????????????????��?
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Key_S1_Str(void) //
{
	if (SysInfo.Power_Value.state == System_ON || SysInfo.Power_Value.state == System_Standy)
	{
		RF_LED_DOWN();
		EMS_LED_UP();
		if (++SysInfo.WorkState > upkeep_mode)
		{
			SysInfo.WorkState = repair_mode;
			RF_LED_UP();
			EMS_LED_DOWN();
		}
		SysInfo.Montor_Flag = 1; // ?????????
		SysInfo.StayTime = 50;
		SysInfo.Skin_Touch_Montor_Flag = 1; // RF??????????��????????????????��
		SysInfo.Power_Value.state = System_ON;
		SysInfo.Skin_No_Touch_Timer = 0;
		SysInfo.Save_Data.WorkState = SysInfo.WorkState;
		SysInfo.Skin_Touch_Flag = 0;
		SysInfo.Skin_Touch_RF_Flag = 0;
		SysInfo.Freq_Cnt = 0;
		if (SysInfo.Power_Value.state == System_Standy) // ?????????????��??????????
		{
			SysInfo.Heating_Flag = 0x01; // ?????????5?????��???��??????��??��
		}
		SysInfo.Mode_Switch_Flag = 0x01; // RF??????
		SysInfo.Reminder_Cnt = 0;
		SysInfo.Temp_Protect_Flage = 1; //���������¶ȼ�2���־λ
	}
	else // ???????
	{
		VBAT_DET_ON();							   // ???ADC??????
		SysInfo.Batt_Value.Power_Display_Flag = 1; // ??????????
		SysInfo.Power_Value.BattState = 0;
		SysInfo.Power_Value.state = System_OFF;
		SysInfo.Sleep_Counts = 0;
	}
}
/**************************************************************************************
 * FunctionName   : Key_S1_Long_Str
 * Description    : ?????????
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Key_S1_Long_Str(void)
{
	//	uint8_t Flag=0;
	if (SysInfo.Power_Value.state == System_OFF)
	{
		//if ((SysInfo.Batt_Value.State == BAT_00_00_STATUS) || (SysInfo.Save_Data.BattState == BAT_00_00_STATUS))
		// #if (USE_BAT_SELECT == USE_5PIN_NEW_BAT)
			if (SysInfo.Batt_Value.State < BAT_00_20_STATUS)
		// #elif(USE_BAT_SELECT == USE_4PIN_OLD_BAT)
		// 	if (SysInfo.Batt_Value.State < BAT_20_40_STATUS)
		// #endif
		{
			SysInfo.WorkState = Standy_mode;
			Led_Value.Mode = Batt_Low_0; // ??��1??LED??1Hz??????5??
			Led_Value.Counts = 0;
			//			SysInfo.Power_Value.state = System_Standy;
		}
		else
		{
			VCC3V3_OUT_ON();
			Read_Current_Level_For_Eeprom();
			SysInfo.Mode_Switch_Flag = 0x01; // RF??????
			IRled_start();

			SysInfo.Power_Value.Enter_Sleep_Flag = 0;
			SysInfo.Power_Value.state = System_ON;
			SysInfo.WorkState = SysInfo.Save_Data.WorkState;

			SysInfo.Skin_Touch_Montor_Flag = 1; // RF??????????��????????????????��
			SysInfo.Heating_Flag = 0x01;		// ?????????5?????��???��??????��??��
			SysInfo.Skin_No_Touch_Timer = 0;
			SysInfo.Freq_Cnt = 0;
			if (!SysInfo.WorkState)
			{
				//				RF_LED_DOWN();
				//				EMS_LED_UP();
				RF_LED_UP();
				EMS_LED_DOWN();
			}
			else
			{
				//				RF_LED_UP() ;
				//				EMS_LED_DOWN();
				RF_LED_DOWN();
				EMS_LED_UP();
			}
			SysInfo.Montor_Flag = 1; // ???????
			SysInfo.StayTime = 50;
			HAL_NVIC_DisableIRQ(EXTI0_1_IRQn); // ??????��?KEY_ON_OFF_Pin???
      SysInfo.Temp_Protect_Flage = 1; //���������¶ȼ�2���־λ
		}
		SysInfo.StayTime_30s_Flag = 1; // ?��???��??,????��???30s,??????????????????��???��?????1??????
	}
	else
	{
		//		Write_Parameter_To_Eeprom(); //??????�Ϧ�
		RF_Osc_Off();
		RF_Stop();
		Set_Mbi5020_Out(0);
		IRled_stop();
		RF_LED_DOWN();
		EMS_LED_DOWN();
		SysInfo.Power_Value.state = System_OFF;
		SysInfo.Save_Data.save_Data_flag = 1;
		SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;
		SysInfo.WorkState = Standy_mode;
		SysInfo.OverTemp_Flag = 0;
		SysInfo.StayTime_30s_Flag = 0;
		SysInfo.Skin_Touch_Flag = 0;
		SysInfo.Skin_Touch_RF_Flag = 0;
		SysInfo.Flag = 0x01;
		led_scan(0x00);
		VBAT_OUT_OFF();
		Key_Call_Data();
		SysInfo.Montor_Flag = 1; // ???????
		SysInfo.StayTime = 50;
		Led_Value.Level = 0x5a;
		SysInfo.Power_Value.Enter_Sleep_Flag = 1;
		HAL_NVIC_EnableIRQ(EXTI0_1_IRQn); // ???KEY_ON_OFF_Pin?��?
		SysInfo.Reminder_Cnt = 0;
	}
	SysInfo.Sleep_Counts = 0;
}
/**************************************************************************************
 * FunctionName   : Key_S2_Str
 * Description    : ??? ??? ?????5??
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Key_S2_Str(void) // ???
{
	if (SysInfo.WorkState == repair_mode)
	{
		SysInfo.repair_level = SysInfo.Save_Data.repair_level; // ???????????1??
		if (++SysInfo.repair_level > Level_5)
		{
			SysInfo.repair_level = Level_5;
		}
		SysInfo.Save_Data.repair_level = SysInfo.repair_level;
	}
	else if (SysInfo.WorkState == upkeep_mode)
	{
		SysInfo.upkeep_level = SysInfo.Save_Data.upkeep_level;

		if (++SysInfo.upkeep_level > Level_5)
		{
			SysInfo.upkeep_level = Level_5;
		}
		SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
		//		SysInfo.EMS_level = SysInfo.upkeep_level ;
	}
	else
		;
	SysInfo.Save_Data.save_Data_flag = 1;
	SysInfo.StayTime_30s_Flag = 1; // ?��???��??,????��???30s,??????????????????��???��?????1??????
}
/**************************************************************************************
 * FunctionName   : Key_S2_Long_Str
 * Description    :
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Key_S2_Long_Str(void)
{
}
/**************************************************************************************
 * FunctionName   : Key_S3_Str
 * Description    : ??? ???? ??��????1??
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Key_S3_Str(void) // ????
{
	if (SysInfo.WorkState == repair_mode)
	{
		SysInfo.repair_level = SysInfo.Save_Data.repair_level; // ???????????1??
		if (--SysInfo.repair_level < Level_1)
		{
			SysInfo.repair_level = Level_1;
		}
		SysInfo.Save_Data.repair_level = SysInfo.repair_level;
	}
	else if (SysInfo.WorkState == upkeep_mode)
	{
		SysInfo.upkeep_level = SysInfo.Save_Data.upkeep_level;
		if (--SysInfo.upkeep_level < Level_1)
		{
			SysInfo.upkeep_level = Level_1;
		}
		SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
		//            SysInfo.EMS_level = SysInfo.upkeep_level ;
	}
	else
		;
	SysInfo.Save_Data.save_Data_flag = 1;
	SysInfo.StayTime_30s_Flag = 1; // ?��???��??,????��???30s,??????????????????��???��?????1??????
}
/**************************************************************************************
 * FunctionName   : Key_S3_Long_Str
 * Description    :
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Key_S3_Long_Str(void)
{
}

/************************************??????????????*********************************/

/**************************************************************************************
 * FunctionName   : Test_Key_Press_Process
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_Key_Press_Process(_Key_Input key)
{
	switch (key)
	{
	default:
		break;
	}
}
/**************************************************************************************
 * FunctionName   : Test_Key_S1_Str
 * Description    : ?????????
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Test_Key_S1_Str(void) //
{
#if AutoTest // ????��?
	//	if(++SysInfo.Test_Mode.Test_Mode>3)
	//	{
	//		SysInfo.Test_Mode.Test_Mode = 0;
	//	}
	//	SysInfo.Test_Mode.Level =1;
	if (++SysInfo.Test_Mode.Test_Mode > 3)
	{
		SysInfo.Test_Mode.Test_Mode = 0;
	}
	SysInfo.Test_Mode.Level = 1;
	SysInfo.Test_Mode.Twink_Flag = 0;
	RF_Call_Data();
#else
	if (++SysInfo.Test_Mode.Test_Mode > 5)
	{
		SysInfo.Test_Mode.Test_Mode = 0;
	}
	SysInfo.Test_Mode.Level = 1;
	SysInfo.Test_Mode.Twink_Flag = 0;
	RF_Call_Data();
#endif
}

/**************************************************************************************
 * FunctionName   : Key_S1_Long_Str
 * Description    : ?????????????
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Test_Key_S1_Long_Str(void)
{
	SysInfo.Test_Mode.Test_Mode_Flag = OFF; // ?????????
	SysInfo.Test_Mode.Test_Mode = 0;
	SysInfo.Test_Mode.Level = 0;
	SysInfo.Test_Mode.Ageing_Mode = 0;
	SysInfo.Save_Data.save_Data_flag = 1; // ???�Ϧ�
	HAL_IWDG_Refresh(&hiwdg);
	System_Data_Save();
	//	Ems_Osc_Off();
	EMS_Pwm_Stop();
	Ems_Boost_Off();
	RF_Osc_Off();
	RF_Stop();
	RF_LED_DOWN();
	EMS_LED_DOWN();
	led_scan(0x00);
	BOOST_5V_OFF();
	VBAT_OUT_OFF();
	ALL_LED_DOWN();
	HAL_NVIC_SystemReset();
}
/**************************************************************************************
 * FunctionName   : Test_Key_S1_Str
 * Description    : ?????
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Test_Key_S2_Str(void) //
{
#if AutoTest // ????��?
	//	if(++SysInfo.Test_Mode.Ageing_Mode>2)
	//	{
	//		SysInfo.Test_Mode.Ageing_Mode=0;
	//	}
	if (++SysInfo.Test_Mode.Ageing_Mode > 1)
	{
		SysInfo.Test_Mode.Ageing_Mode = 0;
		SysInfo.Sleep_Flag = 0;
	}

	if (SysInfo.Test_Mode.Ageing_Mode == 0x01) // ????????
	{
		SysInfo.Save_Data.save_Data_flag = 1; // ???�Ϧ�
	}
	SysInfo.Test_Mode.Temp_level = 0;
#else
	if (!SysInfo.Test_Mode.Ageing_Flag)
	{
		if (++SysInfo.Test_Mode.Level > 6)
		{
			SysInfo.Test_Mode.Level = 1;
			SysInfo.Test_Mode.Twink_Flag = 0;
		}
	}
	else
	{
		if (++SysInfo.Test_Mode.Ageing_Mode > 1)
		{
			SysInfo.Test_Mode.Ageing_Mode = 0;
			SysInfo.Sleep_Flag = 0;
		}

		if (SysInfo.Test_Mode.Ageing_Mode == 0x01) // ????????
		{
			SysInfo.Save_Data.save_Data_flag = 1; // ???�Ϧ�
		}
	}
	SysInfo.Test_Mode.Temp_level = 0;
#endif
}
/**************************************************************************************
 * FunctionName   : Key_S1_Long_Str
 * Description    : ?????????????????
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Test_Key_S2_Long_Str(void)
{
	if (!SysInfo.Test_Mode.Ageing_Flag)
	{
		SysInfo.Test_Mode.Ageing_Flag = 1;

	#if ((ARF001 == DEVICE_R1_RPO)||(ARF001 == DEVICE_R1_HAIWAI))

		SysInfo.Mode_Switch_Flag = 0x01;
	#endif
	}
	else
	{
		SysInfo.Test_Mode.Ageing_Flag = 0;
		ALL_LED_DOWN();
		Ageing_TestData_DeInit();
		SysInfo.Save_Data.save_Data_flag = 1; // ???�Ϧ�

		//    HAL_NVIC_SystemReset();
	}
	SysInfo.Test_Mode.Ageing_Mode = 0;
}
/**************************************************************************************
 * FunctionName   : Test_Key_S1_Str
 * Description    : ??��
 * EntryParameter :
 * ReturnValue    :
 **************************************************************************************/
void Test_Key_S3_Str(void) //
{
#if AutoTest // ????��?
	if (++SysInfo.Test_Mode.Level > 5)
	{
		SysInfo.Test_Mode.Level = 5;
	}
#else
	if (SysInfo.Test_Mode.Temp_level == 0)
	{
		SysInfo.Test_Mode.Temp_level = 1;
	}
	else
	{
		SysInfo.Test_Mode.Temp_level = 0;
	}
#endif
}
/**************************************************************************************
 * FunctionName   : Test_Key_PutUp_Process
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_Key_PutUp_Process(_Key_Input key)
{
	uint8_t Flag;
	if (SysInfo.Test_Mode.Auto_Mode_Level != 0x04)
	{
		switch (key)
		{
		case KEY_MSCAP:
			Test_Key_S1_Str();
			break;
		case KEY_PLUS:
			Test_Key_S2_Str();
			break;
		case KEY_MINUS:
			Test_Key_S3_Str();
			break;
		default:
			break;
		}
	}
	else // ????????
	{
		Flag = 0;
		switch (key)
		{
		case KEY_MSCAP:
			if (SysInfo.Test_Mode.Data[5] == 1)
			{
				Flag = 0x01;
			}
			break;
		case KEY_PLUS:
			if (SysInfo.Test_Mode.Data[5] == 2)
			{
				Flag = 0x01;
			}
			break;
		case KEY_MINUS:
			if (SysInfo.Test_Mode.Data[5] == 3)
			{
				Flag = 0x01;
			}
			break;
		default:
			break;
		}
		if (Flag)
		{
			SysInfo.Test_Mode.Send_Flag = 1;
			SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
		}
	}
}
/**************************************************************************************
 * FunctionName   : Test_Key_Press_Long_Process
 * Description    :
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_Key_Press_Long_Process(_Key_Input key)
{
	switch (key)
	{
	case KEY_MSCAP:
		Test_Key_S1_Long_Str();
		break;
	case KEY_PLUS:
		Test_Key_S2_Long_Str();
		break;
	default:
		break;
	}
}
/************************************??????????????*********************************/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
