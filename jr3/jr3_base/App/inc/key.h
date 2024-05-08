#ifndef _KEY_H
#define _KEY_H

#include "system.h"


/************************************keydriver******************************************/
#define CBIT_00		0x01
#define CBIT_01		0x02
#define CBIT_02		0x04
#define CBIT_03		0x08


#define CKCONFIRM_TIMES		DFAULT_CKCONFIRM_TIMES		//出有效键值扫描次数
#define CHOLDKEY_TIME		(1500/10)	//出长按键压制时间(ms)
#define CPRESSKEY_TIME		(800/10)		//出短按键压制时间(ms)
#define CHOLDRPTKEY_TIME	(700/10)		//出长按连续键间隔时间(ms)
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
	uint8_t ucScanStep;			//扫描状态
	uint8_t ulKeyCode;			//扫描键值
	uint8_t ulCodeTemp;			//临时键值
	uint8_t ulKeyCur;			//临时键值
	//uint8_t ulKIOState;			//键盘IO状态备份
	//uint16_t usHoldTime;			//hold出键间隔
	//uint16_t usHoldRPTTime;		//holdrepeat出键间隔
	uint16_t ucScanCnt;			//扫描计数
}type_sKSCtrl;

#define CBit_PressEnable		CBIT_00		//按下即有键值输出
#define CBit_HoldEnable			CBIT_01		//长按有值输出，此时由usHoldTime值决定续键间隔时间
#define CBit_HoldRPTEnable		CBIT_02		//长按有连续键值输出，此时由usHoldRPTTime值决定续键间隔时间
#define CBit_ReleaseEnable		CBIT_03		//放手有键值输出
#define CBit_MultiKeyEnable		CBIT_04		//容许多键按下有键值输出,此时不支持放手键值（！暂不支持！）

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
	uint8_t b_OnOffFlag;			// 开关机
	uint8_t b_AutoSwitchFlag;		// 手自动
	uint8_t b_OldVersionFlag;		// 老化模式
	uint8_t uc_Level;				// 档位
	uint8_t uc_Mode;				// 模式
	uint8_t uc_LevelSum;			// 电压总挡位
	uint16_t ul_OldCnt;				// 时间计数
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
