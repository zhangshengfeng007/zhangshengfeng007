#ifndef _SLEEP_H
#define _SLEEP_H

#include "system.h"


typedef enum
{
    SLEEP_MODE_NULL,        //正常操作模式
    SLEEP_MODE_WAKE,        //唤醒模式
    SLEEP_MODE_SLEEP        //待机模式
}sleep_mode_type;

typedef struct xSleepStatus_t_
{
	uint8_t b_SleepMode;
	uint32_t l_MaximumWorkCnt;			//最长工作时间计时
	uint32_t l_NoninductivitCnt;		//无感应操作计时
}xSleepStatus_t;


extern xdata xSleepStatus_t sx_SleepStatus;


#define		b_SleepMode_Get()				sx_SleepStatus.b_SleepMode
#define		b_SleepMode_Set(x)				sx_SleepStatus.b_SleepMode=x
#define		ul_SleepModeCnt_Set(x)			sx_SleepStatus.l_NoninductivitCnt=x

void Sleep_Handler(void);


#endif
