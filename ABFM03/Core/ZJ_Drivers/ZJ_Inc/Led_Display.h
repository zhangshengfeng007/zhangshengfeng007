#ifndef _LED_DISPLAY_H
#define _LED_DISPLAY_H
#include "main.h"
#include "includes.h"
#include "Handle.h"
//222
#define LED_UP_STATS   0x01
#define LED_DOWN_STATS 0x00

#define LED_IR_ON()     HAL_GPIO_WritePin(IR_LED_PORT, IR_LED_PIN, GPIO_PIN_SET)
#define LED_IR_OFF()    HAL_GPIO_WritePin(IR_LED_PORT, IR_LED_PIN, GPIO_PIN_RESET)

#define CodeOne         0x7f
#define CodeZero        0x70

extern void IRled_stop(void);
extern void IRled_start(void);

// 档位灯枚举 123
typedef enum
{
    LEVEL_NULL_DISP = 0,
    LEVEL1_DISP,
    LEVEL2_DISP,
    LEVEL3_DISP,
    LEVEL4_DISP,
    Mode_LED_DISP = 4,
    LEVEL5_DISP,
    STATUS_LED_DISP = 5,
}LEVEL_LED_DISP_e;

typedef enum
{
    NO_ERR = 0,
    E1_Bat_Low,
    E2_Temp_HIGHER_45,
    E3_NTC_ERROR,
}ERROR_DISP_e;

typedef enum
{
    White = 0,
    Orange,
    Purple,
    LightPink,
    Cyan,
    RED,
    GREEN,
    BLUE,
    BLACK =0xf5,
}DISP_COLOR_e;


#define Close_R  0
#define Close_G  0
#define Close_B  0

#define White_R  255
#define White_G  255
#define White_B  255


extern uint8_t Led_Colour[15];//G R B
extern uint8_t LCD_data[120];

void Level_led_scan(uint8_t cur_level);
void Mode_led_scan(uint8_t State);
void State_led_scan(uint8_t State);
void Cur_Level_Twink_Display(uint8_t BitFlag,uint16_t stayTime);
// void LedStay_Display(uint8_t BitFlag,uint16_t stayTime);
uint8_t  Led_Display(_LED_VALUE_TypeDef * LED );
void Bat_low_led_Filcker(uint16_t stayTime) ;
void Twink1hz_Display(uint8_t disp_level, uint16_t stayTime, uint8_t state_led_inverse_flag);

void LCD_Refresh(void);
void RGB_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
void RGB_Init(void);
void Set_Color_Handle(uint8_t R,uint8_t G,uint8_t B,uint8_t num);
void Normal_Led_Display(uint8_t Level);
#endif
