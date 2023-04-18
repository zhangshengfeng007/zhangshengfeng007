#ifndef _DEVICE_APP_H
#define _DEVICE_APP_H

#include "common.h"
#include "app_include.h"

typedef enum
{
	LEVEL0 = 0,
	LEVEL1 = 1,
	LEVEL2 = 2,
	LEVEL3 = 3,
}Sys_level_e;

typedef enum
{
	UP_DIR = 0,
	DOWN_DIR = 1,
}Sys_Dir_e;


typedef struct{
	Sys_State_Type State;	//机器状态
	Err_Type Err_State;		//错误状态
	Sys_level_e Level;			// 1-3
	Sys_Dir_e LevelDir;		//档位加减方向
	u16 RF_Time;		//每个输出周期的输出时间
	u16 EMS_Time;		//每个输出周期的输出时间
	u16 OutCount;		//输出计数
	u16 IdleCnt;		//无操作自动关机计数，单位S
	u16 UseCount;		//使用自动关机计数，单位S
	u16 Bat_Lower_3v2_cnt;		//电池电压低于3.2v 计数
	u16 WakeCount;			//唤醒后关机计数，单位S
	u16 Err_Count;			//唤醒后关机计数，单位S
	u8 sysoff_disp_flag;		//切换到 sys off 标志位
}Device_Type;

extern Device_Type Device;

void Device_Init(void);
void Device_Control(Sys_State_Type State);
void Device_Level_Set(u8 data);

void change_to_err_state(void);
void Device_Data_Write(void);
void Device_Data_Read(void);
void Device_Out_Process(void);
void Device_AutoOff_Process(void);
void Normal_mode_Charge_Ctrl(void);
void Test_mode_Charge_Ctrl(void);

#endif




