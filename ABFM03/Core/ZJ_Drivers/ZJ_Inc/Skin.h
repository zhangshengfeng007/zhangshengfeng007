#ifndef _SKIN_H
#define _SKIN_H
#include "includes.h"

#define RF_TOUCH_THRESHOLD      85//45//85  ///�Ӵ���� ��ֵ 123
#define TEST_RF_TOUCH_THRESHOLD 85  ///�Ӵ���� ����ģʽ
//#if ARF001
//#define Skin_Remove_Cnt 30
//#else
#define Skin_Remove_Cnt 5
#define Leave_Skin_Timer  12000

//#endif
void Skin_Touch_Scan(void);
void  Skin_RF_Scan(void)	;
uint8_t  Check_Idle_Time(void) ;
#endif


