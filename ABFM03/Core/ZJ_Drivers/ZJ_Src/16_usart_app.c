/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration 123
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "FreeRTOS.h"

#include "stdbool.h"
#include <string.h>
#include "Test_Mode.h"
#include "stdio.h"
#include "stdarg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

uint32_t peer_code=0;

extern UART_HandleTypeDef huart2;
extern uint8_t Rx_buff[];
extern uint8_t Tx_buff[];
extern uint8_t recv_cnt;
extern uint8_t recv_finish_flag;

extern uint16_t crc16_compute(uint8_t const * p_data, uint32_t size);
extern void hw2000b_init_250k(void);
extern void enter_tila_rf_test(void);
extern void enter_tila_ems_test(void);

//****************************************
//****************************************
static void set_peer_code(void)
{
	uint8_t len = 10;

	peer_code=((Rx_buff[4]<<24)|(Rx_buff[5]<<16)|(Rx_buff[6]<<8)|Rx_buff[7]);
	SavePeerCode(peer_code);
	hw2000b_init_250k();
	memcpy(Tx_buff, Rx_buff, len -2);
	Tx_buff[0]=0x10;
	Send_Data_with_crc(Tx_buff ,len);
}

//****************************************
//****************************************
static void get_peer_code(void)
{
	uint8_t len = 10;

	peer_code=ReadPeerCode();
	Tx_buff[0] = 0x10;
	Tx_buff[1] = len;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Get_Peer_Code_Cmd;
	Tx_buff[4] = ((peer_code>>24)&0xFF);
	Tx_buff[5] = ((peer_code>>16)&0xFF);
	Tx_buff[6] = ((peer_code>>8)&0xFF);
	Tx_buff[7] = (peer_code&0xFF);
	peer_code = ReadPeerCode();
	Send_Data_with_crc(Tx_buff ,len);
}

//****************************************
//****************************************
static void get_soft_version(void)
{
	if(Rx_buff[4] != 0x00)
	{
		return;
	}

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x0A;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = comm_soft_version;

	if(Rx_buff[5] == 0x01)
	{
		Tx_buff[4] = EXTERNAL_VERSION_0;	// �ⲿ�汾��
		Tx_buff[5] = EXTERNAL_VERSION_1;
		Tx_buff[6] = EXTERNAL_VERSION_2;
		Tx_buff[7] = EXTERNAL_VERSION_3;
	}
	else //if( Rx_buff[5] == 0x02)
	{
		Tx_buff[4] = INTERNAL_VERSION_0;    // �ڲ��汾��
		Tx_buff[5] = INTERNAL_VERSION_1;
		Tx_buff[6] = INTERNAL_VERSION_2;
		Tx_buff[7] = INTERNAL_VERSION_3;
	}
	Send_Data_with_crc(Tx_buff, 0x0A);
}

