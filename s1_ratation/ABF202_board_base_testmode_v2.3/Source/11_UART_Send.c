#include "includes.h"

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

#define version "ABF202_V2.3\n\r" //软件版本号
//#define buffsize 8//8
volatile uint16_t rx_len = 0; 
uint8_t recv_data_buff[buffsize] = {0};
uint8_t Tx_Data_Buff[buffsize] = {0};
//uint8_t Rx_Data_Length;

//void Sys_Command_Handle(uint8_t *datacom);
void BKP_SRAM_Init(void);

/*************************************************************************************
* FunctionName	 : User_Uart_Init()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void User_Uart_Init(void)
{
//  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, recv_data_buff, sizeof(recv_data_buff));//HAL_UART_Receive_DMA(&huart2, recv_data_buff, sizeof(recv_data_buff));//sizeof(recv_data_buff)
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	//BKP_SRAM_Init();
}
/*************************************************************************************
* FunctionName	 : BKP_SRAM_Init()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void BKP_SRAM_Init(void)
{
  //  TAMP->BKP0R = 0x55;
  HAL_RTCEx_BKUPWrite(&hrtc,0,0x55);  //备份寄存器写入0x55
  //HAL_RTCEx_BKUPRead(&hrtc,0);
}
/*************************************************************************************
* FunctionName	 : crc16_compute()
* Description    : CRC-16/CCITT-FALSE   0x1021,0xFFFF,0x0000
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
uint16_t crc16_compute(uint8_t const * p_data, uint32_t size, uint16_t const * p_crc)
{
  uint16_t crc = (p_crc == NULL) ? 0xFFFF : *p_crc;
  
  for (uint32_t i = 0; i < size; i++)
  {
    crc  = (uint8_t)(crc >> 8) | (crc << 8);
    crc ^= p_data[i];
    crc ^= (uint8_t)(crc & 0xFF) >> 4;
    crc ^= (crc << 8) << 4;
    crc ^= ((crc & 0xFF) << 4) << 1;
  }
  return crc;
}
/*************************************************************************************
* FunctionName	 : Uart2_Send_Data()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
//void Uart2_Send_Data(void)
//{
//  static uint8_t uart_cnt = 0;
//  
//  if(++uart_cnt >= 10)
//  {  
//    uart_cnt = 0;    
//    //HAL_UART_Transmit_DMA(&huart2,data_ver,sizeof(data_ver)-1);
//  }
//}
/*************************************************************************************
* FunctionName	 : HAL_UART_RxCpltCallback()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
volatile static uint8_t Length=0;
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	uint32_t temp;
//  if(huart->Instance == USART2)
//  {
////    if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE) != RESET) //如果是空闲状态
//    {
//      __HAL_UART_CLEAR_IDLEFLAG(&huart2);  //清除中断标志
//      HAL_UART_DMAStop(&huart2); //停止DMA接收
////			temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);// 获取DMA中未传输的数据个数			
////			rx_len =  buffsize - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
////			DMA1_Channel5->CNDTR = sizeof(recv_data_buff);
////			DMA1_Channel5->CMAR = (uint32_t)&recv_data_buff;
////			 __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);//获取DMA当前还有多少未填充temp =
//////      Rx_Data_Length = buffsize - temp; //计算接收到的数据数量
////      while(HAL_DMA_GetState(&hdma_usart2_tx) == HAL_DMA_STATE_BUSY);
////      HAL_UART_Transmit_DMA(&huart2,recv_data_buff,sizeof(recv_data_buff));
//      //Rx_Data_Length = 0;//接收数据长度清零
////			HAL_UART_DMAStop(&huart2); 
//			//hdma_usart2_tx.Init.MemInc = recv_data_buff;
////			hdma_usart2_rx.Instance->CNDTR= (uint32_t)&recv_data_buff;//DMA_InitStructure.DMA_MemoryBaseAddr 			
//      HAL_UARTEx_ReceiveToIdle_DMA(&huart2, recv_data_buff, sizeof(recv_data_buff));//HAL_UART_Receive_DMA(&huart2, recv_data_buff,sizeof(recv_data_buff));//开启下一轮接收sizeof(recv_data_buff)  rx_len
////			HAL_UART_Receive_DMA(&huart2,recv_data_buff,rx_len);
////			rx_len = 0;
//			Sys_Info.Test_Mode.Receive_Flag = 1;
////      Sys_Command_Handle(recv_data_buff);
//    }
//  }
//}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART2)  //判断是否是UART2
	 { 
			Sys_Info.Test_Mode.Receive_Flag = 1;         //置1表明接收结束
			HAL_UART_DMAStop(&huart2);              
			rx_len =Size;              //接收到的数据长度
	 }
}
/*************************************************************************************
* FunctionName	 : Manufacturer_Data_Send()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
const uint8_t Manufacturer_Data_Table[12] = {Direction_ID_Send,0x0C,AMIRO_ID>>8,AMIRO_ID,ABF_Series>>8,ABF_Series,S1>>8,S1,SOFT_VER,HARDWARE_VER,0x52,0x1C};//10 0C 00 01 00 02 00 01 21 60
void Manufacturer_Data_Send(void)
{
//  HAL_UART_Transmit_DMA(&huart2,(uint8_t *)Manufacturer_Data_Table,sizeof(Manufacturer_Data_Table));
	HAL_UART_Transmit(&huart2,(uint8_t *)Manufacturer_Data_Table,rx_len,100);
}
/*************************************************************************************
* FunctionName	 : Gsensor_Data_Send()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
uint8_t Gsensor_Data_Table[20];//10 0C 00 01 00 02 00 01 21 60
void Gsensor_Data_Send(void)
{
	uint16_t CRC_DATA;
	Gsensor_Data_Table[0] = Direction_ID_Send;
	Gsensor_Data_Table[1] = 0x13;
	Gsensor_Data_Table[2] = 0x00;
	Gsensor_Data_Table[3] = 0x0C;
	Gsensor_Data_Table[4] = 0xD4;
	Gsensor_Data_Table[5] = GU6500XYZdataACCEL.AXIS_X>> 8 & 0xff;
	Gsensor_Data_Table[6] = GU6500XYZdataACCEL.AXIS_X& 0xff;
	Gsensor_Data_Table[7] = GU6500XYZdataACCEL.AXIS_Y>> 8 & 0xff;
	Gsensor_Data_Table[8] = GU6500XYZdataACCEL.AXIS_Y& 0xff;
	Gsensor_Data_Table[9] = GU6500XYZdataACCEL.AXIS_Z>> 8 & 0xff;
	Gsensor_Data_Table[10] = GU6500XYZdataACCEL.AXIS_Z& 0xff;
	Gsensor_Data_Table[11] = GU6500XYZdataGYRO.AXIS_X >> 8 & 0xff;
	Gsensor_Data_Table[12] = GU6500XYZdataGYRO.AXIS_X & 0xff;
	Gsensor_Data_Table[13] = GU6500XYZdataGYRO.AXIS_Y >> 8 & 0xff;
	Gsensor_Data_Table[14] = GU6500XYZdataGYRO.AXIS_Y & 0xff;
	Gsensor_Data_Table[15] = GU6500XYZdataGYRO.AXIS_Z >> 8 & 0xff;
	Gsensor_Data_Table[16] = GU6500XYZdataGYRO.AXIS_Z & 0xff;		
	
  CRC_DATA = crc16_compute(Gsensor_Data_Table,17,0);
  Gsensor_Data_Table[17] = CRC_DATA >> 8 & 0xff;
  Gsensor_Data_Table[18] = CRC_DATA & 0xff;	
//  HAL_UART_Transmit_DMA(&huart2,(uint8_t *)Gsensor_Data_Table,19);
	HAL_UART_Transmit(&huart2,(uint8_t *)Gsensor_Data_Table,rx_len,100);
}
/*************************************************************************************
* FunctionName	 : TX_Command_Handle()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/

void TX_Command_Handle(uint16_t Comm, uint16_t data)
{
  uint16_t CRC_DATA;
  
  Tx_Data_Buff[0] = Direction_ID_Send;
  Tx_Data_Buff[1] = 0x08;//Length;//0x08;	
  Tx_Data_Buff[2] = Comm >> 8 & 0xff;
  Tx_Data_Buff[3] = Comm & 0xff;
  Tx_Data_Buff[4] = data >> 8 & 0xff;
  Tx_Data_Buff[5] = data & 0xff;
 
  CRC_DATA = crc16_compute(Tx_Data_Buff,6,0);//sizeof(Tx_Data_Buff)-2  Length-2
  Tx_Data_Buff[6] = CRC_DATA >> 8 & 0xff;//Length-2
  Tx_Data_Buff[7] = CRC_DATA & 0xff;//Length-1
//  HAL_UART_Transmit_DMA(&huart2,(uint8_t *)Tx_Data_Buff,8);//sizeof(Tx_Data_Buff)  sizeof(Tx_Data_Buff) Length
	HAL_UART_Transmit(&huart2,(uint8_t *)Tx_Data_Buff,rx_len,100);
}

void TX_Command_Handle_L10(uint16_t Comm, uint8_t data1 , uint8_t data2)
{
  uint16_t CRC_DATA;
  
  Tx_Data_Buff[0] = Direction_ID_Send;
  Tx_Data_Buff[1] = 0x0A;//Length;//0x08;	
  Tx_Data_Buff[2] = Comm >> 8 & 0xff;
  Tx_Data_Buff[3] = Comm & 0xff;
	Tx_Data_Buff[4] = 0;
  Tx_Data_Buff[5] = data1 & 0xff;
	Tx_Data_Buff[6] = 0;
  Tx_Data_Buff[7] = data2 & 0xff; 
  CRC_DATA = crc16_compute(Tx_Data_Buff,8,0);//sizeof(Tx_Data_Buff)-2
  Tx_Data_Buff[8] = CRC_DATA >> 8 & 0xff;
  Tx_Data_Buff[9] = CRC_DATA & 0xff;
//  HAL_UART_Transmit_DMA(&huart2,(uint8_t *)Tx_Data_Buff,10);//sizeof(Tx_Data_Buff)  sizeof(Tx_Data_Buff) Length
	HAL_UART_Transmit(&huart2,(uint8_t *)Tx_Data_Buff,rx_len,100);
}

/*************************************************************************************
* FunctionName	 : Sys_Command_Handle()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Sys_Command_Handle(uint8_t *datacom)
{
  uint16_t CRC_Data;
//	static uint8_t Length=0;
  
//	if(datacom[1]>2)
//	{
//		Length =  datacom[1];
//	}
//	else
//	{
//		Length = 2;		
//	}	
  CRC_Data = crc16_compute(recv_data_buff,rx_len-2,0);//Length
  if((datacom[rx_len-2] == (CRC_Data>>8&0xff)) && (datacom[rx_len-1] == (CRC_Data&0xff)))//Length
  {
    if(Direction_ID_Recv == datacom[0])
    {
      switch((uint16_t)(datacom[2]<<8|datacom[3]))
      {
      case Comm_Inset_Testmode :
				if(datacom[4]==0x00 && datacom[5]==0x00)
				{
					Sys_Info.Work_Mode = TESTMODE;
					Sys_Info.Test_Mode.TestMode = Auto_TestModel;
					SYS_BUS_SW_ON;
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
				}
				else
				{
					Sys_Info.Work_Mode = NORMAL;
//					Sys_Info.Test_Mode.TestMode = Auto_TestModel;
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;					
					SYS_BUS_SW_OFF;					
				}
				TX_Command_Handle(Comm_Inset_Testmode,Sys_Info.Test_onff_value);
				break;
      case Comm_Manufactur_Info :
        Manufacturer_Data_Send(); 
        break; //0x0000020000086270
      case Comm_Ntc_Temp :				
        TX_Command_Handle(Comm_Ntc_Temp,Sys_Info.Head_Temperature);
//				SYS_BUS_SW_OFF;
        break; //0x00000300000814c4
      case Comm_Batter_Value :
        TX_Command_Handle(Comm_Batter_Value,Batt_State.BATT_TRUE_Value);
        break; //0x00000400000845E9
     case Comm_Moto_Test :
			{
				if(datacom[4]==0x00 && datacom[5]==0x00)
				{
					LOAD_BUS_SW_ON;
					Moto_Control.Electric_Machinery_Onoff = 1;
//					Moto_Control.Moto_State = Start;
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
				}
				else
				{
					LOAD_BUS_SW_OFF;
//					Moto_Control.Moto_State = End;
					Moto_Control.Electric_Machinery_Onoff = 0;
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
				}
			}
			TX_Command_Handle(Comm_Moto_Test,Sys_Info.Test_onff_value);
				break;
      case Comm_LED_Test :
			{
				if(datacom[4]==0x00 && datacom[5]==0x00)
				{
					LOAD_BUS_SW_ON;
					LED1_ON;LED2_ON;LED3_ON;
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
				}
				else
				{
					LOAD_BUS_SW_OFF;
					LED1_OFF;LED2_OFF;LED3_OFF;
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
				}
			}
			TX_Command_Handle(Comm_LED_Test,Sys_Info.Test_onff_value);
				break;			
      case Comm_Electrode_Test :
				Sys_Info.Power_State = MACHINEON;
				LOAD_BUS_SW_ON;
//				RF_OUT_Start();
//				RF_OUT_PWM_Start();
//				RF_SET_VALUE_PWM_Start();
//				RF_Handler.RF_PID.data = Seal_Target_One;
//				Set_RF_Tim_Psc(15);  //先把RF调整到1Mhz
			
//				Sys_Info.PowerOn_Time_Counter = 21;
				if(RF_Handler.RF_Load_Flag)
				{
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
					RF_Handler.RF_Load_Flag = 0;
				}
				else
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
				TX_Command_Handle(Comm_Electrode_Test,Sys_Info.Test_onff_value);
				break;
      case Comm_RF_Test :				
					if(datacom[rx_len-4]==0xFF && datacom[rx_len-3]==0xFF) //关闭RF//Length
					{
						Sys_Info.Power_State = MACHINEOFF;
						LOAD_BUS_SW_OFF;						
						EMS_OUT_Stop(); 
						RF_OUT_Stop();
						RF_OUT_PWM_Stop(); //输出PWM 
				//    RF_SET_VALUE_PWM_Stop(); //调压PWM
						EMS_OUT_PWM_Stop(); //输出PWM
						Sys_Info.Test_onff_value = AUTO_TEST_OFF;
						TX_Command_Handle(Comm_RF_Test,Sys_Info.Test_onff_value);
					}
					else
					{
						Sys_Info.Test_Mode.RF_EMSTEST_Flag = 0;
						Sys_Info.Power_State = MACHINEON;
						LOAD_BUS_SW_ON;					
						switch(datacom[5])//给档位
						{
							case 1:	Sys_Info.Key_State.Seal_gear = GEARONE;break;
							case 2:	Sys_Info.Key_State.Seal_gear = GEARTWO;break;
							case 3:	Sys_Info.Key_State.Seal_gear = GEARTHREE;break;
							default:	break;
						}
//						if(Length==0x0a)
//						{
							Sys_Info.TestMode_RF_Fre = datacom[7];						
//						}
						TX_Command_Handle_L10(Comm_RF_Test, datacom[5] , Sys_Info.TestMode_RF_Fre);					
					}
//					Sys_Info.Test_Mode.RF_EMSTEST_Flag = 1;


				break;
      case Comm_EMS_Test : 
				if(datacom[rx_len-4]==0xFF && datacom[rx_len-3]==0xFF) //关闭EMS Length
				{
						Sys_Info.Power_State = MACHINEOFF;
						LOAD_BUS_SW_OFF;						
						EMS_OUT_Stop(); 
						RF_OUT_Stop();
						RF_OUT_PWM_Stop(); //输出PWM 
				//    RF_SET_VALUE_PWM_Stop(); //调压PWM
						EMS_OUT_PWM_Stop(); //输出PWM
						Sys_Info.Test_onff_value = AUTO_TEST_OFF;
						TX_Command_Handle(Comm_EMS_Test,Sys_Info.Test_onff_value);					
				}
				else
				{
					Sys_Info.Test_Mode.RF_EMSTEST_Flag = 1;
						Sys_Info.Power_State = MACHINEON;
						LOAD_BUS_SW_ON;				
						switch(datacom[5])//给档位
						{
							case 1:	Sys_Info.Key_State.Slide_gear = GEARONE;break;
							case 2:	Sys_Info.Key_State.Slide_gear = GEARTWO;break;
							case 3:	Sys_Info.Key_State.Slide_gear = GEARTHREE;break;
							default:	break;
						}
						TX_Command_Handle_L10(Comm_EMS_Test, datacom[5] , 1);						
				}
				break;
      case Comm_KEY_Test :				
				TX_Command_Handle(Comm_KEY_Test,Sys_Info.KEY_Value);
				break;
      case Comm_All_Light :
				if(datacom[4]==0x00 && datacom[5]==0x01)
				{
					LOAD_BUS_SW_ON;
					Set_Color_Handle(255,0,0,0);
					Set_Color_Handle(0,0,255,1);
					Set_Color_Handle(0,255,0,2);
					LED1_ON;LED2_ON;LED3_ON;
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
				}
				else
				{
					Set_Color_Handle(0,0,0,0);
					Set_Color_Handle(0,0,0,1);
					Set_Color_Handle(0,0,0,2);
					LED1_OFF;LED2_OFF;LED3_OFF;
					LOAD_BUS_SW_OFF;
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
				}
				LCD_output_transf(Led_Colour);    //RGB灯刷新
				TX_Command_Handle(Comm_All_Light,Sys_Info.Test_onff_value);
				break;
      case Comm_G_Sensor_Test :
				if(datacom[4]==0x00 && datacom[5]==0x00)
				{
					SENSOR_PWR_SW_ON;
					HAL_Delay(1);    //等待传感器供电稳定
					GY6550_INIT();   //陀螺仪初始化					
					GY6500_Read();
					Gsensor_Data_Send();
				}
				else
				{
					SENSOR_PWR_SW_OFF;
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
					TX_Command_Handle(Comm_G_Sensor_Test,Sys_Info.Test_onff_value);
				}
				break;
      case Comm_RGB_Sensor_Calib :
				if(datacom[4]==0x00 && datacom[5]==0x00)
				{
					BACKLIGHT_OFF;
					RGB_BACKLIGHT_ON;        //开启颜色识别传感器背光
					SENSOR_PWR_SW_ON;        //颜色传感器上电
					HAL_Delay(1);
					#ifdef COLOR_SENSOR_ENABLE
					LTR831_INIT();
					#endif	
          Cal_Key.Calibraton_R_Avg = 0;
          Cal_Key.Calibraton_B_Avg = 0;
          Cal_Key.Calibraton_Lux_Avg = 0;
          Cal_Key.Calibraton_Start_flag = 1;
          Cal_Key.timer2_cnt = HAL_GetTick();
          Set_Color_Handle(0,0,0,0);
          Set_Color_Handle(0,0,0,1);
          LCD_output_transf(Led_Colour);					

          Cal_Key.Calibraton_Start_flag  = 0;
          Cal_Key.Calibraton_Start_Cnt = 0;
          Cal_Key.Calibraton_R_Avg = Cal_Key.Calibraton_R_Avg /5;
          Cal_Key.Calibraton_B_Avg = Cal_Key.Calibraton_B_Avg /5;
          Cal_Key.Calibraton_Lux_Avg = 10000/(Cal_Key.Calibraton_Lux_Avg /5);
//          Flash_Data_Save_RGB_FACTOR(Cal_Key.Calibraton_R_Avg,1,Cal_Key.Calibraton_B_Avg,Cal_Key.Calibraton_Lux_Avg);
          
          White_Balance.Calibration_R = Cal_Key.Calibraton_R_Avg;
          White_Balance.Calibration_G = 1;
          White_Balance.Calibration_B = Cal_Key.Calibraton_B_Avg;
          White_Balance.Calibration_Lux = Cal_Key.Calibraton_Lux_Avg;
          
		  #ifdef COLOR_SENSOR_ENABLE
          Set_Color_Handle(255,255,255,0);
          Set_Color_Handle(RGB_Handle.R_SCALED,RGB_Handle.G_SCALED,RGB_Handle.B_SCALED,1);
		  #endif
		 
          LCD_output_transf(Led_Colour);
					Sys_Info.Test_onff_value = AUTO_TEST_ON;					
				}
				else
				{
					RGB_BACKLIGHT_ON;        //开启颜色识别传感器背光
					SENSOR_PWR_SW_ON;        //颜色传感器上电
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
				}
				TX_Command_Handle(Comm_RGB_Sensor_Calib,Sys_Info.Test_onff_value);				
				break;
      case Comm_RGB_Sensor_Test : 
								
				if(datacom[4]==0x00 && datacom[5]==0x00)
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
				else
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
				TX_Command_Handle(Comm_RGB_Sensor_Test,Sys_Info.Test_onff_value);
				break;
      case Comm_Aging_Model : 
				if(datacom[4]==0x00 && datacom[5]==0x00)
				{
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
					TX_Command_Handle(Comm_Aging_Model,Sys_Info.Test_onff_value);
					Sys_Info.Work_Mode = TESTMODE;
					Sys_Info.Test_Mode.TestMode = Aging_TestModel;
					Sys_Info.Power_State = MACHINEON;					
				}
				else
				{
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
					TX_Command_Handle(Comm_Aging_Model,Sys_Info.Test_onff_value);
					Sys_Info.Work_Mode = TESTMODE;
					Sys_Info.Test_Mode.TestMode = Auto_TestModel;
					Sys_Info.Power_State = MACHINEOFF;						
				}
				break;
      case Comm_Life_Test : 
				if(datacom[4]==0x00 && datacom[5]==0x00)
				{
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
					TX_Command_Handle(Comm_Life_Test,Sys_Info.Test_onff_value);
					Sys_Info.Work_Mode = TESTMODE;
					Sys_Info.Test_Mode.TestMode = Life_TestModel;
					Sys_Info.Power_State = MACHINEON;					
				}
				else
				{
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
					TX_Command_Handle(Comm_Life_Test,Sys_Info.Test_onff_value);
					Sys_Info.Work_Mode = TESTMODE;
					Sys_Info.Test_Mode.TestMode = Auto_TestModel;
					Sys_Info.Power_State = MACHINEOFF;						
				}				
				break;
      case Comm_Inset_Bootloader : 
        BKP_SRAM_Init();
        HAL_NVIC_SystemReset();
        break; //0x00001C000008DB8D
      case Comm_Check_Version : 
					Sys_Info.Test_onff_value = SOFT_VER;
					TX_Command_Handle(Comm_Check_Version,Sys_Info.Test_onff_value);			
				break;
      case Comm_Inset_Standby :
//						CRC_Data= crc16_compute(recv_data_buff,buffsize-2,0);
				if(datacom[4]==0x00 && datacom[5]==0x00)
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
				else
					Sys_Info.Test_onff_value = AUTO_TEST_OFF;
					TX_Command_Handle(Comm_Inset_Standby,Sys_Info.Test_onff_value);
					HAL_Delay(5);
						HAL_NVIC_SystemReset();
				break;
      case Comm_Quit_AUTOTest : 
					Sys_Info.Test_onff_value = AUTO_TEST_ON;
					TX_Command_Handle(Comm_Quit_AUTOTest,Sys_Info.Test_onff_value);
					HAL_Delay(5);
					HAL_NVIC_SystemReset();
				break;			
      default:break;
      }
    }
  }
	rx_len = 0;//清除计数
}






//int fputc(int ch,FILE *f)
//{
//  HAL_UART_Transmit(&huart2,(uint8_t *)&ch,1,2);
//  return ch;
//}
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void Uart2_Send_Data(void)
{
  static uint8_t uart_cnt = 0;
  
  if(++uart_cnt >= 10)
  {  
    uart_cnt = 0;
#if 1
//    //    printf("\n\r ==ARH001_DATA==\n\r");
//    //		printf("\n\r 1:%s\n\r",					version);
//    //    printf("\n\r 2:%d - %d - %d\n\r",TEMP_VALUE.TEMP_MOS,TEMP_VALUE.TEMP_LAMP,TEMP_VALUE.TEMP_CAP);
//    //    printf("\n\r 3:%d\n\r",					SysInfo.Charge_Cap_Times);
//    //    printf("\n\r 4:%d\n\r",					SysInfo.Fan_Speed_number);
//    //    printf("\n\r 5:%d\n\r",					SysInfo.CAP_Value_True);
//    //    printf("\n\r 6:%d\n\r",					VALUE_TRUE.CAP_TRUE_VALUE);
//    //    printf("\n\r 7:%d.%d\n\r",			VALUE_TRUE.DCIN_TRUE_VALUE/10,VALUE_TRUE.DCIN_TRUE_VALUE%10);
//    //    printf("\n\r 8:%d - %d\n\r",    (uint32_t)Flash_Times_TypeDef.Flash_Times_dataCnt,SysInfo.Save_Cap_Temp_Count);
//    
//    printf("\n\r 1:%s\n\r", version);
//		printf("\n\r 2:RED:%d - GREEN:%d - BLUE:%d - LUX:%d \n\r", RGB_Handle.r,RGB_Handle.g,RGB_Handle.b,RGB_Handle.RGB_Lux);
//    printf("\n\r 2:RED:%d - GREEN:%d - BLUE:%d - LUX:%d \n\r", RGB_Handle.r,RGB_Handle.g,RGB_Handle.b,RGB_Handle.RGB_Lux);
//    printf("\n\r 3:R_Factor:%f - B_Factor:%f\n\r",    RGB_Handle.R_Factor,RGB_Handle.B_Factor);
//    printf("\n\r 4:R:%d - G:%d - B:%d - lux:%d \n\r", RGB_Handle.R_SCALED,RGB_Handle.G_SCALED,RGB_Handle.B_SCALED,RGB_Handle.Lux_SCALED);

#endif
  }
  Tx_Data_Buff[0] = Direction_ID_Send;
  Tx_Data_Buff[1] = 0x08;//Length;//0x08;	
  Tx_Data_Buff[2] = 0x5A;
  Tx_Data_Buff[3] = 0x5A;
  Tx_Data_Buff[4] = 0xA5;
  Tx_Data_Buff[5] = 0xff;
  HAL_UART_Transmit(&huart2,(uint8_t *)Tx_Data_Buff, 5,100);
}
/**************************************************************************************
* FunctionName   :Aging_Model
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void Aging_Test(void)
{
	if(Sys_Info.Test_Mode.TestMode == Aging_TestModel)//Aging_Model
	{
		if(!Sys_Info.Test_Mode.Test_charge_Flag)
		{
			if((Batt_State.BatteryLevel == Batt_Level_Full))//(Batt_State.BATT_TRUE_Value > 4120)&&&&(Batt_State.BatteryLevel == Batt_Level_Full)//电已充满，进入放电模式//if(BAT_CHARGE_STATE_IN() == 1)//电已充满，进入放电模式 3900))//  4180  &&(BAT_CHARGE_STATE_IN() == 1)
			{
				Sys_Info.Test_Mode.chargetimes++;
				if(Sys_Info.Test_Mode.chargetimes < 2)//2 充满放一次电
				{
					Sys_Info.Test_Mode.Test_charge_Flag = 1;
	//				Sys_Power_On();
					LOAD_BUS_SW_ON; //LOAD_BUS_ON
					OVP_DISABLE;//OVP_ENABLE;
				}
				else
				{

					OVP_ENABLE;
					Sys_Info.Work_Mode = NORMAL;//自动退出该测试模式 
					Sys_Info.Power_State = MACHINEOFF; 
				}
			}
		}
		if((Batt_State.BATT_TRUE_Value < 3300) || Sys_Info.Test_Mode.First_charge_Flag)//电已放光，进入充电模式 3100
		{
			Sys_Info.Test_Mode.First_charge_Flag = 0;
			Sys_Info.Test_Mode.Test_charge_Flag = 0;
			OVP_ENABLE;
			BAT_CHARGE_ENABLE;
			LOAD_BUS_SW_OFF;
		}
		
		if(Sys_Info.Test_Mode.chargetimes)
		{
			if(Batt_State.BATT_TRUE_Value > 4100)
			{
				BAT_CHARGE_DISABLE;
				Sys_Info.Chargestop_Flag = 1;
			}
		}
	}
	else if(Sys_Info.Test_Mode.TestMode == Life_TestModel)//Life_Model
	{
					LOAD_BUS_SW_ON; //LOAD_BUS_ON
					OVP_ENABLE;//OVP_ENABLE;		
	}
}
/**************************************************************************************
* FunctionName   :Life_Test_Model
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void Life_Test(coid)
{
	
}







