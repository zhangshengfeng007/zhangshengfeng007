#ifndef _DRV_LED_H
#define _DRV_LED_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


void Bat_Led_Init(void);
void Bat_Led_Low_Control(ControlStatus cmd);
void Bat_Led_Low_Breathe(void);
void Bat_Led_Full_Control(ControlStatus cmd);
void Bat_Led_Full_Breathe(void);


#endif

