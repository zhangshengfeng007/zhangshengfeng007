/******************************************************************************
 * @file     hw2000b.c
 * @brief    hw2000b function
 *
 *
 * @version  1.2
 * @date     2017-03-21
 *
 * @author   fudl
 *
 * @note
 * @Copyright (C) 2017 Shanghai Eastsoft Microelectronics C0., Ltd. 123
 ******************************************************************************/
#include "includes.h"
#include "hw2000b.h"

uint8_t RX_BUF[HW_BUFFER_LEN], TX_BUF[HW_BUFFER_LEN];
extern uint32_t peer_code;

uint8_t Level_Delay = 0;
uint16_t hw2000_reset_count = 0;
uint32_t hw2000_log_time=0;
HW_2_4G_t hw_2_4G_data;
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
void Spi_Init(void)
{
	HW_CSN_HIGH();
	HW_CE_LOW();
	HAL_Delay(2);
	HW_CE_HIGH();
	HAL_Delay(5);
}
/******************************************************************************
 * @brief    hw2000b_write_reg
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
void HW_Write_Reg(uint8_t addr, uint16_t data)
{
	uint8_t temp_data;

	HW_CSN_LOW();
	addr |= 0x80;

	HAL_SPI_Transmit(&hspi2, &addr, 1, 10);

	temp_data = (data >> 8);
	HAL_SPI_Transmit(&hspi2, &temp_data, 1, 10);

	temp_data = data;
	HAL_SPI_Transmit(&hspi2, &temp_data, 1, 10);

	HW_CSN_HIGH();
}
/******************************************************************************
 * @brief    HW_Read_Reg
 *
 * @note	 read register function
 *
 * @param	 addr reg address
 * @retval	 data
 *
 * @version  1.0
 * @date
 * @author
 ******************************************************************************/
uint16_t HW_Read_Reg(uint8_t addr)
{
	uint8_t temp_data;
	uint16_t value;

	HW_CSN_LOW();

	HAL_SPI_Transmit(&hspi2, &addr, 1, 10);

	// HAL_SPI_TransmitReceive(&hspi1 ,&addr , &temp_data , 1 , 1000);

	HAL_SPI_TransmitReceive(&hspi2, &addr, &temp_data, 1, 1000);
	value = temp_data;

	HAL_SPI_TransmitReceive(&hspi2, &addr, &temp_data, 1, 1000);
	value = (value << 8);
	value = (value | temp_data);

	HW_CSN_HIGH();

	return value;
}
void HW_Write_Fifo(uint8_t addr, uint8_t *data, uint16_t length)
{
	HW_CSN_LOW();
	addr |= 0x80;
	HAL_SPI_Transmit(&hspi2, &addr, 1, 10);

	HAL_SPI_Transmit(&hspi2, data, length, 10);

	HW_CSN_HIGH();
}
void HW_Read_Fifo(uint8_t addr, uint8_t *data, uint16_t length)
{
	HW_CSN_LOW();

	HAL_SPI_Transmit(&hspi2, &addr, 1, 10);

	HAL_SPI_TransmitReceive(&hspi2, &addr, data, length, 10);

	HW_CSN_HIGH();
}

void Rx_Enable(void)
{
	HW_Write_Reg(0x36, 0x0080); //FIFO0 enable �?�?占用
	HW_Write_Reg(0x37, 0x0000); //FIFO1 disable
	HW_Write_Reg(0x21, 0x0080); //RX enable
}

void HW_Tx_Data(uint8_t *Buf, uint16_t Size)
{
	uint16_t _reg;
	uint8_t cnt = 0;

	HW_Write_Reg(0x21, 0x0100); // TX enable
	HAL_Delay(1);//delete if lower spi rate,
	HW_Write_Reg(0x3B, 0x8000); //clear TX FIFO
	HW_Write_Fifo(0x32, Buf, Size); //write FIFO
	HW_Write_Reg(0x36, 0x0081);  //FIFO0 occupy
	HW_Write_Reg(0x37, 0x0000); //FIFO1 disable

	do{
		HAL_Delay(1);
		_reg = HW_Read_Reg(0x3D);	//发射状�?�判�?
		cnt++;

		if(cnt > 10)
		{
			break;
		}
	}while(!(_reg & 0x0001));

	HW_Write_Reg(0x36, 0x0080); //FIFO0 no occupy
	HW_Write_Reg(0x3D, 0x0008); //clear int0
	HW_Write_Reg(0x21, 0x0000); //TX disable
}

