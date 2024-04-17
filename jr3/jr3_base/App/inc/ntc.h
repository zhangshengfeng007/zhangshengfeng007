#ifndef _NTC_H
#define _NTC_H

#include "system.h"

typedef struct xNTCStatus_t_
{
	//uint8_t ntcbuff[3];
	uint16_t ntcvalue;
}xNTCStatus_t_;



void NTC_Init(void);
void NTC_Handler(void);

#endif
