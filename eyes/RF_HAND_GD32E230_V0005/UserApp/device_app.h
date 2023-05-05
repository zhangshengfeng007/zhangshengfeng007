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
	Sys_State_Type State;	//»úÆ÷×´Ì¬
	Err_Type Err_State;		//´íÎó×´Ì¬
	Sys_level_e Level;			// 1-3
	Sys_Dir_e LevelDir;
	u16 RF_Time;
	u16 EMS_Time;
	u16 OneCycle;

	u16 OutCount;
	u16 IdleCnt;
	u16 UseCount;
	u16 Bat_Lower_3v2_cnt;
	u16 WakeCount;
	u16 Err_Count;
	u8 sysoff_disp_flag;
	u16 charge_run_cnt;
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




