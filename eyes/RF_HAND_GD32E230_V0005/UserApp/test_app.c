/***********************************************************
 * @file    test_app.c
 * @brief  测试应用程序
 * @version V1.0
 * @date    2023.02.02
 ***********************************************************/
#include "test_app.h"
#include "math.h"

Test_Type Test;
extern u16 Temp_ctrl_RF_time(void);
/*
*************************************************************
* 功能说明: 测试应用初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Test_Init(void)
{
	Test.State = Func_DISABLE;
	Test.Comm = None_CMD;
	Test.NTC_TARGET = 450;
	Test.Adj_Cnt = 0;
}



/*
*************************************************************
* 功能说明: 测试应用初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
static void test_ems_rf_ctrl(u16 RT_Time)
{
			// 输出控制
	if (Device.OutCount == 0)
	{
		Ems_Control(Func_ENABLE);
	}
	else if (Device.OutCount == Device.EMS_Time)
	{
		Ems_Control(Func_DISABLE);
	}
	else if (Device.OutCount == (Device.EMS_Time + 1))
	{
		RF_Control(Func_ENABLE);
	}
	else if (Device.OutCount == (Device.EMS_Time + 1 + RT_Time))
	{
		RF_Control(Func_DISABLE);
	}
}
/*
*************************************************************
* 功能说明: 主输出周期处理函数,总输出时间25MS
* 形    参: 无
* 返 回 值: 无 1ms进入一次
*************************************************************
*/
void Test_Mode_Device_Out_Process(void)
{
	static u16 MsCnt = 0; // 毫秒钟计数
	static u8 Load_first = 0;
	static u8 printf_cnt = 0;
	static u16 led_trigle_cnt = 0;
	static u8 all_leds_on_flag = 0;
	static u16 test_mode_print_cnt = 0;
	static u16 read_temp_cnt = 0;
    static u16 test_1s_cnt = 0;
	u8 bufer[10] = {0};
	uint16_t Crc;

	u16 RF_T = 0;

	if(Test.State == Func_DISABLE)
	{
		return;
	}

	if ((Power.BatLevel <= BAT_LEVEL_LOW5))
	{
		RF_Control(Func_DISABLE);
		Ems_Control(Func_DISABLE);
		return;
	}

	// RF_T = Temp_ctrl_RF_time();
	RF_T = Ctrl_RF_time_depend_on_level();
	MsCnt++;

	switch(Test.mode_sta)
	{

		case ONLY_RF_TEST_STA:
		{
			// 输出控制

			if (Device.OutCount == (Device.EMS_Time + 1))
			{
				Device.IdleCnt = 0;  // 无负载，运行时长
				Ems_Control(Func_DISABLE);
				RF_Control(Func_ENABLE);
				// printf_cnt ++;
				// if(printf_cnt > 40)
				// {
				// 	printf_cnt = 0;
				// 	printf("ONLY_RF_TEST_STA  \r\n");
				// }
			}
			else if (Device.OutCount == (Device.EMS_Time + 1 + RF_T))
			{
				RF_Control(Func_DISABLE);
			}

			break;
		}

		case ONLY_EMS_TEST_STA:
		{

			// 输出控制
			if (Device.OutCount == 0)
			{
				Device.IdleCnt = 0;  // 无负载，运行时长
				RF_Control(Func_DISABLE);
				Ems_Control(Func_ENABLE);
				// printf_cnt ++;
				// if(printf_cnt > 40)
				// {
				// 	printf_cnt = 0;
				// 	printf("ONLY_EMS_TEST  \r\n");
				// }
			}
			else if (Device.OutCount == Device.EMS_Time)
			{
				Ems_Control(Func_DISABLE);
			}

			break;
		}
		case Ntc_Set_STA:
		case Read_Flume_Temp_STA: // 读取水槽温度，关闭输出
		{
			read_temp_cnt ++;        // 200ms 打印一次数据
			if(read_temp_cnt >= 1000)
			{
				read_temp_cnt = 0;
				RF_Control(Func_DISABLE);
				Ems_Control(Func_DISABLE);
				//printf("RF_tmp=%d.%d *C, Rf_time =%d\r\n", RF.Temp/10, RF.Temp%10, RF.duty_time);
				//printf("RF_tmp=%d.%d *C\r\n", RF.Temp/10, RF.Temp%10);
				// bufer[0] = 0x10;
				// bufer[1] = 0x08;
				// bufer[2] = 0x00;
				// bufer[3] = 0x03;
				// bufer[4] = RF.Temp / 10;
				// bufer[5] = RF.Temp % 10;

				// Crc = crc16_compute(bufer, 6);
				// bufer[6] = ((Crc >> 8) & 0xff);
				// bufer[7] = (Crc & 0xff);
				// Drv_Uart_Send(bufer, 0x08);
			}
			break;
		}

		case Aging_Model_SAT:   //Aging_Model_SAT == Test.mode_sta
		{
			if(0 == Test.EMS_RF_out_flag)
			{
				Ems_Control(Func_DISABLE);
				RF_Control(Func_DISABLE);
				break;
			}

			// if ((Power.BatLevel <= BAT_LEVEL_LOW20))   // 当电压低于20%时，不再输出
			if(Power.adc_val <= BAT_3V6_ADC_VAL)
			{
				Test.EMS_RF_out_flag = 0;
				Test.Charge_flag = 1;
				printf("bat low to charge \r\n");
				break;
			}

			test_1s_cnt ++;
			if(test_1s_cnt > 1000)
			{
				test_1s_cnt = 0;
				Test.Ems_RF_cnt ++;
				//  printf("Aling_mode: run =%d,finish_flag = %d \r\n", Test.Ems_RF_cnt, Test.Aging_finish_flag);
			}

			if(Test.Ems_RF_cnt < AGING_EMS_RF_MAX_CNT)
			{
				test_ems_rf_ctrl(RF_T);
			}
			else if(Test.Charge_cnt < AGING_CHARGE_MAX_CNT)
			{
				Test.EMS_RF_out_flag = 0;
				Test.Ems_RF_cnt = AGING_EMS_RF_MAX_CNT;
			}
			else
			{
				if(0 == Test.Aging_finish_flag)
				{
					Test.Aging_finish_flag = 1;
					Device_Data_Write();
				}
			}
			break;
		}

		case Inset_Life_Test_STA:   //Aging_Model_SAT == Test.mode_sta
		{
			if(0 == Test.EMS_RF_out_flag)
			{
				Ems_Control(Func_DISABLE);
				RF_Control(Func_DISABLE);
				break;
			}

			// if ((Power.BatLevel <= BAT_LEVEL_LOW20))   // 当电压低于20%时，不再输出
			if(Power.adc_val <= BAT_3V6_ADC_VAL)
			{
				Test.EMS_RF_out_flag = 0;
				Test.Charge_flag = 1;
				printf("bat low to charge \r\n");
				break;
			}

			test_1s_cnt ++;
			if(test_1s_cnt > 1000)
			{
				test_1s_cnt = 0;
				Test.Ems_RF_cnt ++;
				//  printf("Aling_mode: run =%d,finish_flag = %d \r\n", Test.Ems_RF_cnt, Test.Aging_finish_flag);
			}

			if(Test.Ems_RF_cnt < INSET_LIFE_EMS_RF_MAX_CNT)       // 电量充足时，由时间控制输出
			{
				test_ems_rf_ctrl(RF_T);
				Test.Charge_cnt  = 0;
			}
			else //if(Test.Charge_cnt < INSET_LIFE_CHARGE_MAX_CNT)
			{
				Test.Charge_flag = 1;
				Test.EMS_RF_out_flag = 0;
				Test.Ems_RF_cnt = 0;
			}

			break;
		}


		default:
		{
			if (RF.Load == 1)
			{
				Device.IdleCnt = 0;
				RF.UnLoad_Count = 0;

				if (RF.Load_First == 0)
				{
					RF.Load_First = 1;
					Motor_Start_Time(200);
					//printf("Device_Err_Off_motor run \r\n");
				}

				// 输出时间计数
				if (MsCnt >= 1000)
				{
					MsCnt = 0;
					//Device.UseCount++;
				}

				// 输出控制
				test_ems_rf_ctrl(RF_T);

				test_1s_cnt ++;
				if(test_1s_cnt > 1000)
				{
					test_1s_cnt = 0;
					printf("RF_T =%d ms \r\n", RF_T);
				}

			}
			else
			{
				// 离开皮肤时间计数
				if (MsCnt >= 1000)
				{
					MsCnt = 0;
					Device.IdleCnt ++;
					if (RF.Load_First != 0)
					{
						RF.UnLoad_Count++;
						if (RF.UnLoad_Count >= 30)
						{
							RF.Load_First = 0;
							RF.UnLoad_Count = 0;
						}
					}
				}

				// // 无负载RF间断输出进行检测
				// Ems_Control(Func_DISABLE);
				// switch (Device.OutCount)
				// {
				// 	case 1:
				// 		RF_Control(Func_ENABLE);
				// 		break;
				// 	case 6:
				// 		RF_Control(Func_DISABLE);
				// 		break;
				// }

				// 无负载EMS间断输出进行检测
				RF_Control(Func_DISABLE);
				switch (Device.OutCount)
				{
				case 1:
					Ems_Control(Func_ENABLE);
					break;
				case 3:   //6
					Ems_Control(Func_DISABLE);
					break;
				}
				break;
			}
		}

	}
	Device.OutCount++;
	if (Device.OutCount >= 25)
	{
		Device.OutCount = 0;
	}

}


