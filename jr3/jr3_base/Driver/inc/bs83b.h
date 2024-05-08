#ifndef _BS83B_H
#define _BS83B_H

#include "system.h"
//触摸按键驱动
#define		IIC_PA				(0x14)
#define		IIC_PAC				(0x15)
#define		IIC_PB				(0x20)
#define		IIC_PBC				(0x21)				//0:out 1:in


#define		TOUCH_KEY_1			((iic_pa_data&0x01)>>0)
#define		TOUCH_KEY_2			((iic_pa_data&0x02)>>1)
#define		TOUCH_KEY_3			((iic_pa_data&0x04)>>2)
#define		TOUCH_KEY_4			((iic_pa_data&0x08)>>3)
#define		TOUCH_KEY_5			((iic_pa_data&0x10)>>4)
#define		TOUCH_KEY_6			((iic_pa_data&0x20)>>5)
#define		TOUCH_KEY_7			((iic_pa_data&0x40)>>6)

uint8_t IIC_Test(void);
void IIC_Init(void);
void Delay_us(uint8_t us);

#endif
