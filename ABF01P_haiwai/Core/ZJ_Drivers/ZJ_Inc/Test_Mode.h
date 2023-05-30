#ifndef _TEST_MODE_H
#define _TEST_MODE_H
#include "includes.h"

#if ARF001
	#define version  "V2.2.1"
#else
	#define version  "V1.3.1"
#endif

#define AutoTest 1
/*******************R1 PRO �»Դｵ��***************************/
#if (ARF001 == DEVICE_R1_RPO)
#define Manufacturer_Number_H 0x00
#define Manufacturer_Number_L 0x01
#define Model_Number_H  0x00
#define Model_Number_L  0x02
#define Device_Number_H 0x00
#define Device_Number_L 0x10
#define Soft_Version  0x21
#define Hardware_Version  0x21

/*******************R1 PRO MAX**********************************/
#elif (ARF001 == DEVICE_R1_RPO_MAX)
#define Manufacturer_Number_H 0x00
#define Manufacturer_Number_L 0x01
#define Model_Number_H  0x00
#define Model_Number_L  0x02
#define Device_Number_H 0x00
#define Device_Number_L 0x10
#define Soft_Version  0x13
#define Hardware_Version  0x15
/*******************R1_haiwai **********************************/
#elif (ARF001 == DEVICE_R1_HAIWAI)
#define Manufacturer_Number_H 0x00
#define Manufacturer_Number_L 0x01
#define Model_Number_H  0x00
#define Model_Number_L  0x02
#define Device_Number_H 0x00
#define Device_Number_L 0x10
#define Soft_Version  0x13
#define Hardware_Version  0x15

#endif
#define Target_Value_State BAT_80_100_STATUS
#define BOOTLOADER_ADDRESS     (uint32_t)0x08000000  //BOOTLOADER_ADDRESS
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
	Comm_Quit_Testmode,
}_Tset_status_TypeDef;
/**************************************************************/

/*******************R1 PRO MAX***************************/
/*
#define Manufacturer_Number_H 0x00
#define Manufacturer_Number_L 0x01
#define Model_Number_H  0x00
#define Model_Number_L  0x02
#define Device_Number_H 0x00
#define Device_Number_L 0x11
#define Soft_Version  0x18
#define Hardware_Version  0x15
*/
/**************************************************************/
#define Test_Pole_Change 100
#define Mode_LED_UP()   {RF_LED_UP();EMS_LED_UP();}
#define Mode_LED_DOWN() {RF_LED_DOWN();EMS_LED_DOWN();}
#define ALL_LED_UP()    {RF_LED_UP();EMS_LED_UP();GEAR_LED1_UP();GEAR_LED2_UP();GEAR_LED3_UP();GEAR_LED4_UP();GEAR_LED5_UP();}
#define ALL_LED_DOWN()  {RF_LED_DOWN();EMS_LED_DOWN();GEAR_LED1_DOWN();GEAR_LED2_DOWN();GEAR_LED3_DOWN();GEAR_LED4_DOWN();GEAR_LED5_DOWN();}

#define LEVEL_LED_UP()    {GEAR_LED1_UP();GEAR_LED2_UP();GEAR_LED3_UP();GEAR_LED4_UP();GEAR_LED5_UP();}
#define LEVEL_LED_DOWN()  {GEAR_LED1_DOWN();GEAR_LED2_DOWN();GEAR_LED3_DOWN();GEAR_LED4_DOWN();GEAR_LED5_DOWN();}



void Test_Key_Proc(void);
void Test_Mode_Key_Process(void);
void Test_Mode_Run_Process(void);
void Test_RF_Waveform_Process(void);
void Test_EMS_Waveform_Process(void);
void Test_UART_Send_Data(void);
void Test_Pole_run(uint8_t Pole_Counts);
void Test_AutoMode_Run_Process(void);
void Test_UART_Deal_Process(void);
uint8_t display_binary(int8_t bin_data)	;
void Ageing_Test_Process(void);
void Ageing_TestData_DeInit(void);
void Ageing_TestData_Init(void);
typedef  void (*pFunction)(void);
void BKP_SRAM_Init(void);
void Comm_GSensor_Control_Process(void);
#endif

