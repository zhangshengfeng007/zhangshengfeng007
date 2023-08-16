#ifndef __KEY_PROCESS_H
#define __KEY_PROCESS_H

#include "stm32g0xx_hal.h"
#include "Key_Read.h"

void Key_Proc(void);


void Dealwith_power_key_up(void);
void Dealwith_power_key_long(void);
void Dealwith_level_add_key_up(void);
void Dealwith_level_add_key_long(void);
void Dealwith_level_dec_key_up(void);
void Dealwith_level_dec_key_long(void);
void Key_Call_Data(void);


void Dealwith_testmode_power_key_up(void);
void Dealwith_testmode_level_add_key_up(void);
void Dealwith_testmode_level_dec_up(void);
void Dealwith_testmode_power_key_long(void);

#endif