void set_power_8(void) // 8db
{
	HW_Write_Reg(0x0B, 0x0873); // 8
}

// 自动配对时的频率
void set_auto_math_freq_2470M(void)
{
	HW_Write_Reg(0x22, 0x1800);
	HW_Write_Reg(0x25, 0x9A6); // Total frea =  0x9A6 + 0x1800 & 0x0x3f = 2.470G
}

// 正常通讯时的频率：2402 + devicd_id %16
void set_normal_freq(void)
{
	HW_Write_Reg(0x22, 0x1800+(peer_code%16));
	HW_Write_Reg(0x25, 0x962); // Total frea =  0x962 + 0x1800 & 0x0x3f = 2.470G
}


/***
 * 在数据收发时，收发两端芯片的工作频点必须设置一致。
    频点的可使用频率范围是 2402MHz ~ 2483MHz， 频率间隔为 1MHz，共 82 个可用频点。
   其配置方式如下：
     工作频点= 起始频点（RF_FREQ_BASE） +步长（STEP） x 频道号码（PLL_CH_NO）
     其中， 起始频点RF_FREQ_BASE (MHZ)为FREQBASE寄存器[0x25] 的Bit11-Bit0，默认值：0x0962 = 2402 MHZ
	        步长（STEP）为1MHz，
            频道号码 PLL_CH_NO 为 CHANNR 寄存器的[0x22]的 Bit6-Bit0。
 *
 *注： 需在使能发送或接收之前完成频点设置。
 * **/

// {
// void HW2000B_init_250K(void)
// {
// 	uint8_t i;
// 	uint16_t agcTab[18] = { 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x000A,
// 	0x0012, 0x0212, 0x0412, 0x064A, 0x084A, 0x0A4A,
// 	0x0A52, 0x0A92, 0x0C92, 0x0CD2, 0x0CDA, 0x0CE3};

// 	HW2000B_write_reg(0x4C, 0x55AA);
// 	for (i = 0; i < 18; i++) {
// 	HW2000B_write_reg(0x50 + i, agcTab[i]);
// 	}
// 	HW2000B_write_reg(0x0F, 0XFC04);
// 	HW2000B_write_reg(0x01, 0x4D58);
// 	HW2000B_write_reg(0x02, 0x04CC);
// 	HW2000B_write_reg(0x08, 0xB6C4);
// 	HW2000B_write_reg(0x09, 0xB442);
// 	HW2000B_write_reg(0x4A, 0x326C);
// 	HW2000B_write_reg(0x0B, 0x0873);
// 	HW2000B_write_reg(0x49, 0x1330);
// 	HW2000B_write_reg(0x27, 0x8F20);
// 	HW2000B_write_reg(0x48, 0x4300);
// 	HW2000B_write_reg(0x26, 0x000C);
// 	HW2000B_write_reg(0x28, 0x8402);
// 	HW2000B_write_reg(0x2C, 0x918B);
// 	HW2000B_write_reg(0x2A, 0x40FF);
// 	HW2000B_write_reg(0x1A, 0x0D31);
// 	HW2000B_write_reg(0x19, 0x0884);
// 	HW2000B_write_reg(0x20, 0xF000);
// 	HW2000B_write_reg(0x22, 0x1830); //默认 2.450GHz 频点配置，建议用户更改频点
// 	HW2000B_write_reg(0x40, 0xE7E7); //默认 pipe 地址，建议用户更改为自定义地址
// 	HW2000B_write_reg(0x41, 0xE7E7); //默认 pipe 地址，建议用户更改为自定义地址
// 	HW2000B_write_reg(0x42, 0xE7E7); //默认 pipe 地址，建议用户更改为自定义地址
// }

// }




//-------------------




