#ifndef _TOUCH_H
#define _TOUCH_H

#include "system.h"

typedef struct xTouchStatus_t_
{
	uint8_t b_TouchFlag:1;
	uint8_t uc_TouchTimeCnt:7;
}xTouchStatus_t;

extern xTouchStatus_t sx_TouchStatus;

#define		b_TouchFlag_Get()		sx_TouchStatus.b_TouchFlag

void Touch_Handler(void);


#endif
