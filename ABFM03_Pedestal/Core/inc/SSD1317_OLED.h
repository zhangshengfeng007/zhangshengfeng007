#ifndef __SSD1317_OLED_H
#define __SSD1317_OLED_H


#include "User_System_Config.h"
void OLED_Init(void);
void OLED_Data_Process(uint8_t Level,uint8_t Lock,uint8_t Err,uint8_t Connect);
void Display_Level(uint8_t Level);
void OLED_Display(void);
void OLED_Display_Off(void);
#endif

