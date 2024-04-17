#ifndef _ROTARY_H
#define _ROTARY_H

#include "system.h"
//ÐýÅ¥



#define		ROTARY_LEVEL_1				0x01
#define		ROTARY_LEVEL_2				0x02
#define		ROTARY_LEVEL_3				0x03
#define		ROTARY_LEVEL_4				0x04
#define		ROTARY_LEVEL_5				0x05
#define		ROTARY_LEVEL_6				0x06
#define		ROTARY_LEVEL_OFF			0x07

//043
//154
//265
//376
//410
//521
//632

typedef struct xRotaryStatus_t_
{
	uint8_t bRKbuf;
}xRotaryStatus_t;

extern xRotaryStatus_t xRotaryStatus;


void Rotary_Handler(void);
void Rotary_Cb(void);
#endif
