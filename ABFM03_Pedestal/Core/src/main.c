/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file main.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */

#include "User_System_Config.h"
#include "HW2000.h"
#include "ui_draw.h"
#include "uart.h"
#include "stdio.h"
#include "flash.h"
#include "stdio.h"
#define version  		"V0.1.0.0"
#define INSIDE_Version  "V0.1.0.0"   // 内部版本号

uint16_t reg_value;
u16 led0pwmval = 1;
u8 dir = 1;
extern uint32_t peer_code;
extern _TEST_MODE_HANDLE_TypeDef test_mode;
static void IWDG_Init(void);


/********************************************************
 * @brief  Main program.
 *  100ms µ÷ÓÃÒ»´Î£¬
 *  500ms Î¹¹·Ò»´Î
 ******************************************************/
static void deal_with_wdt(void)
{
	static u8 feed_wdt_cnt = 0;

	feed_wdt_cnt ++;
	if(feed_wdt_cnt > 5)
	{
		feed_wdt_cnt = 0;
		IWDG_ReloadKey();
	}

}
/************************/
//Í¼±êÉÁË¸´¦Àí
// ÐèÒªÉÁË¸Ê±£¬ÉÁË¸3´Î
// 100ms Ö´ÐÐÒ»´Î
/************************/
static void display_icons_check(void)
{
	static u8 lock_undisplay_cnt;
	static u8 level_undisplay_cnt;

	if(data_disp.lock_inverse_cnt)
	{
		if(data_disp.lock_inverse_cnt == MAX_INVESER_CNT)
		{
			data_disp.lock_undisp_flag = 1;
		}
		lock_undisplay_cnt ++;
		if(lock_undisplay_cnt >= 3)
		{
			lock_undisplay_cnt = 0;
			data_disp.lock_inverse_cnt --;

			if(data_disp.lock_undisp_flag)
			{
			data_disp.lock_undisp_flag = 0;
			}
			else
			{
				data_disp.lock_undisp_flag = 1;
			}
		}
	}
	else
	{
		data_disp.lock_undisp_flag = 0;
		lock_undisplay_cnt = 0;
	}

	if(data_disp.level_inverse_cnt)
	{

		if(data_disp.level_inverse_cnt == MAX_INVESER_CNT)
		{
			data_disp.level_undisp_flag = 1;
		}

		level_undisplay_cnt ++;
		if(level_undisplay_cnt >= 3)
		{
			level_undisplay_cnt = 0;

			if(data_disp.level_undisp_flag)
			{
				data_disp.level_undisp_flag = 0;
			}
			else
			{
				data_disp.level_undisp_flag = 1;
			}
			data_disp.level_inverse_cnt --;
		}
	}
	else
	{
		data_disp.level_undisp_flag = 0;
		level_undisplay_cnt = 0;
	}
}

/********************************************************
 * @brief  Main program.
 *  100ms µ÷ÓÃÒ»´Î£¬
 *
 ******************************************************/
static void system_idle_cnt(void)
{
	IDLE_120S_Cnt ++;

	if(IDLE_120S_Cnt >= 1100)            // 1200
	{
		IDLE_120S_Cnt = 1200;
		data_disp.idle_mode_flag = 1;
	}
	else
	{
		data_disp.idle_mode_flag = 0;
	}
}


/************************/
//------------ display----------
// 100 ms 调用一次
/************************/
static void HW_2_4G_Auto_match_disp(void)
{
	static uint8_t disp_cnt = 0;
	static uint8_t disp_flag = 0;

	disp_cnt ++;
	if(disp_cnt >= 5)
	{
		disp_cnt = 0;
		if(disp_flag)
		{
			disp_flag = 0;
		}
		else
		{
			disp_flag = 1;
		}
	}

	if(disp_flag)
	{
		connect_station_Display();
	}
	else
	{
		ui_draw_clear_screen();
	}
	update_to_oled();
}

/************************/
//------------ui display----------
// 100 ms 调用一次
/************************/
static void my_display_update(void)
{

	if(AUTO_MATCH_MODE == hw_2_4G_data.mode)
	{
		HW_2_4G_Auto_match_disp();
		return;
	}

	display_icons_check();
	if((data_disp.Lock_State != last_data_disp.Lock_State)||(SysInfo.Err_State != last_data_disp.Err_State)\
	||(data_disp.Level != last_data_disp.Level)||(data_disp.Connect != last_data_disp.Connect)\
	||(data_disp.lock_undisp_flag != last_data_disp.lock_undisp_flag)||(data_disp.level_undisp_flag != last_data_disp.level_undisp_flag)\
	||(data_disp.idle_mode_flag != last_data_disp.idle_mode_flag))
	{
		last_data_disp.Lock_State = data_disp.Lock_State;
		last_data_disp.Err_State = SysInfo.Err_State;

		last_data_disp.Level = data_disp.Level;
		last_data_disp.Connect = data_disp.Connect;
		last_data_disp.lock_undisp_flag = data_disp.lock_undisp_flag;
		last_data_disp.level_undisp_flag = data_disp.level_undisp_flag;
		last_data_disp.idle_mode_flag = data_disp.idle_mode_flag;

		ui_draw_clear_screen();

//		if(data_disp.idle_mode_flag)
//		{
//			ui_draw_clear_screen();
//			update_to_oled();
//			return;
//		}

		if(SysInfo.Err_State != Sys_Err_None)
		{
			display_err_show((eALARM_TYPE)SysInfo.Err_State);
		}
		else if(data_disp.level_undisp_flag)
		{
			display_ems_level(Level_NOT_USED);
		}
		else
		{
			display_ems_level((uint8_t)SysInfo.Level_TypeDef);
		}
		//
		connect_station_Display();
		update_to_oled();

	}
}

