/******************************************************************************
 * @file     spi0.c
 * @brief    hr8p296 spi function
 *
 *
 * @version  1.0
 * @date     2018-12-06
 *
 * @author   fudl
 *
 * @note
 * @Copyright (C) 2018 Shanghai Eastsoft Microelectronics C0., Ltd.
 ******************************************************************************/
#include "main.h"
#include "HW2000.h"
#include "ui_draw.h"

extern uint32_t peer_code;
uint8_t RX_BUF[HW_BUFFER_LEN];
uint8_t TX_BUF[HW_BUFFER_LEN];

HW_2_4G_t hw_2_4G_data;
uint32_t tmp_id_data = 0;

/******************************************************************************
 * @brief    hr8p296_spi0_init
 *
 * @note	 spi0 init function
 *
 * @param	 None
 * @retval	 None
 *
 * @version  1.0
 * @date
 * @author
 ******************************************************************************/


void HW_Spi_Init(void)
{
	HW_CSN_LOW;
	HW_CE_HIGH;
	Delay_MS(2);
	HW_CE_HIGH;
	Delay_MS(5);
}

uint8_t HW_SendByte(uint8_t byte)
{
	/*!< Loop while DAT register in not emplty */
	while (SPI_I2S_GetStatus(SPI1, SPI_I2S_TE_FLAG) == RESET)
		;

	/*!< Send byte through the SPI1 peripheral */
	SPI_I2S_TransmitData(SPI1, byte);

	/*!< Wait to receive a byte */
	while (SPI_I2S_GetStatus(SPI1, SPI_I2S_RNE_FLAG) == RESET)
		;

	/*!< Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}
/******************************************************************************
 * @brief    hw3000_write_reg
 *
 * @note	 write register function
 *
 * @param	 addr reg address
			 data
 * @retval	 None
 *
 * @version  1.0
 * @date
 * @author
 ******************************************************************************/
void HW2000_Write_Reg(uint8_t addr, uint16_t data)
{
	uint8_t temp_data;

	HW_CSN_LOW;
	addr |= 0x80;

	HW_SendByte(addr);

	temp_data =  (data >> 8);
	HW_SendByte(temp_data);

	temp_data =  data;
	HW_SendByte(temp_data);

	HW_CSN_HIGH;

}

///******************************************************************************
// * @brief    hw3000_read_reg
// *
// * @note	 read register function
// *
// * @param	 addr reg addressqqqqqqqqqqqqqqqqqqq
// * @retval	 data
// *
// * @version  1.0
// * @date
// * @author
// ******************************************************************************/

uint16_t HW2000_Read_Reg(uint8_t addr)
{
	uint8_t temp_data;
	uint16_t value;

	HW_CSN_LOW;

	HW_SendByte(addr);

	temp_data = HW_SendByte(0XFF);
	value = temp_data;

	temp_data = HW_SendByte(0XFF);

	value = (value << 8);
	value = (value | temp_data);

	HW_CSN_HIGH;

	return value;
}

void HW2000_Write_Fifo(uint8_t addr, uint8_t *data, uint16_t length)
{
	uint16_t i;

	HW_CSN_LOW;
	addr |= 0x80;
	HW_SendByte(addr);

	for(i=0; i<length; i++)
	{
		HW_SendByte(data[i]);
	}

	HW_CSN_HIGH;
}

/******************************************************************************
 * @brief    hw3000_read_fifo
 *
 * @note	 read fifo function
 *
 * @param	 addr reg address
 *		     data
 *           length
 * @retval	 None
 *
 * @version  1.0
 * @date
 * @author
 ******************************************************************************/

void HW2000_Read_Fifo(uint8_t addr, uint8_t *data, uint16_t length)
{
	uint16_t i;

	HW_CSN_LOW;

	HW_SendByte(addr);

	for(i=0; i<length; i++)
	{
		data[i] = HW_SendByte(0XFF);
	}

	HW_CSN_HIGH;
}
void set_power_8(void) //8db
{
	HW2000_Write_Reg(0x0B, 0x0873);//8
}
void HW2000_Init_250k(void)
{
	uint8_t i;

	uint16_t agcTab[18] =
	{
		0x0000, 0x0000, 0x0000, 0x0001,
		0x0002, 0x000A, 0x0012, 0x0212,
		0x0412, 0x064A, 0x084A, 0x0A4A,
		0x0A52, 0x0A92, 0x0C92, 0x0CD2,
		0x0CDA, 0x0CE3
	};

	HW_Spi_Init();

	HW2000_Write_Reg(0x4C, 0x55AA);		//close write protect
	for (i = 0; i < 18; i++) {
		HW2000_Write_Reg(0x50 + i, agcTab[i]);
	}
	HW2000_Write_Reg(0x0F, 0xFC04);

	HW2000_Write_Reg(0x01, 0x4D58);
	HW2000_Write_Reg(0x02, 0x04CC);
	HW2000_Write_Reg(0x08, 0xB6C4);
	HW2000_Write_Reg(0x09, 0xB442);

	HW2000_Write_Reg(0x4A, 0x326C);
	HW2000_Write_Reg(0x0B, 0x0873);        //tx power 8
	HW2000_Write_Reg(0x49, 0x1330);

	HW2000_Write_Reg(0x27, 0x8F20);
	HW2000_Write_Reg(0x48, 0x4300);
	// HW2000_Write_Reg(0x26, 0x000C);
	HW2000_Write_Reg(0x26, 0xFC0C);//大电流驱动打开 （spi通讯速度高于 1MHZ）

	HW2000_Write_Reg(0x28, 0x8402);	 	  //sync_thres:2bit
	HW2000_Write_Reg(0x2C, 0x918B);
	HW2000_Write_Reg(0x2A, 0x40FF);	    //rate:250kbps, ACK time
	HW2000_Write_Reg(0x1A, 0x0D31);
	HW2000_Write_Reg(0x19, 0x0884);
	HW2000_Write_Reg(0x20, 0xF000);		  //preamble:16 bytes  sync:48 bits
	HW2000_Write_Reg(0x36, 0x0081);	//FIFO0 ä½¿èƒ½,pipe 0
	HW2000_Write_Reg(0x37, 0x0000); //FIFO1 disable

//	HW2000_Write_Reg(0x22, 0x1800+(peer_code%16));//ABFM03_ADDR_NUM);//((0xA2<<24)|(0x00<<16)|(0x00<<8)|ABFM03_ADDR_NUM)%16);

	switch(hw_2_4G_data.mode)
	{
		case AUTO_MATCH_MODE:
		{
			set_auto_math_freq_2470M();
			break;
		}

		case NORMAL_MODE:
		default:
		{
			set_normal_freq();
			break;
		}
	}

	HW_RF_RX_EN();			//æŽ¥æ”¶ä½¿èƒ�?
}

void HW_RF_TX(uint8_t *TX_Buf, uint16_t Length)
{
	uint16_t _reg;
	uint8_t cnt = 0;

	HW2000_Write_Reg(0x21, 0x0100); // TX enable
	Delay_MS(1);//delete if lower spi rate,
	HW2000_Write_Reg(0x3B, 0x8000); //clear TX FIFO
	HW2000_Write_Fifo(0x32, TX_Buf, Length); //write FIFO
	HW2000_Write_Reg(0x36, 0x0081);  //FIFO0 occupy
	HW2000_Write_Reg(0x37, 0x0000); //FIFO1 disable

	do{
		Delay_MS(1);
		_reg = HW2000_Read_Reg(0x3D);	//å‘å°„�?Š¶æ�?åˆ¤æ�??
		cnt++;

		if(cnt > 10)
		{
			break;
		}
	}while(!(_reg & 0x0001));

	HW2000_Write_Reg(0x36, 0x0080); //FIFO0 no occupy
	HW2000_Write_Reg(0x3D, 0x0008); //clear int0
	HW2000_Write_Reg(0x21, 0x0000); //TX disable
}

void HW_RF_RX_EN(void)
{
	 HW2000_Write_Reg(0x36, 0x0080); //FIFO0 enable
	 HW2000_Write_Reg(0x37, 0x0000); //FIFO1 disable
	 HW2000_Write_Reg(0x21, 0x0080); //RX enable
}

void HW_RX_Data_Process(void)
{
	uint16_t _reg, i;
	uint8_t And_Check = 0;
	static uint8_t ClearScreemCount = 0;
	static uint8_t printf_send_cnt;


	_reg = HW2000_Read_Reg(0x3D);

	if(_reg & 0x0001)
	{
		_reg = HW2000_Read_Reg(0x36);

		for(i = 0; i < HW_BUFFER_LEN; i++){
			RX_BUF[i] = 0;
		}

		if((_reg & 0x2000) == 0)	//CRC Ð£ï¿½ï¿½
		{
			HW2000_Read_Fifo(0x32, RX_BUF, HW_BUFFER_LEN);

			for(i=0;i<(HW_BUFFER_LEN-1);i++)
			{
				And_Check += RX_BUF[i];  //Ð£ï¿½ï¿½
			}

			// printf_send_cnt ++;
			// if(printf_send_cnt >= 5)
			// {
			// 	printf_send_cnt = 0;
			// 	// printf (" \r\n");
			// 	for(i=0; i < INDEX_AND; i++)
			// 	{
			// 	    printf (" %0x", RX_BUF[i]);
			// 	}
			//  //	printf (" tmp_id_data: 0x%0x--tmp_id:0x%0x", tmp_id_data, hw_2_4G_data.tmp_device_id);
			//  }

		 	if(hw_2_4G_data.mode == AUTO_MATCH_MODE)
			{
				tmp_id_data = (RX_BUF[INDEX_DEVICE] << 24) | (RX_BUF[INDEX_ID1] << 16) | (RX_BUF[INDEX_ID2] << 8) | RX_BUF[INDEX_ID3];

			 	if(hw_2_4G_data.tmp_device_id == tmp_id_data)
				{
					hw_2_4G_data.recv_new_id_cnt ++;
					if(hw_2_4G_data.recv_new_id_cnt >= 3)
					{
						hw_2_4G_data.save_new_id_flag = 1;
						peer_code = hw_2_4G_data.tmp_device_id;
						SavePeerCode(peer_code);
						printf (" Temp_id:0x%0x", hw_2_4G_data.tmp_device_id);
					}

					if((hw_2_4G_data.recv_new_id_cnt >= 10) || ((RX_BUF[INDEX_LEVEL] == 0x5a)&&(RX_BUF[INDEX_LOCK] == 0xa5)))
					//if((RX_BUF[INDEX_LEVEL] == 0x5a)&&(RX_BUF[INDEX_LOCK] == 0xa5))
					{
						printf (" Temp_id:0x%0x", hw_2_4G_data.tmp_device_id);
						printf (" recv reset msg");
						peer_code = hw_2_4G_data.tmp_device_id;
						SavePeerCode(peer_code);
						hw_2_4G_data.mode = NORMAL_MODE;

						hw_2_4G_data.recv_new_id_cnt = 0;
						hw_2_4G_data.save_new_id_flag = 0;

						HW2000_Init_250k();
						printf (" 2.4G save new id reset");
						return;
					}
				}
				hw_2_4G_data.tmp_device_id = (RX_BUF[INDEX_DEVICE] << 24) | (RX_BUF[INDEX_ID1] << 16) | (RX_BUF[INDEX_ID2] << 8) | RX_BUF[INDEX_ID3];
				hw_2_4G_data.send_id_flag = 1;

				// printf_send_cnt ++;
				// if(printf_send_cnt >= 5)
				// {
				// 	printf_send_cnt = 0;
				// 	// printf (" \r\n");
				// 	for(i=0; i < INDEX_AND; i++)
				// 	{
				// 	    printf (" %0x", RX_BUF[i]);
				// 	}
				//  	printf (" Recv_2.4G mode: %d--tmp_id:0x%0x", hw_2_4G_data.mode, hw_2_4G_data.tmp_device_id);
				//  }


			}

			if(And_Check == RX_BUF[INDEX_AND])
			{

				if((RX_BUF[INDEX_START] == 0xAA) && (RX_BUF[INDEX_LEN] == 0x0A) && (RX_BUF[INDEX_END] == 0x66))
				{

					SysInfo.recv_frame_cnt ++;


					if(((RX_BUF[INDEX_DEVICE]<<24)|(RX_BUF[INDEX_ID1]<<16)|(RX_BUF[INDEX_ID2]<<8)|RX_BUF[INDEX_ID3])==peer_code)
					{
						//printf ("\n\r check peer code ok --------\n\r");
						if((_LOCK_STATE)(RX_BUF[INDEX_LOCK] & 0x0f) == Un_Lock)  // Ö÷»ú½âËøÖ¸Áî
						{
							Auto_30S_Lock_Cnt = 0;
							IDLE_120S_Cnt = 0;
							SysInfo.Lock_State = Un_Lock;
							data_disp.Lock_State = Un_Lock;
							// printf ("\n\r  Wireless_Open_clock \n\r");
						}
						SysInfo.Err_State = (_ERR_STATE)(RX_BUF[INDEX_LOCK] >> 4);

						if(SysInfo.Level_TypeDef != (_LEVEL_TypeDef)RX_BUF[INDEX_LEVEL])
						{
							if(0x1F == (_LEVEL_TypeDef)RX_BUF[INDEX_LEVEL])
							{
								SysInfo.Level_State = No_State;

								if(Level_3 == SysInfo.Level_TypeDef) //×î´óÖµ
								{
									data_disp.level_inverse_cnt = MAX_INVESER_CNT;
									// printf ("\n\r change level_ err :=============\n\r");
								}

							}
							else
							{
								SysInfo.Level_TypeDef = (_LEVEL_TypeDef)RX_BUF[INDEX_LEVEL];
								SysInfo.Level_State =No_State;
								SysInfo.Last_Level_TypeDef=SysInfo.Level_TypeDef;
								ClearScreemCount=0;
								data_disp.Level = SysInfo.Last_Level_TypeDef;

							}

							Auto_30S_Lock_Cnt = 0;
							IDLE_120S_Cnt = 0;

						//	printf ("\n\r diff recv[level]=%d \n\r",(_LEVEL_TypeDef)RX_BUF[INDEX_LEVEL]);
						}
						// else
						// {
							// if(SysInfo.Level_TypeDef==0x1F)
							// {
							// 	ClearScreemCount++;
							// 	data_disp.level_undisp_flag = 1;
							// 	printf ("\n\r same recv[level]=%d \n\r",(_LEVEL_TypeDef)RX_BUF[INDEX_LEVEL]);
							// }

							// if(ClearScreemCount>=2)
							// {
							// 	ClearScreemCount=0;
							// 	SysInfo.Level_TypeDef=SysInfo.Last_Level_TypeDef;
							// 	SysInfo.Level_State =No_State;
							// }
						// }


						// ui_draw_clear_screen();
						// if( SysInfo.Err_State != Sys_Err_None)
						// {
						// 	display_err_show((eALARM_TYPE)SysInfo.Err_State);
						// }
						// else
						// {
						// 	display_ems_level((uint8_t)SysInfo.Level_TypeDef);
						// }

						SysInfo.Connect = 1;
						SysInfo.recv_msg_flag = 1;
						data_disp.Connect = 1;
						// RX_Time = 0;
						// Reset_Cnt = 0;
					}
				}
			}
		}
		HW2000_Write_Reg(0x3B, 0x0080); //clear RX FIFO Ö¸ï¿½ï¿½
		HW2000_Write_Reg(0x3D, 0x0008); // clear int0
		HW2000_Write_Reg(0x21, 0x0000); //RX disable
	}

	// if(RX_Time > 500)
	// {
	// 	RX_Time = 0;
	// 	Reset_Cnt++;
	// 	HW2000_Init_250k();
	// }

	// if(Reset_Cnt > 10)
	// {
	// 	Reset_Cnt = 0;
	// 	SysInfo.Connect = 0;
	// }
}


/*
2.4G·¢ËÍº¯Êý£¬ÐèÒª¶¨Ê±·¢ËÍÐÅÏ¢£¬ÒÔ¹²ºÍÖ÷»úµÄ²éÑ¯
*/
void HW_TX_Data_Process(void)
{
	uint8_t i;
	static uint8_t printf_send_cnt;

	HW2000_Write_Reg(0x21, 0x0000);  // ï¿½Ø±Õ½ï¿½ï¿½ï¿½
	HW2000_Write_Reg(0x3D, 0x0008); // clear int0
	Delay_MS(1);
	TX_BUF[INDEX_START] = 0x55;//ï¿½ï¿½Ê¼ï¿½Ö½ï¿½
	TX_BUF[INDEX_LEN] = 0x0A;//ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½


	if(SysInfo.Lock_State == Un_Lock)
	{
		TX_BUF[INDEX_LEVEL] = SysInfo.Level_State;//ï¿½ï¿½Î»ï¿½Ó¼ï¿½×´Ì¬
	}
	else
	{
		TX_BUF[INDEX_LEVEL] = No_State;
	}

	if((hw_2_4G_data.send_id_flag)&&(hw_2_4G_data.mode == AUTO_MATCH_MODE))
	{
		TX_BUF[INDEX_DEVICE] = ((hw_2_4G_data.tmp_device_id >> 24) & 0xff);
		TX_BUF[INDEX_ID1] = ((hw_2_4G_data.tmp_device_id >> 16) & 0xff);
		TX_BUF[INDEX_ID2] = ((hw_2_4G_data.tmp_device_id >> 8) & 0xff);
		TX_BUF[INDEX_ID3] = (hw_2_4G_data.tmp_device_id & 0xff);

		if(hw_2_4G_data.save_new_id_flag)
		{
			TX_BUF[INDEX_LOCK] = 0XFA;//ï¿½ï¿½ï¿½ï¿½×´Ì¬
		}
		else
		{
			TX_BUF[INDEX_LOCK] = 0;
		}


	}
	else
	{
		TX_BUF[INDEX_DEVICE] = ((peer_code >> 24) & 0xFF);//0xA2;//ï¿½ï¿½Ö·1
		TX_BUF[INDEX_ID1] = ((peer_code >> 16) & 0xFF);//0x00;//ï¿½ï¿½Ö·2
		TX_BUF[INDEX_ID2] = ((peer_code >> 8) & 0xFF);//0x00;//ï¿½ï¿½Ö·3
		TX_BUF[INDEX_ID3] = (peer_code & 0xFF);//ABFM03_ADDR_NUM;//ï¿½ï¿½Ö·4

		TX_BUF[INDEX_LOCK]=SysInfo.Lock_State;//ï¿½ï¿½ï¿½ï¿½×´Ì¬
	}


	if(SysInfo.Lock_State == Un_Lock)
	{
		TX_BUF[INDEX_LEVEL] = SysInfo.Level_State;//ï¿½ï¿½Î»ï¿½Ó¼ï¿½×´Ì¬
	}else{
		TX_BUF[INDEX_LEVEL] = No_State;
	}

	TX_BUF[INDEX_END] = 0x66;//ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½
	TX_BUF[INDEX_AND] = 0x00;
	for(i=0;i<(HW_BUFFER_LEN-1);i++)
	{
		TX_BUF[INDEX_AND] += TX_BUF[i]; //Ð£ï¿½ï¿½Î»
	}
	HW_RF_TX(TX_BUF, HW_BUFFER_LEN);

	//  printf_send_cnt ++;
	// if(printf_send_cnt > 100)
	//  {
	//  	printf_send_cnt = 0;
	// 	printf (" send msg --100cnt");
	//  }
	SysInfo.send_frame_cnt ++;
}


// 自动配对时的频率
void set_auto_math_freq_2470M(void)
{
	HW2000_Write_Reg(0x22, 0x1800);
	HW2000_Write_Reg(0x25, 0x9A6); // Total frea =  0x9A6 + 0x1800 & 0x0x3f = 2.470G
}


// 正常通讯时的频率：2402 + devicd_id %16
void set_normal_freq(void)
{
	HW2000_Write_Reg(0x22, 0x1800+(peer_code%16));
	HW2000_Write_Reg(0x25, 0x962); // Total frea =  0x962 + 0x1800 & 0x0x3f = 2.470G
}



/*
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
  0.0xAAï¿½ï¿½Ê¼ï¿½Ö½ï¿½
  1.ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½
  2.ï¿½ï¿½Ö·1
  3.ï¿½ï¿½Ö·2
  4.ï¿½ï¿½Ö·3
  5.ï¿½ï¿½Ö·4
  6.ï¿½ï¿½Î»×´Ì¬
  7.ï¿½ï¿½ï¿½ï¿½×´Ì¬&ï¿½ï¿½ï¿½ï¿½×´Ì¬ ï¿½ï¿½ï¿½ï¿½Î»ï¿½æ´¢ï¿½ï¿½ï¿½ï¿½×´Ì¬ ï¿½ï¿½ï¿½ï¿½Î»ï¿½æ´¢ï¿½ï¿½ï¿½ï¿½×´Ì¬
  8.0x66ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½
  9.ï¿½ï¿½Ð£ï¿½ï¿½ï¿½Ö½ï¿½

  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
  0.0x55ï¿½ï¿½Ê¼ï¿½Ö½ï¿½
  1.ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½
  2.ï¿½ï¿½Ö·1
  3.ï¿½ï¿½Ö·2
  4.ï¿½ï¿½Ö·3
  5.ï¿½ï¿½Ö·4
  6.ï¿½ï¿½Î»ï¿½Ó¼ï¿½×´Ì¬
  7.ï¿½ï¿½ï¿½ï¿½×´Ì¬
  8.0x66ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½
  9.ï¿½ï¿½Ð£ï¿½ï¿½ï¿½Ö½ï¿½
*/


