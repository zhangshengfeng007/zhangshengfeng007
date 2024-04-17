#ifndef _BURNIN_H
#define _BURNIN_H

#include "system.h"

typedef struct xBurninStatus_t_
{
	uint8_t b_BurninFlag:1;				// �ϻ�ģʽ
	uint8_t b_BurninReadyFlag:1;
	uint8_t b_BurninMode:1;				//0:�ϻ�ģʽ 1:����ģʽ
	uint8_t b_LifeTestMode:1;			//0:���  1:ֹͣ
	uint8_t b_BurninLastMode;
	uint8_t b_BurninStart;				//��ͣ��־
	uint8_t OnTestCnt;					//�ϻ�ģʽ�������
	uint8_t OnTouchTimes;				//��������
	uint8_t b_BurninCntStart;			//��⿪������־
	uint16_t BurninCntStartCnt;			//��⿪��������
	uint16_t ul_BurninTestCnt;			//�ϻ�����
	uint32_t uc_LifeTesttimeCnt;		//��������
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

