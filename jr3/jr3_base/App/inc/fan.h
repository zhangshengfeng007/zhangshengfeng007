#ifndef _FAN_H
#define _FAN_H

#include "system.h"



enum FANONOFFTYPE{FAN_BOOT, FAN_OFF, FAN_ON};

typedef struct xFanStatus_t_
{
	uint8_t b_RotateErrorFlag:1;		/* 风扇异常 */
	uint8_t b_halfspeedflag;			/* 半速標志 */
	uint8_t b_RotateKeepworkFlag:3;		/* 关机持续工作標志 */
	uint8_t uc_RotateNormalTimes;
	uint8_t uc_RotateAbnormalTimes;
	uint8_t uc_half_worktime_count;
	uint16_t ul_RotateKeepworkCnt;		/* 关机持续工作时间计数 */
	uint16_t ul_fanadcvalue;
	uint16_t ul_fanadcbuff;		//更改
	
}xFanStatus_t_;

extern xdata xFanStatus_t_ xFanStatus;

#define		FAN_ON()					do{xFanStatus.b_halfspeedflag=0;FAN=1;/*Pwm_Close_Fan();P2M|=0x20;FAN=1;*/}while(0)
#define		FAN_OFF()					do{xFanStatus.b_halfspeedflag=0;FAN=0;/*{Pwm_Close_Fan();P2M|=0x20;FAN=0;*/}while(0)
#define		FAN_HALFSPEED()				do{xFanStatus.uc_half_worktime_count = 0;xFanStatus.b_halfspeedflag=1;FAN=0;/*P2M&=~0x20;Pwm_Set_Y(6000,3000);Pwm_Open_Fan();*/}while(0)

#define		uc_FanKeepFlagSet(x)				xFanStatus.b_RotateKeepworkFlag=x
#define		b_FanRotateErrorFlag_Set(x)			xFanStatus.b_RotateErrorFlag=x
#define		b_FanRotateErrorFlag_Get()			xFanStatus.b_RotateErrorFlag
#define		b_FanWorkingSet(x)					xFanStatus.b_RotateWorking=x


void Fan_Init(void);
void Fan_Handler(void);
void FanWork_Cb(void);

#endif
