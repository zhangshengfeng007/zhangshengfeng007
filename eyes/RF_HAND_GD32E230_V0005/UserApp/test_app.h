#ifndef _TEST_APP_H
#define _TEST_APP_H

#include "common.h"
#include "app_include.h"

typedef enum
{
	None_CMD,
	Inset_Testmode_CMD,	 // 进入或推出测试模式
	Manufacture_Info_CMD,
	Ntc_Temp_CMD,
	Bat_Value_CMD,
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
	Comm_Peer_Set,
	Comm_Peer_Get,
	Comm_Ntc_Set,
	Comm_Oled_Display,
	Comm_Led_Display,
	Comm_Software_Info,
	Comm_Default_Par,
}Test_Comm_Type;



typedef enum
{
	None_STA,
	Inset_Testmode_STA,	 // 进入或推出测试模式
	Manufacture_Info_STA,
	Read_Flume_Temp_STA,  // 读取恒温水槽温度
	Bat_Value_STA,
	Motor_TEST_STA,
	HeadLED_TEST_STA,
	Electrode_Test_STA,
	ONLY_RF_TEST_STA,
	ONLY_EMS_TEST_STA,
	KEY_TEST_STA,
	Light_Ctrl_STA,
	GSensor_Ctrl_STA,
	RGBSensor_Calibra_STA,
	RGBSensor_Ctrl_STA,
	Aging_Model_SAT,
	Inset_Life_Test_STA,
	Inset_Bootloader_STA,
	Check_Version_STA,
	Standby_STA,
	Wireless_communication_STA,
	Peer_Set_SAT,
	Peer_Get_STA,
	Ntc_Set_STA,
	Oled_TEST_STA,
	Led_TEST_STA,
	Software_Info_STA,
	Default_Param_STA,
}Test_STATE_e;

#define AGING_CHARGE_MAX_CNT   1200//1200     // 20分钟
#define AGING_EMS_RF_MAX_CNT   600//600      // 10分钟

#define INSET_LIFE_CHARGE_MAX_CNT   600//1200     // 10 分钟
#define INSET_LIFE_EMS_RF_MAX_CNT   240//600      //  4 分钟


typedef struct{
	Func_Type State;
	Test_STATE_e mode_sta;
	Test_Comm_Type Comm;  //功能
	s16 NTC_TARGET;		  //NTC校准目标值
	u16 Adj_Cnt;		  //校准计数
	int Aging_run_cnt;     // 老化测试运行时长，按 秒计算

	uint8_t EMS_RF_out_flag; // 老化模式，ems_rf输出标志位
	uint8_t Charge_flag; // 老化模式，ems_rf输出标志位
	int Ems_RF_cnt;     // 老化测试射频时长，按 秒计算，运行10min
	int Charge_cnt;     // 老化测试运行时长，按 秒计算，运行20min
	uint8_t Aging_finish_flag; // 老化测试完毕标志位

}Test_Type;

extern Test_Type Test;

void Test_Init(void);
void Test_Order_Process(void);
uint16_t crc16_compute(uint8_t const * p_data, uint32_t size);
void RF_Ntc_Adjust_Process(void);

void Test_Mode_Device_Out_Process(void);
#endif




