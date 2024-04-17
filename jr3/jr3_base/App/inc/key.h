#ifndef _KEY_H
#define _KEY_H

#include "system.h"


/************************************keydriver******************************************/
#define CBIT_00		0x01
#define CBIT_01		0x02
#define CBIT_02		0x04
#define CBIT_03		0x08


#define CKCONFIRM_TIMES		DFAULT_CKCONFIRM_TIMES		//����Ч��ֵɨ�����
#define CHOLDKEY_TIME		(1500/10)	//��������ѹ��ʱ��(ms)
#define CPRESSKEY_TIME		(800/10)		//���̰���ѹ��ʱ��(ms)
#define CHOLDRPTKEY_TIME	(700/10)		//���������������ʱ��(ms)
#define DFAULT_CKCONFIRM_TIMES		(30/10)

//#define OS_TICKS_PER_SEC	1000    		/* Set the number of ticks in one second                        */
//#define CSYSTICK_BASE		(1000/OS_TICKS_PER_SEC)

enum
{
	CKSTEP_NULL,
	CKSTEP_PRESS,
	CKSTEP_HOLD,
	CKSTEP_HOLDRPT,
	CKSTEP_RLS
};

typedef struct tag_strKSCtrl
{
	uint8_t ucScanStep;			//ɨ��״̬
	uint8_t ulKeyCode;			//ɨ���ֵ
	uint8_t ulCodeTemp;			//��ʱ��ֵ
	uint8_t ulKeyCur;			//��ʱ��ֵ
	//uint8_t ulKIOState;			//����IO״̬����
	//uint16_t usHoldTime;			//hold�������
	//uint16_t usHoldRPTTime;		//holdrepeat�������
	uint16_t ucScanCnt;			//ɨ�����
}type_sKSCtrl;

#define CBit_PressEnable		CBIT_00		//���¼��м�ֵ���
#define CBit_HoldEnable			CBIT_01		//������ֵ�������ʱ��usHoldTimeֵ�����������ʱ��
#define CBit_HoldRPTEnable		CBIT_02		//������������ֵ�������ʱ��usHoldRPTTimeֵ�����������ʱ��
#define CBit_ReleaseEnable		CBIT_03		//�����м�ֵ���
#define CBit_MultiKeyEnable		CBIT_04		//�����������м�ֵ���,��ʱ��֧�ַ��ּ�ֵ�����ݲ�֧�֣���

//key press
#define CKEY_S1			CBIT_00

//key hold
#define CKEY_HLD_S1			CBIT_01
//key press release
#define CKEY_RLS_S1			CBIT_02
//key hold release
#define CKEY_HRLS_S1		CBIT_03

#define CKEY_NULL       0xFF


#define IO_KEY		0xff

typedef enum 
{
    KEY_EVENT_NULL,
    KEY_EVENT_PRESS=1,
    KEY_EVENT_LONG,
    KEY_EVENT_RELEASE,
}key_event_type;
/******************************************************************************/


typedef struct xSysStatus_t_
{
	uint8_t b_OnOffFlag;			// ���ػ�
	uint8_t b_AutoSwitchFlag;		// ���Զ�
	uint8_t b_OldVersionFlag;		// �ϻ�ģʽ
	uint8_t uc_Level;				// ��λ
	uint8_t uc_Mode;				// ģʽ
	uint8_t uc_LevelSum;			// ��ѹ�ܵ�λ
	uint16_t ul_OldCnt;				// ʱ�����
}xSysStatus_t;

extern xSysStatus_t sx_SysStatus;

#define		b_OnOffFlag_Get()				sx_SysStatus.b_OnOffFlag
#define		b_AutoSwitchFlag_Set(x)			{sl_AutolightTime_Set(0);sx_SysStatus.b_AutoSwitchFlag=x;}
#define		b_AutoSwitchFlag_Get()			sx_SysStatus.b_AutoSwitchFlag
#define		uc_Mode_Set(x)					sx_SysStatus.uc_Mode=x
#define		uc_Mode_Get()					sx_SysStatus.uc_Mode
#define		uc_Level_Set(x)					sx_SysStatus.uc_Level=x
#define		uc_Level_Get()					sx_SysStatus.uc_Level
#define		uc_LevelSum_Set(x)				{sx_SysStatus.uc_LevelSum=x;Charge_Duty_Set(x);Fight_Light_Time_Set(x);}
#define		uc_LevelSum_Get()				sx_SysStatus.uc_LevelSum


void Key_init(void);
void Key_Scan_Cb(void);
void Key_Handler(void);
void PowerOnOff_Set(uint8_t value);

#endif