/****************************
 *
****************************/
static void Frame_loss_rate_check(void)
{
	if(SysInfo.send_frame_cnt >= 100)
	{
		SysInfo.Loss_Rate = SysInfo.recv_frame_cnt;
		SysInfo.send_frame_cnt = 0;
		SysInfo.recv_frame_cnt = 0;
	}
}

/********************************************************
 * @brief  Main program.
 ******************************************************/
int main(void)
{
	u8 task_1ms_cnt;
	u8 hw2000_reset_50ms_cnt = 0;
	u8 disconnect_500ms_cnt = 0;
	u8 print_cnt;

	MX_GPIO_Init();
	MX_SPI2_Init();
	MX_TIME_Init();
	// MX_ADC_Init();
	OLED_SH1106_Init();
	MX_USART1_UART_Init();
	MX_TIMER6_Init();
	HW_Spi_Init();
	IWDG_Init();
	static uint32_t last_peer_code=0;
	peer_code=ReadPeerCode();

	printf ("\n\r ==========\n\r");
	printf ("\n\r Base version:%s, Inside version:%s\n\r",  version, INSIDE_Version);
//	printf ("\n\r ----20230117------\n\r");
	printf ("\n\r PEER_CODE: 0x%0x\n\r", peer_code);

	HW2000_Init_250k();

	SysInfo.Lock_State  = Un_Lock;
	data_disp.Lock_State = Un_Lock;
	SysInfo.Connect = 0;

	while(1)
	{
		if(SysInfo.timer_1ms_flag)
		{
			SysInfo.timer_1ms_flag = 0;
			task_1ms_cnt ++;
			if(0 == task_1ms_cnt % 5)
			{
				sys_key_check();
				HW_RF_RX_EN();                         //ï¿½ï¿½ï¿½ï¿½Ê¹ï¿½ï¿½
				HW_RX_Data_Process();                  //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½
				Key_2_4G_auto_match_mode_Check();
			}

			 if(0 == task_1ms_cnt % 20)
			 {
				Uart_Protocol_Process(); 			// 2023 03 22 �����ڴ�������Ų�� 100ms����������
			 }

			if(0 == task_1ms_cnt % 50)
			{
				Frame_loss_rate_check(); // 丢包率测试
				HW_TX_Data_Process();
				hw2000_reset_50ms_cnt ++;
				if(SysInfo.recv_msg_flag)
				{
					SysInfo.recv_msg_flag = 0;
					hw2000_reset_50ms_cnt = 0;
					disconnect_500ms_cnt = 0;
				}

				if(hw2000_reset_50ms_cnt >= 10)
				{
					hw2000_reset_50ms_cnt = 0; // 500ms reset 2.4G
					disconnect_500ms_cnt ++;
					HW2000_Init_250k();       //  ½ÓÊÜÊý¾Ý³¬Ê± ÖØÆô£¬Õý³£½ÓÊÕµ½Êý¾Ýºó²»»áÔÙÖØÆô
				}

				if(disconnect_500ms_cnt >= 10)
				{
					SysInfo.Connect = 0;
					data_disp.Connect = 0;
					disconnect_500ms_cnt = 0;
				}
			}

			if(0 == task_1ms_cnt % 100)
			{
				task_1ms_cnt = 0;

				Lock_Check();
				system_idle_cnt();
				my_display_update();// ui_update
				deal_with_wdt();

				print_cnt ++;
				if(print_cnt >= 10)
				{
					print_cnt = 0;
					// printf ("\n\r Lock_State = %d, connect_sta =%d, lock_cnt = %d\n\r", SysInfo.Lock_State, SysInfo.Connect, Auto_30S_Lock_Cnt);
					// printf ("\n\r outside_Ver:%s, Inside_Version:%s\n\r", version, INSIDE_Version);
					//printf ("\n\r loss_rate:%d \n\r", SysInfo.Loss_Rate);
					//printf ("\n\r 2.4G_mode =0x%0x \n\r", hw_2_4G_data.mode);
				}
			}
		}
	}
}

static void IWDG_Init(void)
{
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteConfig(IWDG_WRITE_ENABLE);
	/* IWDG counter clock: LSI/32 */
	IWDG_SetPrescalerDiv(IWDG_PRESCALER_DIV32);
	/* 1000ms */
	IWDG_CntReload(1066);
	/* Reload IWDG counter */
	IWDG_ReloadKey();
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

/**
 * @brief Assert failed function by user.
 * @param file The name of the call that failed.
 * @param line The source line number of the call that failed.
 */
#ifdef USE_FULL_ASSERT
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    while(1)
    {
    }
}
#endif // USE_FULL_ASSERT

/**
 * @}
 */


