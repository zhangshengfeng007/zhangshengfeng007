#ifndef _SLEEP_H
#define _SLEEP_H

#include "system.h"


typedef enum
{
    SLEEP_MODE_NULL,        //��������ģʽ
    SLEEP_MODE_WAKE,        //����ģʽ
    SLEEP_MODE_SLEEP        //����ģʽ
}sleep_mode_type;

typedef struct xSleepStatus_t_
{
	uint8_t b_SleepMode;
	uint32_t l_MaximumWorkCnt;			//�����ʱ���ʱ
	uint32_t l_NoninductivitCnt;		//�޸�Ӧ������ʱ
}xSleepStatus_t;


extern xdata xSleepStatus_t sx_SleepStatus;


#define		b_SleepMode_Get()				sx_SleepStatus.b_SleepMode
#define		b_SleepMode_Set(x)				sx_SleepStatus.b_SleepMode=x
#define		ul_SleepModeCnt_Set(x)			sx_SleepStatus.l_NoninductivitCnt=x

void Sleep_Handler(void);


#endif
