#ifndef __USART_H__
#define __USART_H__

#include "User_System_Config.h"

typedef enum
{
	Comm_NULL,
	Comm_Inset_Testmode,
	Comm_manufacture_Info,
	Comm_Ntc_Temp,
	Comm_Batter_Value,
	Comm_Moto_Control,
	Comm_HeadLED_Control,
	Comm_Electrode_Test,
	Comm_RF_Control,
	Comm_EMS_Control,
	Comm_KEY_Control,
	Comm_Light_Control,
	Comm_GSensor_Control,
	Comm_RGBSensor_Calibra,
	Comm_RGBSensor_Control,
	Comm_Aging_Model,
	Comm_Inset_Life_Testl,
	Comm_Inset_Bootloader,
	Comm_Check_Version,
	Comm_Standby,
	Comm_Wireless_communication,
	comm_peer_code_set,
	comm_peer_code_get,
	comm_ntc_adj_set,
	comm_ntc_adj_get,
	comm_screem_test,
	comm_key_test,
	comm_2_4G_LOSS_RATE = 0x1c,
}_Tset_status_TypeDef;

typedef struct
{
  uint8_t  Test_Mode_Flag;
  uint8_t  Screem_Flag;
  uint8_t  Key_Flag;
  uint32_t Screem_Count;
	uint8_t  Key_State;
	uint8_t  Key_Value;
}_TEST_MODE_HANDLE_TypeDef;



void MX_USART1_UART_Init(void);
void MX_TIMER6_Init(void);
void Uart_Protocol_Process(void);

#endif /*__ USART_H__ */
