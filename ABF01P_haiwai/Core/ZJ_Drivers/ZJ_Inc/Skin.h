#ifndef _SKIN_H
#define _SKIN_H
#include "includes.h"

#define RF_TOUCH_THRESHOLD      35  ///Ω”¥•ºÏ≤‚ ∑•÷µ 
#define TEST_RF_TOUCH_THRESHOLD 25  ///Ω”¥•ºÏ≤‚ ≤‚ ‘ƒ£ Ω
//#if ARF001
//#define Skin_Remove_Cnt 30
//#else
#define Skin_Remove_Cnt 5
//#endif
void Skin_Touch_Scan(void);
uint8_t  Skin_RF_Scan(void)	;
uint8_t  Cosmetic_Time_Count(void) ;
#endif


