#ifndef _TOUCHKEY_H
#define _TOUCHKEY_H

#include "system.h"

#define		TBIT1			0x0001
#define		TBIT2			0x0002
#define		TBIT3			0x0004
#define		TBIT4			0x0008
#define		TBIT5			0x0010
#define		TBIT6			0x0020
#define		TBIT7			0x0040
#define		TBIT8			0x0080

#define		THBIT1			0x0100
#define		THBIT2			0x0200
#define		THBIT3			0x0400
#define		THBIT4			0x0800
#define		THBIT5			0x1000
#define		THBIT6			0x2000
#define		THBIT7			0x4000
#define		THBIT8			0x8000

#define		TKPRESSTIME		3
#define		TKHLODTIME		400

enum
{
	TKSTEP_NULL,
	TKSTEP_PRESS,
	TKSTEP_HOLD,
	TKSTEP_HOLDRPT,
	TKSTEP_RLS
};

typedef struct xTKStatus_t_
{
	uint8_t ucStep;
	uint8_t bTKModeScanCnt;
	uint8_t bTKAutoScanCnt;
	uint8_t bTKPressFlag;
//	uint16_t ucScanCnt;
//	uint32_t ucIOState;
//	uint32_t ucTKCur;
	uint32_t ucTKCode;
}xTKStatus_t;


void Touch_Key_Init(void);
void Touch_Key_Handler(void);


#endif