/*
*************************************************************
* 功能说明: 测试指令解析，调用周期10MS
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Test_Order_Process(void)
{
	uint8_t bufer[20] = {0};
	uint8_t buf_len = 0;
	uint8_t i = 0;
	uint16_t Crc = 0;
	uint16_t cmd = 0;
	uint16_t tmp;

	for (i = 0; i < Uart_RX_Len; i++)
	{
		bufer[i] = Uart_RX_Buffer[i];
	}
	buf_len = Uart_RX_Len;

	Uart_RX_Finish = Func_DISABLE;
	Uart_RX_Len = 0;

	if (bufer[1] < 2 || bufer[1] != buf_len)
	{
		return;
	}
	else
	{
		Crc = crc16_compute(bufer, buf_len - 2);
	}

	if (bufer[buf_len - 2] == ((Crc >> 8) & 0xff) && bufer[buf_len - 1] == (Crc & 0xff))
	{
		cmd = (bufer[2] << 8) | bufer[3];
		Test.State = Func_ENABLE;
	}
	else
	{
		return;
	}

	switch (cmd)
	{
		case Inset_Testmode_CMD:
		{
			if ((bufer[4] == 0) && (bufer[5] == 0))
			{
				Test.State = Func_ENABLE;
				Test.Comm = None_CMD;
				Device.State =  SYS_TEST;
				//printf("Enter Test test mode \r\n");
			}
			else if ((bufer[4] == 0xff) && (bufer[5] == 0xff))
			{
				Test.State = Func_DISABLE;
				Test.Comm = None_CMD;
				Device.State =  SYS_ON;
				Device.IdleCnt = _AUTO_OFF_ - 3;  // 退出后，3s关机
				//printf("Exit Test test mode \r\n");
			}
			bufer[0] = 0x10;
			Crc = crc16_compute(bufer, 6);
			bufer[6] = ((Crc >> 8) & 0xff);
			bufer[7] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 0x08);
			break;
		}

		case Ntc_Temp_CMD:
		{
		    if ((bufer[4] == 0xff) && (bufer[5] == 0xff))
			{
				Test.Comm = None_CMD;
				Test.mode_sta = None_STA;
				Device.State =  SYS_ON;
				//printf("Exit water temp read \r\n");
			}
			else
			{
				Test.mode_sta = Read_Flume_Temp_STA;
				Device.State =  SYS_TEST;
				//printf("Enter water temp read  \r\n");
				//printf("RF_tmp = %d.%d °C\r\n", RF.Sc_Temp/10, RF.Sc_Temp% 10);
			}
			bufer[0] = 0x10;
			bufer[1] = 0x08;
			bufer[2] = 0x00;
			bufer[3] = 0x03;
			bufer[4] = RF.Temp / 10;
			bufer[5] = RF.Temp % 10;

			Crc = crc16_compute(bufer, 6);
			bufer[6] = ((Crc >> 8) & 0xff);
			bufer[7] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 0x08);

			//Test.Comm = Ntc_Temp_CMD;
			Ems_Control(Func_DISABLE);
			RF_Control(Func_DISABLE);
			Test.State = Func_ENABLE;
			break;
		}

        case Bat_Value_CMD:
		{
			bufer[0] = 0x10;
			bufer[1] = 0x08;
			bufer[2] = 0x00;
			bufer[3] = 0x04;

			tmp = Power.adc_val *330*15/4096;
			//printf("Bat_val =%d.%d V \r\n",tmp/1000, tmp%1000);
			bufer[4] = tmp/1000;
			bufer[5] = tmp/10 % 100;  // 保留小数点两位

			Crc = crc16_compute(bufer, 6);
			bufer[6] = ((Crc >> 8) & 0xff);
			bufer[7] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 0x08);
			break;
		}

		case Comm_RF_Control:
		{
		 	if ((bufer[4] == 0xff) && (bufer[5] == 0xff))
			{
				Test.mode_sta = None_STA;
				//printf("Exit RF_Test \r\n");
				Device.State =  SYS_ON;
			}
			else
			{
				Test.mode_sta = ONLY_RF_TEST_STA;
				Device.State =  SYS_TEST;
				Ems_Control(Func_DISABLE);
				RF_Control(Func_DISABLE);
				//printf("Enter RF_Test \r\n");
			}

			bufer[0] = 0x10;
			bufer[1] = 0x0A;

			Crc = crc16_compute(bufer, 8);
			bufer[8] = ((Crc >> 8) & 0xff);
			bufer[9] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 10);
			break;
		}

		case Comm_EMS_Control:
		{
			if ((bufer[4] == 0xff) && (bufer[5] == 0xff))
			{
				Test.mode_sta = None_STA;
				Device.State =  SYS_ON;
				//printf("Exit ems_Test\r\n");
			}
			else
			{
				Test.State = Func_ENABLE;
				Test.mode_sta = ONLY_EMS_TEST_STA;
				Ems_Control(Func_DISABLE);
				RF_Control(Func_DISABLE);
				Device.State =  SYS_TEST;
				//printf("Enter EMS_Test \r\n");
			}
			bufer[0] = 0x10;
			bufer[1] = 0x0A;

			Crc = crc16_compute(bufer, 8);
			bufer[8] = ((Crc >> 8) & 0xff);
			bufer[9] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 10);
			break;
		}

		case Comm_Light_Control:
		{

		    if ((bufer[4] == 0xff) && (bufer[5] == 0xff))
			{
				Test.mode_sta = None_STA;
				Device.State =  SYS_ON;
				//printf("Exit light_Test  \r\n");
			}
			else
			{
				//printf("Enter light_Test \r\n");
				LED_1D_ON;
				LED_2D_ON;
				LED_3D_ON;
				Bat_Led_Low_Control(ENABLE);
				Bat_Led_Full_Control(ENABLE);
				LED_LASER_ON;
				Test.mode_sta = Light_Ctrl_STA;
				Device.State =  SYS_TEST;
			}
			bufer[0] = 0x10;
			bufer[1] = 0x08;

			Crc = crc16_compute(bufer, 6);
			bufer[6] = ((Crc >> 8) & 0xff);
			bufer[7] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 8);
			break;
		}

		case Comm_Software_Info:
		{
			if(bufer[4]==0x00 && bufer[5]==0x01)
			{
				bufer[4]=OUTSIDE_Version >> 8;
				bufer[5]=OUTSIDE_Version & 0xff;
			}
			else if(bufer[4]==0x00 && bufer[5]==0x02)
			{
				bufer[4]=INSIDE_Version >> 8;
				bufer[5]=INSIDE_Version & 0xff;
			}
			else
			{
				break;
			}
			bufer[0]=0x10;
			bufer[1]=0x08;
			bufer[2]=0x00;
			bufer[3]=Comm_Software_Info;
			Crc = crc16_compute(bufer, 6);
			bufer[6] = ((Crc >>8));
			bufer[7] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 0x08);
			break;
		}

		/*
			老化模式，输出10min，充电20分钟
		*/
		case Comm_Aging_Model:
		{
			if(bufer[4]==0x00)
			{
				if(0x00 == bufer[5])
				{
					Test.mode_sta = Aging_Model_SAT;
					Test.State = Func_ENABLE;
					Device.State = SYS_TEST;
					LED_LASER_ON;
					Test.EMS_RF_out_flag = 1;
					Test.Ems_RF_cnt = 0;
					Test.Charge_cnt = 0;
				}
				else if (0x01 == bufer[5])
				{
					if(0 == Test.Aging_finish_flag)
					{
						bufer[5] = 0xff;
					}
				}
			}
			else if(bufer[4]==0xFF && bufer[5]==0xFF)
			{
				Test.mode_sta = None_STA;
				Device.State = SYS_ON;
				Test.EMS_RF_out_flag = 1;
				Test.Ems_RF_cnt = 0;
				Test.Charge_cnt = 0;

			}
			bufer[0]=0x10;
			bufer[1]=0x08;
			bufer[2]=0x00;
			bufer[3]=Comm_Aging_Model;
			Crc = crc16_compute(bufer, 6);
			bufer[6] = ((Crc >>8));
			bufer[7] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 0x08);
			break;
		}


		case Comm_Ntc_Set: // 温度校准 45度
		{
			// if (Test.State == Func_ENABLE)
			// {
				// Test.NTC_TARGET = bufer[4] * 10;
				// Test.NTC_TARGET += bufer[5];
				// Test.Adj_Cnt = 0;

				// bufer[0] = 0x10;
				// Crc = crc16_compute(bufer, 6);
				// bufer[6] = ((Crc >> 8) & 0xff);
				// bufer[7] = (Crc & 0xff);
				// Drv_Uart_Send(bufer, 0x08);

				// RF_Control(Func_DISABLE);
				// Ems_Control(Func_DISABLE);
				// Test.Comm = Comm_Ntc_Set;



				// RF.Ntc_Offset = 450 - RF.Sc_Temp;
				// Device_Data_Write();
				// Device_Data_Read();

				Test.NTC_TARGET = 450;
				Test.mode_sta = Ntc_Set_STA;
				bufer[0] = 0x10;
				bufer[4] = 0x00;
				bufer[5] = 0x00;

				if(450 == RF.Ntc_Offset + RF.Sc_Temp)
				{
					bufer[5] = 0x01;
				}
				Crc = crc16_compute(bufer, 6);
				bufer[6] = ((Crc >> 8) & 0xff);
				bufer[7] = (Crc & 0xff);
				Drv_Uart_Send(bufer, 0x08);

			// }
			break;
		}

		case Comm_Default_Par:
		{
			RF.Ntc_Offset = 10;
			Device.Level = 1;
			Test.Aging_finish_flag = 0;
			Device_Data_Write();
			//printf("param set default value \r\n");

			bufer[0]=0x10;
			bufer[1]=0x08;
			bufer[2]=0x00;
			Crc = crc16_compute(bufer, 6);
			bufer[6] = ((Crc >>8));
			bufer[7] = (Crc & 0xff);
			Drv_Uart_Send(bufer, 0x08);
			break;
		}

	default:
		break;
	}
}

