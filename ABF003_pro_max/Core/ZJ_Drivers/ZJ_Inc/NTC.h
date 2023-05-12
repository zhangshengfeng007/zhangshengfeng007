#ifndef _NTC_H
#define _NTC_H
#include "includes.h"

#define NORMAL_TEMP 	40
#define OVER_TEMP 		42
#define STOP_TEMP 		46//44

#define TEMP_TAB_INDEX  15
#define NTC_CHANNEL 1
extern uint8_t ntc_adc_enable_flag;
extern int8_t system_temp;	
extern uint8_t over_temp_flag;

int8_t NTC_Temp_Check(void);
extern void NTC_Temp_Running(void);	

#endif

