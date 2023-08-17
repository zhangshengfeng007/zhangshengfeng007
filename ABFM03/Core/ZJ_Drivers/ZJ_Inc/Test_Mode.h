#ifndef _TEST_MODE_H
#define _TEST_MODE_H
#include "includes.h"

#if ARF001
		////////////////���������汾��(ע������2�׺�Ҫͬʱ����)///////////////123
	#define version  "V0.1.1.0"    // 2023 0817 根据试产bom 将版本号更新为V0.1.1.0

	#define EXTERNAL_VERSION_0	0
	#define EXTERNAL_VERSION_1	1
	#define EXTERNAL_VERSION_2	1   // 2023 0817 根据试产bom 将版本号更新为V0.1.1.0
	#define EXTERNAL_VERSION_3	0
////////////////////////////////////////////////////////////////////
	#define inside_version  "V0.1.1.0" // 2023 0817 根据试产bom 将版本号更新为V0.1.1.0
//�ڲ��汾��,����0001�Ծ͵ľ���V0.0.01
	// #define INTERNAL_VERSION_0	0
	// #define INTERNAL_VERSION_1	1  // 2023 03 14 �޸İ汾Ϊv 0.1.2.0
	// #define INTERNAL_VERSION_2	2  //  2023 03 20 �޸İ汾λv 0.1.2.1
	// #define INTERNAL_VERSION_3	4  // 2023 03 29 �޸İ汾λv 0.1.2.4

   	#define INTERNAL_VERSION_0	0
	#define INTERNAL_VERSION_1	1  //
	#define INTERNAL_VERSION_2	1  // 2023 0817 根据试产bom 将版本号更新为V0.1.1.0
	#define INTERNAL_VERSION_3	0  //
#else
	#define version  "V1.2.0"
#endif

#define AutoTest 1
/*******************R1 PRO �»Դｵ��***************************/
#if ARF001
#define Manufacturer_Number_H 0x00
#define Manufacturer_Number_L 0x01
#define Model_Number_H  0x00
#define Model_Number_L  0x02
#define Device_Number_H 0x00
#define Device_Number_L 0x10
#define Soft_Version  0x00
#define Hardware_Version  0x21
#endif

#define Target_Value_State 		BAT_80_100_STATUS
#define BOOTLOADER_ADDRESS     (uint32_t)0x08000000  //BOOTLOADER_ADDRESS
typedef enum
{
	Comm_NULL,             // 0
	Comm_Inset_Testmode,   // 1  进入/退出 测试模式，
	Comm_manufacture_Info, // 2
	Comm_Ntc_Temp,         // 3
	Comm_Batter_Value,     // 4
	Comm_Moto_Control,     // 5

	Comm_HeadLED_Control,  // 6  进入/关闭灯板测试--和第 11条指令 2选1 (旧版本硬件)
	Comm_Electrode_Test,   // 7  电极头导通测试
	Comm_RF_Control,       // 8   RF测试
	Comm_EMS_Control,       // 9  EMS测试
	Comm_KEY_Control,      // 10  按键测试

	Comm_Light_Control,    // 11  测试所有灯光--和第 6条指令 2选1（新版本硬件）
	Comm_GSensor_Control,  // 12  进入/退出 g_sensor测试
	Comm_RGBSensor_Calibra, // 13  颜色传感器_校准
	Comm_RGBSensor_Control, // 14  颜色传感器_测试
	Comm_Aging_Model,       // 15  老化测试

	Comm_Inset_Life_Testl,  // 16  寿命测试
	Comm_Inset_Bootloader,  // 17
	Comm_Check_Version,     // 18
	Comm_Standby,           // 19  测试待机电流（后续待机电流会上传到上位机软件）
	Comm_Wireless_test,     // 20
	Set_Peer_Code_Cmd,      // 21
	Get_Peer_Code_Cmd,      // 22
	//comm_factory_reset,     // 23
	ADJUST_NTC_TEMP_Cmd,

    // comm_bat_low_touch,     // 24
	// comm_ntc_overtemp_touch,// 25
	// comm_ntc_fault_touch,   // 26
	comm_soft_version = 26,      // 27
	comm_reset_param = 27,      // 27  档位 紧致提拉模式1档
//	comm_normal_work_get_ntc,
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
void Key_Enter_Test_Mode_Check(void);
void Key_ctrl_test_mode_func(void);
// void Test_Mode_Run_Process(void);
void Test_RF_Waveform_Process(void);
void Test_EMS_Waveform_Process(void);
// void Test_UART_Send_Data(void);
void Test_Pole_run(uint8_t Pole_Counts);
// void Test_AutoMode_Run_Process(void);
void Test_UART_Deal_Process(void);
uint8_t display_binary(int8_t bin_data)	;
void Ageing_Test_Process(void);
void Ageing_TestData_DeInit(void);
void Ageing_TestData_Init(void);
typedef  void (*pFunction)(void);
void BKP_SRAM_Init(void);
void Test_Control_ModeRun(void);

// void Test_LED_Display_Process(void);
// uint8_t Test_Level_LED_Display(void);
// uint8_t Test_State_LED_Display(void);
void NTC_Temp_SendData(void);
void Ageing_Test_Ctrl(void);
void Test_USB_Plug_Scan(void);
void Agine_mode_output_ctrl(void);
void Agine_mode_charge_ctrl(void);
void enter_xiufu_rf_test(void);

void Test_mode_ctrl_Process(void);
void Key_2_4G_auto_match_mode_Check(void);
#endif

