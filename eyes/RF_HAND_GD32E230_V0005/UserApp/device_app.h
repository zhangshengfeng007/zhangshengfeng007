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
	Sys_State_Type State;	//����״̬
	Err_Type Err_State;		//����״̬
	Sys_level_e Level;			// 1-3
	Sys_Dir_e LevelDir;		//��λ�Ӽ�����
	u16 RF_Time;		//ÿ��������ڵ����ʱ��
	u16 EMS_Time;		//ÿ��������ڵ����ʱ��
	u16 OutCount;		//�������
	u16 IdleCnt;		//�޲����Զ��ػ���������λS
	u16 UseCount;		//ʹ���Զ��ػ���������λS
	u16 Bat_Lower_3v2_cnt;		//��ص�ѹ����3.2v ����
	u16 WakeCount;			//���Ѻ�ػ���������λS
	u16 Err_Count;			//���Ѻ�ػ���������λS
	u8 sysoff_disp_flag;		//�л��� sys off ��־λ
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