//****************************************
//****************************************
void send_ntc_temp(void)
{
	int temp;

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Comm_Ntc_Temp;

	temp = (int)(SysInfo.NTC_Temp * 10);
	Tx_buff[4] = (temp/10) & 0xff;
	Tx_buff[5] = (temp%10);
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
// 校准： calibration
//****************************************
void cali_ntc_result(TRUE_FALSE_e res)
{
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = ADJUST_NTC_TEMP_Cmd;

	Tx_buff[4] = 0;
	switch(res)
	{
		case True:
		{
			Tx_buff[5] = 1; //  校验成功
			break;
		}
		case False:
		default:
		{
			Tx_buff[5] = 0; //  校验失败
			break;
		}
	}
	Send_Data_with_crc(Tx_buff, 0x08);
}


//****************************************
//****************************************
static void get_NTC_temp(void)
{
	int temp;

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Comm_Ntc_Temp;
	if((Rx_buff[4] == 0xFF)&&(Rx_buff[5] == 0xFF))
	{
		Tx_buff[4] = 0xff;
		Tx_buff[5] = 0xff;
	}
	else
	{
		temp = (int)(SysInfo.NTC_Temp * 10);
		Tx_buff[4] = (temp/10) & 0xff;
		Tx_buff[5] = (temp%10);
	}
	SysInfo.Test_Mode.Printf_NTC_Flag = 0;
	Send_Data_with_crc(Tx_buff, 0x08);
	SysInfo.Test_Mode.Test_Mode_Flag = OFF;
	SysInfo.Test_Mode.STATUS = NONE_TEST;
}

//****************************************
// 边输出，边读取ntc
//****************************************
static void enter_fast_read_NTC(void)
{
	enter_xiufu_rf_test();
	SysInfo.Test_Mode.STATUS = NTC_WITH_OUTPUT_TEST;
}


//****************************************
//****************************************
static void get_Battery_val(void)
{
	int temp;

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Comm_Batter_Value;

    temp = BAT_Data.adc_Value * 122 / 100;
	Tx_buff[4] = temp / 1000;
	Tx_buff[5] = (temp % 1000) / 10;
	Send_Data_with_crc(Tx_buff, 0x08);
	SysInfo.Test_Mode.Test_Mode_Flag = OFF;
	SysInfo.Test_Mode.STATUS = NONE_TEST;
}


//****************************************
//****************************************
static void dealwith_light_test(void)
{
	if(Rx_buff[4] == 0xFF && Rx_buff[5] == 0xFF) //�˳�led ����
	{
		SysInfo.Test_Mode.Test_Mode_Flag = OFF;
		SysInfo.Test_Mode.STATUS = NONE_TEST;
	}
	else
	{
		SysInfo.Test_Mode.Test_Mode_Flag = ON;
		SysInfo.Test_Mode.STATUS = LED_TEST;
	}

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Send_Data_with_crc(Tx_buff, 0x08);
	SysInfo.Test_Mode.Printf_NTC_Flag = 0;
}

//****************************************
//****************************************
static void ems_test_level_select(uint8_t level)
{
	switch(level)
	{
		case Level_1:
		{
			SysInfo.upkeep_level = Level_1;
			EMS_Handle.vol_value = Level_20;
			break;
		}
		case Level_2:
		{
			SysInfo.upkeep_level = Level_2;
			EMS_Handle.vol_value = Level_25;
			break;
		}
		case Level_3:
		{
			SysInfo.upkeep_level = Level_3;
			EMS_Handle.vol_value = Level_30;
			break;
		}
		default:
		{
			SysInfo.upkeep_level = Level_1;
			EMS_Handle.vol_value = Level_20;
			break;
		}
	}
	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
}


//****************************************
//****************************************
static void RF_test_level_select(uint8_t level)
{
	switch(level)
	{
		case Level_1:
		{
			SysInfo.repair_level = Level_1;

			break;
		}
		case Level_2:
		{
			SysInfo.repair_level = Level_2;

			break;
		}
		case Level_3:
		{
			SysInfo.repair_level = Level_3;

			break;
		}
		default:
		{
			SysInfo.repair_level = Level_1;

			break;
		}
	}
	SysInfo.Save_Data.repair_level = SysInfo.repair_level;
}

//****************************************
//****************************************
static void RF_test_freq_select(uint8_t freq)
{
	switch(freq)   // 频率选择
	{
		case 1:
		{
			SysInfo.Test_Mode.Set_RF_Freq = RF_1MHZ_FREQ;
			break;
		}
		case 2:
		{
			SysInfo.Test_Mode.Set_RF_Freq = RF_1_5MHZ_FREQ;
			break;
		}
		case 3:
		{
			SysInfo.Test_Mode.Set_RF_Freq = RF_2MHZ_FREQ;
			break;
		}
		case 4:
		{
			SysInfo.Test_Mode.Set_RF_Freq = RF_ALL_FREQ;
			break;
		}
	}
}

//****************************************
//****************************************
static void dealwith_RF_test(void)
{
	if(Rx_buff[4]==0xFF && Rx_buff[3]==0xFF) //�ر�RF
	{
		SysInfo.Test_Mode.RF_Flag = 0x00;
		RF_DeInit();
		SysInfo.Test_Mode.Test_Mode_Flag = OFF;
		SysInfo.Test_Mode.STATUS = NONE_TEST;

		Tx_buff[0] = 0x10;
		Tx_buff[1] = 0x08;
		Tx_buff[2] = 0x00;
		Tx_buff[3] = Rx_buff[3];
		Tx_buff[4] = Rx_buff[4];
		Tx_buff[5] = Rx_buff[5];
		Send_Data_with_crc(Tx_buff, 0x08);
		SysInfo.Test_Mode.Printf_NTC_Flag = 0;
		return;
	}

	//-------------------------------------
	if((Rx_buff[5] > 3) || (Rx_buff[5] == 0))  // 档位纠错
	{
		return;
	}

	if((Rx_buff[7] > 4) || (Rx_buff[7] == 0))  // 频率纠错
	{
		return;
	}

	ems_test_level_select(Rx_buff[5]); //档位选择 ---RF测试，也是在紧致提拉模式下
	RF_test_freq_select(Rx_buff[7]); // 频率选择

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x0A;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Tx_buff[6] = Rx_buff[6];
	Tx_buff[7] = Rx_buff[7];
	Send_Data_with_crc(Tx_buff, 0x0A);

	SysInfo.Test_Mode.Test_Mode_Flag = ON;
	SysInfo.Test_Mode.STATUS = TILA_MODE_RF_TEST;
	enter_tila_rf_test();
	SysInfo.Test_Mode.Test_Mode_Flag = 1;
	SysInfo.Test_Mode.Printf_NTC_Flag = 0;
}


//****************************************
//****************************************
static void ems_test_freq_select(uint8_t freq)
{
	switch(freq)   // 频率选择
	{
		case 1:
		{
			SysInfo.Test_Mode.Set_EMS_Freq = EMS_1KHZ_FREQ;
			break;
		}
		case 2:
		{
			SysInfo.Test_Mode.Set_EMS_Freq = EMS_2KHZ_FREQ;
			break;
		}
		case 3:
		{
			SysInfo.Test_Mode.Set_EMS_Freq = EMS_3KHZ_FREQ;
			break;
		}
		case 4:
		{
			SysInfo.Test_Mode.Set_EMS_Freq = EMS_ALL_FREQ;
			break;
		}
	}
}

//****************************************
//****************************************
static void dealwith_EMS_test(void)
{
	if(Rx_buff[4] == 0xFF && Rx_buff[5] == 0xFF) //�ر�RF
	{
		SysInfo.Test_Mode.EMS_Flag = 0x00;
		Ems_DeInit();
		SysInfo.Test_Mode.Test_Mode_Flag = OFF;
		SysInfo.Test_Mode.STATUS = NONE_TEST;
		SysInfo.Test_Mode.Set_EMS_Freq = EMS_ALL_FREQ;

		Tx_buff[0] = 0x10;
		Tx_buff[1] = 0x08;
		Tx_buff[2] = 0x00;
		Tx_buff[3] = Rx_buff[3];
		Tx_buff[4] = Rx_buff[4];
		Tx_buff[5] = Rx_buff[5];
		Send_Data_with_crc(Tx_buff, 0x08);
		SysInfo.Test_Mode.Printf_NTC_Flag = 0;
		return;
	}
	//-------------------------------------
	if((Rx_buff[5] > 3) || (Rx_buff[5] == 0))  // 档位纠错
	{
		return;
	}

	if((Rx_buff[7] > 4) || (Rx_buff[7] == 0))  // 频率纠错
	{
		return;
	}

	ems_test_level_select(Rx_buff[5]); //档位选择
	ems_test_freq_select(Rx_buff[7]); // 频率选择

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x0A;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Tx_buff[6] = Rx_buff[6];
	Tx_buff[7] = Rx_buff[7];
	Send_Data_with_crc(Tx_buff, 0x0A);

	SysInfo.Test_Mode.Test_Mode_Flag = ON;
	SysInfo.Test_Mode.STATUS = TILA_MODE_EMS_TEST;
	SysInfo.Test_Mode.Printf_NTC_Flag = 0;
	SysInfo.Test_Mode.Test_Mode_Flag = 1;
	enter_tila_ems_test();
}
//****************************************
// 电源按键，短按，发送码值
//****************************************
void dealwith_KEY_test_power_key_up(void)
{
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = 0x0A;
	Tx_buff[4] = 0x00;
	Tx_buff[5] = 0x01;
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
// 电源按键，档位 + 短按，发送码值
//****************************************
void dealwith_KEY_test_level_add_key_up(void)
{
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = 0x0A;
	Tx_buff[4] = 0x00;
	Tx_buff[5] = 0x02;
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
// 电源按键，档位 - 短按，发送码值
//****************************************
void dealwith_KEY_test_level_dec_key_up(void)
{
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = 0x0A;
	Tx_buff[4] = 0x00;
	Tx_buff[5] = 0x03;
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
//****************************************
static void dealwith_KEY_test(void)
{
	if(Rx_buff[4] == 0xFF && Rx_buff[5] == 0xFF) //�ر�RF
	{
		// SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
		SysInfo.Test_Mode.Test_Mode_Flag = OFF;
		SysInfo.Test_Mode.STATUS = NONE_TEST;
	}
	else
	{
		SysInfo.Test_Mode.Test_Mode_Flag = ON;
		// SysInfo.Test_Mode.Auto_Mode_Level = 0x04;
		// SysInfo.Test_Mode.Ageing_Mode = 0;
		SysInfo.Test_Mode.Is_Enter_Ageing_Mode = 0;
		SysInfo.Test_Mode.STATUS = KEY_TEST;
	}
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
//****************************************
void G_SENSOR_run_send_data(void)
{
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = 0x0c;
	Tx_buff[4] = 0x00;
	Tx_buff[5] = 0x01;
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
//****************************************
void G_SENSOR_idle_send_data(void)
{
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = 0x0c;
	Tx_buff[4] = 0x00;
	Tx_buff[5] = 0x00;
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
//****************************************
static void dealwith_G_SENSOR_test(void)
{
	if(Rx_buff[4] == 0xFF && Rx_buff[5] == 0xFF) //�ر�RF
	{
		SysInfo.Test_Mode.Test_Mode_Flag = OFF;
		SysInfo.Test_Mode.STATUS = NONE_TEST;
	}
	else
	{
		SysInfo.Test_Mode.RF_Flag = 0;
		SysInfo.Test_Mode.EMS_Flag = 0;
		SysInfo.Test_Mode.Test_Mode_Flag = ON;
		SysInfo.Power_Value.state = System_ON;
		SysInfo.Test_Mode.STATUS = Gsensor_TEST;
	}
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Send_Data_with_crc(Tx_buff, 0x08);
}

/***********************************************
*
* 进入老化测试初始化
***********************************************/
extern _EMS_HANDLE_TypeDef EMS_Handle;

void enter_agine_test_init(void)
{
	SysInfo.Test_Mode.EMS_Flag=0;
	SysInfo.Test_Mode.RF_Flag=0;
	SysInfo.Test_Mode.Printf_NTC_Flag = 0;

	SysInfo.Test_Mode.Test_Mode_Flag = ON;
	SysInfo.Test_Mode.Ageing_Flag = 1;

	// SysInfo.Test_Mode.Is_Enter_Ageing_Mode=0;
	SysInfo.Mode_Switch_Flag = OUT_PUT_RF;
	RF_Handle.Run_Flag = 1;
	// SysInfo.Skin_Touch_Flag =1;
	SysInfo.Test_Mode.STATUS = AGING_TEST;
	SysInfo.WorkState = upkeep_mode;

  	SysInfo.upkeep_level =Level_3;
	EMS_Handle.vol_value = Level_30;
	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
  	Ems_Value_Add(&EMS_Handle);

	SysInfo.Power_Value.state = System_ON;
	//SysInfo.Save_Data.save_Data_flag = 1;

	SysInfo.Test_Mode.Aging_run_once_over_flag = 0;
	//SysInfo.Test_Mode.EMS_RF_out_flag = 0;
	//SysInfo.Test_Mode.Charge_flag = 1;

	SysInfo.Test_Mode.EMS_RF_out_flag = 1;
	SysInfo.Test_Mode.Charge_flag = 0;
	Ems_DeInit();
	RF_DeInit();

}


/***********************************************
 * 老化测试逻辑：
 *  1. 先将电池的点充满 >=BAT_80_100_STATUS
 *  2. 按紧致提拉模式3档输出，输出5min，暂停2min。防止内部期间过热
 *  3. 运行到低电，开始充电，至70%电量  BAT_20_30_STATUS， BAT_30_40_STATUS
 *
***********************************************/
static void dealwith_ALIGN_test(void)
{
	if(Rx_buff[4] == 0xFF && Rx_buff[5] == 0xFF) //�ر�RF
	{
		Ageing_TestData_DeInit();
		SysInfo.Test_Mode.Ageing_Flag = 0;

		SysInfo.Test_Mode.Test_Mode_Flag = OFF;
		SysInfo.Test_Mode.STATUS = NONE_TEST;
		Tx_buff[5] = Rx_buff[5];
	}
	else if ((Rx_buff[4] == 0x00 && Rx_buff[5] == 0x01))
	{
		if(SysInfo.Test_Mode.Ageing_finish_Flag)
		{
			Tx_buff[5] = 1;
		}
		else
		{
			Tx_buff[5] = 0;
		}

	}
	else
	{
		enter_agine_test_init();
		Tx_buff[5] = Rx_buff[5];
	}
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];

	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
//寿命测试，初始化
//****************************************
void enter_inset_life_test_init(void)
{
	SysInfo.Test_Mode.EMS_Flag=0;
	SysInfo.Test_Mode.RF_Flag=0;
	SysInfo.Test_Mode.Printf_NTC_Flag = 0;

	SysInfo.Test_Mode.Test_Mode_Flag = ON;
	SysInfo.Test_Mode.Ageing_Flag = 1;

	// SysInfo.Test_Mode.Is_Enter_Ageing_Mode=0;
	SysInfo.Mode_Switch_Flag = OUT_PUT_RF;
	RF_Handle.Run_Flag = 1;
	// SysInfo.Skin_Touch_Flag =1;
	SysInfo.Test_Mode.STATUS = INSET_LIFE_TEST;
	SysInfo.WorkState = upkeep_mode;

  	SysInfo.upkeep_level =Level_3;
	EMS_Handle.vol_value = Level_30;
	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
  	Ems_Value_Add(&EMS_Handle);

	SysInfo.Power_Value.state = System_ON;
	//SysInfo.Save_Data.save_Data_flag = 1;

	SysInfo.Test_Mode.Aging_run_once_over_flag = 0;
	//SysInfo.Test_Mode.EMS_RF_out_flag = 0;
	//SysInfo.Test_Mode.Charge_flag = 1;

	SysInfo.Test_Mode.EMS_RF_out_flag = 1;
	SysInfo.Test_Mode.Charge_flag = 0;
	Ems_DeInit();
	RF_DeInit();

}


//****************************************
//****************************************
static void dealwith_Inset_life_test(void)
{
	if(Rx_buff[4] == 0xFF && Rx_buff[5] == 0xFF) //�ر�RF
	{
		Ageing_TestData_DeInit();
		SysInfo.Test_Mode.Ageing_Flag = 0;
		// SysInfo.Test_Mode.Ageing_Mode = 0;
		SysInfo.Mode_Switch_Flag = 0;
		RF_Handle.Run_Flag = 0;
		SysInfo.Skin_Touch_Flag = 0;
		//SysInfo.Save_Data.save_Data_flag = 1; //���浵λ
		//ALL_LED_DOWN();
//						HAL_NVIC_SystemReset();
	}
	else
	{
		SysInfo.Test_Mode.Printf_NTC_Flag = 0;
		SysInfo.Test_Mode.Test_Mode_Flag = ON;
		SysInfo.Test_Mode.Ageing_Flag = 1;
		// SysInfo.Test_Mode.Ageing_Mode = 1;
		SysInfo.Mode_Switch_Flag = 1;
		RF_Handle.Run_Flag = 1;
		SysInfo.Skin_Touch_Flag = 1;
		//SysInfo.Save_Data.save_Data_flag = 1; //���浵λ
		//RF_LED_DOWN();
		//EMS_LED_UP();
		enter_inset_life_test_init();
	}

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
//****************************************
static void dealwith_INSET_test(void)
{
	if(Rx_buff[4] == 0x00 && Rx_buff[5] == 0x00) //������������ģʽָ��
	{
		SysInfo.Test_Mode.Test_Mode_Flag = Auto_ON;
	}
	else   //�˳���������ģʽָ��
	{
		SysInfo.Test_Mode.RF_Flag = 0x00;
		SysInfo.Test_Mode.EMS_Flag = 0x00;
		SysInfo.Test_Mode.Test_Mode_Flag = OFF;
	}
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
// 参数 01， 恢复未紧致提拉模式 1档，淡纹修护 模式档位调为1档
// 参数 02， 在上面的基础上将温度偏差也清0。
//****************************************
static void reset_param_test(void)
{
	SysInfo.WorkState = upkeep_mode;
	SysInfo.repair_level =Level_1;
	SysInfo.upkeep_level =Level_1;
	SysInfo.Save_Data.repair_level = SysInfo.repair_level;
	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;

	if(2 == Rx_buff[5])
	{
		SysInfo.Test_Mode.Ageing_finish_Flag = 0;
		SysInfo.NTC_offset = 0;
	}
	Write_Parameter_To_Eeprom(); //保存信息档位
	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Send_Data_with_crc(Tx_buff, 0x08);
}

//****************************************
//****************************************
static void dealwith_adjust_ntc_temp(void)
{

	SysInfo.Test_Mode.NTC_TARGET = Rx_buff[4];

	Tx_buff[0] = 0x10;
	Tx_buff[1] = 0x08;
	Tx_buff[2] = 0x00;
	Tx_buff[3] = Rx_buff[3];
	Tx_buff[4] = Rx_buff[4];
	Tx_buff[5] = Rx_buff[5];
	Send_Data_with_crc(Tx_buff, 0x08);

	SysInfo.Test_Mode.STATUS = ADJUST_NTC_TEST;
	SysInfo.Test_Mode.ADJUST_NTC_Delay = 0;
	SysInfo.Test_Mode.Test_Mode_Flag = 1;
}

//***********************************************************
//download test peer code:
//	0x00 0A 00 15 AA 55 55 AA 37 74
//	0x00 0A 00 15 A2 00 00 14 B0 D6
//	0x00 08 00 16 00 00 FD FE
//***********************************************************
void Uart_Protocol_Process(void)
{
	uint16_t Crc=0;
	uint8_t Length=0;
	uint16_t cmd=0;
	int temp;

	if(recv_finish_flag)
	{
		recv_finish_flag = 0;
		if(Rx_buff[1] < 2 || Rx_buff[1] != recv_cnt)
		{
			recv_cnt = 0;// ---clear recv cnt
			return;
		}

		Length = Rx_buff[1];
		Crc = crc16_compute(Rx_buff, Length-2);
		if((Rx_buff[Length-2] != ((Crc >> 8) & 0xff)) || (Rx_buff[Length-1] != (Crc & 0xff)))
		{
			recv_cnt = 0;// ---clear recv cnt
			return;
		}

		cmd = (Rx_buff[2] << 8) | Rx_buff[3];
		switch(cmd)
		{
			case Set_Peer_Code_Cmd:
			{
				set_peer_code();
				break;
			}
			case Get_Peer_Code_Cmd:
			{
				get_peer_code();
				break;
			}
			case comm_soft_version:
			{
				get_soft_version();
				break;
			}
			case Comm_Ntc_Temp:// 0x03
			{
				get_NTC_temp();
				//enter_fast_read_NTC();
				break;
			}
			case Comm_Batter_Value://0x04
			{
		        get_Battery_val();
				break;
			}
			case Comm_Light_Control:
			{
				dealwith_light_test();
				break;
			}

			case Comm_RF_Control:
			{
				dealwith_RF_test();
				break;
			}

			case Comm_EMS_Control:
			{
				dealwith_EMS_test();
				break;
			}
			case Comm_KEY_Control:  //00 08 00 0A 00 01 DB DD
			{
				dealwith_KEY_test();
				break;
			}

			case Comm_GSensor_Control: //Gsensor TEST
			{
				dealwith_G_SENSOR_test();
				break;
			}

			case Comm_Aging_Model://
			{
				dealwith_ALIGN_test();
				break;
			}
			case Comm_Inset_Life_Testl:
			{
				dealwith_Inset_life_test();
				break;
			}

			case Comm_Inset_Testmode:
			{
				dealwith_INSET_test();
				break;
			}

			case ADJUST_NTC_TEMP_Cmd:
			{
				dealwith_adjust_ntc_temp();
				break;
			}

			case comm_reset_param:
			{
				reset_param_test();
				break;
			}
			default:
			{
				break; // ---debug
			}
		}
		recv_cnt = 0;
	}

}








/* USER CODE END 1 */