/*
*************************************************************
* 功能说明: CRC16校验函数
* 形    参: p_data 校验数据指针，size校验数据的长度
* 返 回 值: 校验数据
*************************************************************
*/
uint16_t crc16_compute(uint8_t const *p_data, uint32_t size)
{
	uint16_t crc = 0xFFFF;

	for (uint32_t i = 0; i < size; i++)
	{
		crc = (uint8_t)(crc >> 8) | (crc << 8);
		crc ^= p_data[i];
		crc ^= (uint8_t)(crc & 0xFF) >> 4;
		crc ^= (crc << 8) << 4;
		crc ^= ((crc & 0xFF) << 4) << 1;
	}

	return crc;
}

/*
*************************************************************
* 功能说明: 电极头NTC校准处理，调用周期1000MS
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void RF_Ntc_Adjust_Process(void)
{
	u8 tx_buf[10] = {0};
	u16 Crc = 0;

	Test.Adj_Cnt++;

	if (Test.Adj_Cnt > 120)
	{ // 超时校准
		Test.Adj_Cnt = 0;

		if (abs(Test.NTC_TARGET - RF.Sc_Temp) >= 60)
		{
			tx_buf[5] = 0x00;
		}
		else
		{
			RF.Ntc_Offset = Test.NTC_TARGET - RF.Sc_Temp;
			Device_Data_Write();
			Device_Data_Read();
			tx_buf[5] = 0x01;
		}

		tx_buf[0] = 0x10;
		tx_buf[1] = 0x08;
		tx_buf[2] = 0x00;
		tx_buf[3] = 0x17;
		tx_buf[4] = 0x00;

		Crc = crc16_compute(tx_buf, 6);
		tx_buf[6] = ((Crc >> 8) & 0xff);
		tx_buf[7] = (Crc & 0xff);
		Drv_Uart_Send(tx_buf, 0x08);

		Test.Comm = None_CMD;
	}
	else
	{
		tx_buf[0] = Test.Adj_Cnt;
		Drv_Uart_Send(tx_buf, 1);
	}
}
