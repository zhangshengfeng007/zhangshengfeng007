#ifndef __UART_SEND_H
#define __UART_SEND_H
#define buffsize 255//8
extern uint8_t recv_data_buff[buffsize];
extern void Uart2_Send_Data(void);
extern void User_Uart_Init(void);
extern void Aging_Test(void);
extern void Sys_Command_Handle(uint8_t *datacom);

//==========================Manufacturer Data==================================//
//ID_code---------------------
#define AMIRO_ID  0x0001
#define XIAOMI_ID 0x0002
//MODEL_code-----------------
#define AML_Series 0x0001
#define ABF_Series 0x0002
//Device ID-------------------
#define R1_PRO_LITE     0x0010
#define S1              0x0001
#define R1_PRO_MAX      0x0011
#define O2_OVERSEAS_VERSION 0x0001
//Soft_Version----------------
#define SOFT_VER        0x23
//Hard_Version----------------
#define HARDWARE_VER    0x60
//Direction ID----------------
#define Direction_ID_Recv 0x00 //pc->产品
#define Direction_ID_Send 0x10 //产品->pc
//Soft_Version 

#define AUTO_TEST_ON		0
#define AUTO_TEST_OFF		0xffff

//command---------------------
#define Comm_Inset_Testmode					0x0001	//进入测试模式
#define Comm_Manufactur_Info				0x0002	//获取制造信息（版本）
#define Comm_Ntc_Temp								0x0003	//获取NTC温度值
#define Comm_Batter_Value						0x0004	//获取电池电压值
#define Comm_Moto_Test							0x0005	//马达连续震动
#define Comm_LED_Test								0x0006	//打开灯板（LED-IR-ON）输出
#define Comm_Electrode_Test					0x0007	//电极头导通性测试
#define Comm_RF_Test								0x0008	//RF输出模式
#define Comm_EMS_Test								0x0009	//EMS输出模式
#define Comm_KEY_Test								0x000A	//按键测试
#define Comm_All_Light							0x000B	//所有灯光测试
#define Comm_G_Sensor_Test					0x000C	//G-SENSOR测试
#define Comm_RGB_Sensor_Calib				0x000D	//RGB-SENSOR校准
#define Comm_RGB_Sensor_Test				0x000E	//RGB-SENSOR测试
#define Comm_Aging_Model						0x000F	//老化模式（最高档位输出到电池没电）
#define Comm_Life_Test							0x0010	//进入寿命测试（插电最高档位输出）
#define Comm_Inset_Bootloader				0x0011	//进入BootLoader（进入BootLoader升级）
#define Comm_Check_Version					0x0012	//版本校验（升级完进行重启、校验版本号）
#define Comm_Inset_Standby					0x0013	//待机模式，测试待机电流 
#define Comm_Quit_AUTOTest					0x0014	//退出测试模式
//#define Comm_Quit_Standby				0x000E	//退出待机模式 

//--------------------------------
#endif




