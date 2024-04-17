#ifndef _BURNIN_H
#define _BURNIN_H

#include "system.h"

typedef struct xBurninStatus_t_
{
	uint8_t b_BurninFlag:1;				// 老化模式
	uint8_t b_BurninReadyFlag:1;
	uint8_t b_BurninMode:1;				//0:老化模式 1:寿命模式
	uint8_t b_LifeTestMode:1;			//0:打光  1:停止
	uint8_t b_BurninLastMode;
	uint8_t b_BurninStart;				//启停标志
	uint8_t OnTestCnt;					//老化模式进入计数
	uint8_t OnTouchTimes;				//触摸计数
	uint8_t b_BurninCntStart;			//打光开启检测标志
	uint16_t BurninCntStartCnt;			//打光开启检测计数
	uint16_t ul_BurninTestCnt;			//老化计数
	uint32_t uc_LifeTesttimeCnt;		//寿命计数
}xBurninStatus_t;

extern xBurninStatus_t sx_BurninStatus_t;

#define			BurninEntering()				(sx_BurninStatus_t.b_BurninReadyFlag==true)
#define			b_BurninFlag_Get()				sx_BurninStatus_t.b_BurninFlag
#define			b_BurninReadyFlag_Set(x)		sx_BurninStatus_t.b_BurninReadyFlag=x
#define			b_BurninMode_Get()				sx_BurninStatus_t.b_BurninMode
#define			b_BurninMode_Set(x)				sx_BurninStatus_t.b_BurninMode=x
#define			b_BurninLastMode_Get()			sx_BurninStatus_t.b_BurninLastMode
#define			b_BurninLastMode_Set(x)			sx_BurninStatus_t.b_BurninLastMode=x
#define			b_BurninCntStart_Set(x)			sx_BurninStatus_t.b_BurninCntStart=x
#define			b_BurninStart_Get()				sx_BurninStatus_t.b_BurninStart
#define			b_BurninStart_Set(x)			sx_BurninStatus_t.b_BurninStart=x
#define			b_LifeTestMode_Get()			sx_BurninStatus_t.b_LifeTestMode

#define			ul_BurninTestCnt_Get()			sx_BurninStatus_t.ul_BurninTestCnt
#define			ul_BurninTestCnt_Set(x)			sx_BurninStatus_t.ul_BurninTestCnt=x


void Burnin_init(void);
void BurninInit_Cb(void);
void Burnin_Handler(void);

#endif