void HW_Printf(uint8_t *data,uint8_t flag)
{
	if (flag == 1)
	{
		printf("\n\r TX_Data:%X\t%X\t%X\t%X\t%X\t%X\t%X\t%X\t%X\t%X \n\r",
				data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
	}
	else
	{
		printf("\n\r RX_Data:%X\t%X\t%X\t%X\t%X\t%X\t%X\t%X\t%X\t%X \n\r",
				data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
	}
}

// 运�?�周�?10MS
void HW_Process(void)
{
	uint8_t _reg, i;
	uint8_t And_Check = 0;
	uint8_t TX_Flag = 0;
	uint32_t temp;

	if (Level_Delay > 0)
	{
		Level_Delay--;
	}

	//if (SysInfo.Power_Value.state == System_ON || (SysInfo.Test_Mode.NTC_With_Alarm_Flag == 0x01)) // SysInfo.System_Err_status != System_Normal)//
	if (SysInfo.Power_Value.state == System_ON)
	{
		_reg = HW_Read_Reg(0x3D);	//读取�?�?

		if(_reg & 0x0001)
		{
			_reg = HW_Read_Reg(0x36);

			for(i = 0; i < HW_BUFFER_LEN; i++){
				RX_BUF[i] = 0;
			}

			if ((_reg & 0x2000) == 0)	//CRC 校验
			{
				HW_Read_Fifo(0x32, RX_BUF, HW_BUFFER_LEN);

				for(i=0;i<(HW_BUFFER_LEN-1);i++)
				{
					And_Check += RX_BUF[i];  //校验
				}

				if (And_Check == RX_BUF[INDEX_AND])
				{
					if (SysInfo.Test_Mode.STATUS == 0x07)
					{
						HW_Printf(RX_BUF, 0);
					}

					if(hw_2_4G_data.mode == AUTO_MATCH_MODE)
					{
						TX_BUF[INDEX_START] =0xF1; // 0xA2;//��??��1
						TX_BUF[INDEX_LEN] =0xF2; // 0xA2;//��??��1

						TX_BUF[INDEX_DEVICE] = 0xA2; // 0xA2;//��??��1
						TX_BUF[INDEX_ID1] = ((SysInfo.Device_id >> 16) & 0xFF); // 0x00;//��??��2
						TX_BUF[INDEX_ID2] = ((SysInfo.Device_id >> 8) & 0xFF);	// 0x00;//��??��3
						TX_BUF[INDEX_ID3] = (SysInfo.Device_id & 0xFF);			//

						TX_BUF[INDEX_END] = 0x66; // �����ֽ�
						TX_BUF[INDEX_AND] = 0;

						//hw_2_4G_data.recv_new_id_cnt ++;
						//if(hw_2_4G_data.recv_new_id_cnt >= 5)

						if((TX_BUF[INDEX_ID1] == RX_BUF[INDEX_ID1]) && (TX_BUF[INDEX_ID2] == RX_BUF[INDEX_ID2]) && (TX_BUF[INDEX_ID3] == RX_BUF[INDEX_ID3]))
						{
							//if(hw_2_4G_data.recv_new_id_cnt == 0)
							//{
								peer_code = SysInfo.Device_id & 0x00ffffff;
								peer_code |= 0xA2000000;
								SavePeerCode(peer_code);
							//}
							hw_2_4G_data.recv_new_id_cnt ++;
							TX_BUF[INDEX_LEVEL] = 0x5a;
							TX_BUF[INDEX_LOCK] = 0xa5;
							hw_2_4G_data.mode = NORMAL_MODE;
						}

						for(i=0;i<(HW_BUFFER_LEN-1);i++)
						{
							TX_BUF[INDEX_AND] += TX_BUF[i];
						}
						TX_Flag = 1;
						//hw2000_reset_count = 0;
					}
					else
					{
						if ((RX_BUF[INDEX_START] == 0x55) && (RX_BUF[INDEX_LEN] == 0x0A) && (RX_BUF[INDEX_END] == 0x66))
						{
						if ((RX_BUF[INDEX_DEVICE] == 0xA2) && ((RX_BUF[INDEX_DEVICE] << 24) | (RX_BUF[INDEX_ID1] << 16) | (RX_BUF[INDEX_ID2] << 8) | RX_BUF[INDEX_ID3]) == peer_code)
						{
							TX_BUF[INDEX_START] = 0xAA;						//?e��?��??��
							TX_BUF[INDEX_LEN] = 0x0A;						// 3��?����??��
							TX_BUF[INDEX_DEVICE] = ((peer_code >> 24) & 0xFF); // 0xA2;//��??��1
							TX_BUF[INDEX_ID1] = ((peer_code >> 16) & 0xFF); // 0x00;//��??��2
							TX_BUF[INDEX_ID2] = ((peer_code >> 8) & 0xFF);	// 0x00;//��??��3
							TX_BUF[INDEX_ID3] = (peer_code & 0xFF);			//
							if (SysInfo.WorkState == upkeep_mode)
							{
								if (RX_BUF[INDEX_LEVEL] == 1)
								{
									if (Level_Delay == 0)
									{
										Level_Delay = 60;
										if (EMS_Handle.vol_value < Level_30)
										{
											EMS_Handle.vol_value++;
											TX_BUF[INDEX_LEVEL] = EMS_Handle.vol_value;
										}
										else
										{
											TX_BUF[INDEX_LEVEL] = 31; // ��31���豸�����һ��?
										}
									}
									else
									{
										TX_BUF[INDEX_LEVEL] = EMS_Handle.vol_value;
									}
								}
								else if (RX_BUF[INDEX_LEVEL] == 2)
								{
									if (Level_Delay == 0)
									{
										Level_Delay = 60;
										if (EMS_Handle.vol_value > Level_1)
										{
											EMS_Handle.vol_value--;
											TX_BUF[INDEX_LEVEL] = EMS_Handle.vol_value;
										}
										else
										{
											TX_BUF[INDEX_LEVEL] = 31; // ��31���豸�����һ��?
										}
									}
									else
									{
										TX_BUF[INDEX_LEVEL] = EMS_Handle.vol_value;
									}
								}
								else
								{
									TX_BUF[INDEX_LEVEL] = EMS_Handle.vol_value;
								}
							}
							else if (SysInfo.WorkState == repair_mode)
							{
								if (RX_BUF[INDEX_LEVEL] == 1)
								{
									SysInfo.repair_level = SysInfo.Save_Data.repair_level; // �¶ȹ��ߣ��ή��1��
									//                if(++SysInfo.repair_level>Level_3)
									//                {
									//                  SysInfo.repair_level = Level_3 ;
									//                  TX_BUF[INDEX_LEVEL]= SysInfo.repair_level;
									//                }
									if (Level_Delay == 0)
									{
										Level_Delay = 60;
										if (SysInfo.repair_level < Level_3)
										{
											SysInfo.repair_level++;
											TX_BUF[INDEX_LEVEL] = SysInfo.repair_level;
										}
										else
										{
											TX_BUF[INDEX_LEVEL] = 31; // ��31���豸�����һ��?
										}
									}
									else
									{
										TX_BUF[INDEX_LEVEL] = SysInfo.repair_level;
									}
									SysInfo.Save_Data.repair_level = SysInfo.repair_level;
								}
								else if (RX_BUF[INDEX_LEVEL] == 2)
								{
									SysInfo.repair_level = SysInfo.Save_Data.repair_level; // �¶ȹ��ߣ��ή��1��
									//                if(--SysInfo.repair_level<Level_1)
									//                {
									//                  SysInfo.repair_level = Level_1 ;
									//                  TX_BUF[INDEX_LEVEL]= SysInfo.repair_level;
									//                }
									if (Level_Delay == 0)
									{
										Level_Delay = 60;
										if (SysInfo.repair_level > Level_1)
										{
											SysInfo.repair_level--;
											TX_BUF[INDEX_LEVEL] = SysInfo.repair_level;
										}
										else
										{
											TX_BUF[INDEX_LEVEL] = 31; // ��31���豸�����һ��?
										}
									}
									else
									{
										TX_BUF[INDEX_LEVEL] = SysInfo.repair_level;
									}
									SysInfo.Save_Data.repair_level = SysInfo.repair_level;
								}
								else
								{

									TX_BUF[INDEX_LEVEL] = SysInfo.repair_level;
								}
							}

							TX_BUF[INDEX_LOCK] = SysInfo.System_Err_status << 4; // SysInfo.Err_State << 4;       //�豸״̬:����״̬&����״̬ ����λ�洢����״̬ ����λ�洢����״̬
							TX_BUF[INDEX_LOCK] |= SysInfo.Lock_State;		   // �豸״̬:����״̬&����״̬ ����λ�洢����״̬ ����λ�洢����״̬

							if(SysInfo.Lock_State == RX_BUF[INDEX_LOCK])   // 2023 03 20  lock״̬��ͬ������0
							{
								SysInfo.Lock_State = Attonity;
							}

							TX_BUF[INDEX_END] = 0x66; // �����ֽ�
							TX_BUF[INDEX_AND] = 0;

							for(i=0;i<(HW_BUFFER_LEN-1);i++)
							{
								TX_BUF[INDEX_AND] += TX_BUF[i];
							}
							TX_Flag = 1;
							hw2000_reset_count = 0;

							upkeep_RF_Level(&EMS_Handle);

							if (SysInfo.Test_Mode.STATUS == 0x07)
							{
								HW_Printf(TX_BUF, 1);
							}
						}
					}
				}
				}
			}
			HW_Write_Reg(0x3B, 0x0080); //clear RX FIFO 指针
			HW_Write_Reg(0x3D, 0x0008); // clear int0
			HW_Write_Reg(0x21, 0x0000); //RX disable

			if(TX_Flag)
			{
				vTaskDelay(5);
				HW_Tx_Data(TX_BUF, HW_BUFFER_LEN);

				// printf (" TX data:\n\r");
				// for(i = 0; i < HW_BUFFER_LEN; i++)
				// {
				// 	printf (" TX[%d] =0x%0x ", i, TX_BUF[i]);

				// }

				// printf (" RX data:\n\r");
				// for(i = 0; i < HW_BUFFER_LEN; i++)
				// {
				// 	printf (" rx[%d] =0x%0x ", i, RX_BUF[i]);

				// }


			}
		}

		hw2000_reset_count += 10;
		if(hw2000_reset_count >= 1000)
		{
			hw2000_reset_count = 0;
			hw2000b_init_250k();
		}

		Rx_Enable();
	}


}





/****************************************************/
// 根据芯片使用笔记 AN1047, 第3.2寄存器初始化设置 表3-2
//
//**************************************************
void hw2000b_init_250k(void)
{
	uint8_t i;

	uint16_t agcTab[18] = {0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x000A,
						   0x0012, 0x0212, 0x0412, 0x064A, 0x084A, 0x0A4A,
						   0x0A52, 0x0A92, 0x0C92, 0x0CD2, 0x0CDA, 0x0CE3};
	Spi_Init();

	HW_Write_Reg(0x4C, 0x55AA); // close write protect
	for (i = 0; i < 18; i++)
	{
		HW_Write_Reg(0x50 + i, agcTab[i]);
	}

	HW_Write_Reg(0x01, 0x4D58);
	HW_Write_Reg(0x02, 0x04CC);
	HW_Write_Reg(0x08, 0xB6C4);
	HW_Write_Reg(0x09, 0xB442);
	HW_Write_Reg(0x0B, 0x0873); // tx power 8
	HW_Write_Reg(0x0F, 0xFC04);
	HW_Write_Reg(0x19, 0x0884);
	HW_Write_Reg(0x1A, 0x0D31);
	HW_Write_Reg(0x20, 0xF000); // preamble:16 bytes  sync:48 bits
	//HW_Write_Reg(0x26, 0x000C);
	HW_Write_Reg(0x26, 0xFC0C);//大电流驱动打开 （spi通讯速度高于 1MHZ）
	HW_Write_Reg(0x27, 0x8F20);
	HW_Write_Reg(0x28, 0x8402); // sync_thres:2bit
	HW_Write_Reg(0x2A, 0x40FF); // rate:250kbps, ACK time
	HW_Write_Reg(0x2C, 0x918B);

	HW_Write_Reg(0x48, 0x4300);
	HW_Write_Reg(0x49, 0x1330);
	HW_Write_Reg(0x4A, 0x326C);

	HW_Write_Reg(0x36, 0x0081);	//FIFO0 使能,pipe 0
	HW_Write_Reg(0x37, 0x0000); //FIFO1 disable

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
	Rx_Enable();	 //接收使能
}
