#ifndef _WORK_H
#define _WORK_H

#include "system.h"

typedef enum
{
	DETECTIVE_INIT_BOOT,
	DETECTIVE_INIT_ON,
	//DETECTIVE_INIT_SUPPLY,
	//DETECTIVE_INIT_DISPLAY,
	DETECTIVE_INIT_FINISH
}detective_init_type;

typedef struct sx_FightLightStatus_t_
{
	//uint8_t b_FightLightFanBusy:1;
	uint8_t b_FightLightTouchFanBusy:1;
	uint8_t b_FightDisplayBusy:1;
	uint8_t b_KeyPressFlag:1;			// 手动打光按键按下标志位
	uint8_t b_ChargeOkFlag:1;			// 充电完成标志位
	uint8_t b_FightLightFlag:1;			// 打光标志
	uint8_t b_IPLWorking:1;				// 正在打光
	uint8_t b_DetInitFlag:3;			// 检测初始化
	uint8_t b_IsPowerOnFlag;
	uint8_t uc_FightRefightCnt;
	//uint8_t sl_FightNum;				//自动打光次数
	uint16_t uc_DetInitCnt;
	uint16_t ul_FightLightCnt:12;		// 打光时间间隔计数
	uint16_t sl_AutolightTime;			//自动打光时间
	uint16_t uc_FightLightTime;
}sx_FightLightStatus_t;

extern sx_FightLightStatus_t sx_FightLightStatus;


#define		b_IsPowerOnFlag_Get()				sx_FightLightStatus.b_IsPowerOnFlag
#define		b_IsPowerOnFlag_Set(x)				sx_FightLightStatus.b_IsPowerOnFlag=x
#define		b_IPLWorking_Get()					sx_FightLightStatus.b_IPLWorking
#define		b_DetInitFlag_Set(x)				sx_FightLightStatus.b_DetInitFlag=x
#define		b_DetInitFlag_Get()					sx_FightLightStatus.b_DetInitFlag
#define		b_ChargeOkFlag_Set(x)				sx_FightLightStatus.b_ChargeOkFlag=x
#define		b_ChargeOkFlag_Get()				sx_FightLightStatus.b_ChargeOkFlag
#define		b_KeyPressFlag_Set(x)				sx_FightLightStatus.b_KeyPressFlag=x
#define		b_KeyPressFlag_Get()				sx_FightLightStatus.b_KeyPressFlag
#define		b_FightLighTouchFanBusy_Set(x)			sx_FightLightStatus.b_FightLightTouchFanBusy=x
#define		b_FightLighTouchFanBusy_Get()			sx_FightLightStatus.b_FightLightTouchFanBusy
#define		b_FightLighFlag_Get()				sx_FightLightStatus.b_FightLightFlag
#define		sl_AutolightTime_Set(x)				sx_FightLightStatus.sl_AutolightTime=x
#define		FightLightReset()					{sx_FightLightStatus.b_FightLightFlag=false;sx_FightLightStatus.ul_FightLightCnt=0;}


void Work_Init(void);
void Work_Handler(void);
void Work_Cb(void);
void Fight_Light_Time_Set(uint8_t num);
uint8_t IsChargeDelayOK(void);


#endif
