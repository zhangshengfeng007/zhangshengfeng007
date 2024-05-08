#ifndef _74HC595_H
#define _74HC595_H

#include "system.h"
//LED驱动

//enum LEDMODETYPE{	LED_OFF			=0x00,
//					LED_MODE_6		=0x40,
//					LED_MODE_5		=0x10,
//					LED_MODE_4		=0x08, 
//					LED_MODE_3		=0x04, 
//					LED_MODE_2		=0x02, 
//					LED_MODE_1		=0x01,
//					LED_MODE		=LED_MODE_1+LED_MODE_2+LED_MODE_3+LED_MODE_4+LED_MODE_5+LED_MODE_6,
//					LED_AUTO		=0x20,
//					//LED_SENCE		=0x40,
//					LED_ALL			=0xff};
enum LEDMODETYPE{	LED_OFF			=0x00,
					LED_MODE_6		=0x80,
					LED_MODE_5		=0x20,
					LED_MODE_4		=0x10, 
					LED_MODE_3		=0x08, 
					LED_MODE_2		=0x04, 
					LED_MODE_1		=0x02,
					LED_MODE		=LED_MODE_1+LED_MODE_2+LED_MODE_3+LED_MODE_4+LED_MODE_5+LED_MODE_6,
					LED_AUTO		=0x40,
					//LED_SENCE		=0x40,
					LED_ALL			=0xff};

					
void SMG_Write_Byte(uint8_t dat);


#endif
