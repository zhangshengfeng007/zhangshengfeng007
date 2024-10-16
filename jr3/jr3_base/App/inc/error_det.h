#ifndef _ERROR_DET_H
#define _ERROR_DET_H

#include "system.h"

typedef enum 
{
	ERROR_TYPE_NULL			=0x00,
    ERROR_TYPE_FAN			=0x01,
	ERROR_TYPE_VOLTAGE		=0x02,
	ERROR_TYPE_NTC			=0x04,
	ERROR_TYPE_SUPPLY		=0x08,
	ERROR_TYPE_ALL			=0xff,
}error_type_type;

typedef struct xErrorStatus_t_
{
	uint8_t b_errorflashdir:1;			//0灭1亮
	uint8_t b_errortype;
	uint8_t b_errorsupplytestcnt;
	uint16_t uc_errorflashcnt;
}xErrorStatus_t;

extern xErrorStatus_t sx_ErrorStatus;

#define				ErrorType_Set(x)				sx_ErrorStatus.b_errortype |= x
#define				ErrorType_Get()					sx_ErrorStatus.b_errortype

void ErrorType_Reset(uint8_t errortype);
void Error_Det_Handler(void);

#endif
