#ifndef _LED_APP_H
#define _LED_APP_H

#include "common.h"
#include "app_include.h"

typedef enum
{
    LEVEL_LEDS_OFF = 0,
    LEVEL_LEDS_ON,
}LEVEL_LEDS_e;

typedef struct
{
  u8 trigle_delay_cnt;  /* data */
  u8 trigle_flag;
}LED_DISP_t;


void Level_Leds_Control(LEVEL_LEDS_e cmd);
void Power_Led_Process(void);
void Level_Leds_Force_refresh(void);

#endif
