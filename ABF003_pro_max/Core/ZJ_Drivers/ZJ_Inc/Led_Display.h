#ifndef _LED_DISPLAY_H
#define _LED_DISPLAY_H
#include "main.h"
#include "includes.h"
#include "Handle.h"
////LED ��λ��ģʽ��  ����
#define GEAR_LED1_PIN       GPIO_PIN_15 ///��λLED1
#define GEAR_LED1_PORT		GPIOB
#define GEAR_LED2_PIN       GPIO_PIN_2 ///��λLED2
#define GEAR_LED2_PORT		GPIOB
#define GEAR_LED3_PIN       GPIO_PIN_6 ///��λLED3
#define GEAR_LED3_PORT		GPIOC
#define GEAR_LED4_PIN       GPIO_PIN_7 ///��λLED4
#define GEAR_LED4_PORT		GPIOC
#define GEAR_LED5_PIN       GPIO_PIN_11 ///��λLED5
#define GEAR_LED5_PORT		GPIOA


#define RF_LED_PIN          GPIO_PIN_14 ///RFģʽLED
#define RF_LED_PORT			GPIOB
#define EMS_LED_PIN         GPIO_PIN_2 ///EMSģʽLED
#define EMS_LED_PORT		GPIOD


#define GEAR_LED1_UP()    HAL_GPIO_WritePin(GEAR_LED1_PORT,GEAR_LED1_PIN,GPIO_PIN_RESET)
#define GEAR_LED1_DOWN()  HAL_GPIO_WritePin(GEAR_LED1_PORT,GEAR_LED1_PIN,GPIO_PIN_SET)
#define GEAR_LED2_UP()    HAL_GPIO_WritePin(GEAR_LED2_PORT,GEAR_LED2_PIN,GPIO_PIN_RESET)
#define GEAR_LED2_DOWN()  HAL_GPIO_WritePin(GEAR_LED2_PORT,GEAR_LED2_PIN,GPIO_PIN_SET)
#define GEAR_LED3_UP()    HAL_GPIO_WritePin(GEAR_LED3_PORT,GEAR_LED3_PIN,GPIO_PIN_RESET)
#define GEAR_LED3_DOWN()  HAL_GPIO_WritePin(GEAR_LED3_PORT,GEAR_LED3_PIN,GPIO_PIN_SET)
#define GEAR_LED4_UP()    HAL_GPIO_WritePin(GEAR_LED4_PORT,GEAR_LED4_PIN,GPIO_PIN_RESET)
#define GEAR_LED4_DOWN()  HAL_GPIO_WritePin(GEAR_LED4_PORT,GEAR_LED4_PIN,GPIO_PIN_SET)
#define GEAR_LED5_UP()    HAL_GPIO_WritePin(GEAR_LED5_PORT,GEAR_LED5_PIN,GPIO_PIN_RESET)
#define GEAR_LED5_DOWN()  HAL_GPIO_WritePin(GEAR_LED5_PORT,GEAR_LED5_PIN,GPIO_PIN_SET)


#define RF_LED_UP()       HAL_GPIO_WritePin(RF_LED_PORT,RF_LED_PIN,GPIO_PIN_RESET)
#define RF_LED_DOWN()     HAL_GPIO_WritePin(RF_LED_PORT,RF_LED_PIN,GPIO_PIN_SET)
#define EMS_LED_UP()      HAL_GPIO_WritePin(EMS_LED_PORT,EMS_LED_PIN,GPIO_PIN_RESET)
#define EMS_LED_DOWN()    HAL_GPIO_WritePin(EMS_LED_PORT,EMS_LED_PIN,GPIO_PIN_SET)

#define Upkeep_LED_ON()  EMS_LED_UP()
#define Upkeep_LED_OFF() EMS_LED_DOWN()

#define Repair_LED_ON()  RF_LED_UP()
#define Repair_LED_OFF() RF_LED_DOWN()

#define	OLED_ON_BIT		1U<<6
#define	PLED_ON_BIT		1U<<5
#define	WLED1_ON_BIT	1U<<4
#define	WLED2_ON_BIT	1U<<3
#define	WLED3_ON_BIT	1U<<2
#define	WLED4_ON_BIT	1U<<1
#define	WLED5_ON_BIT	1U<<0

extern void led_scan(uint8_t bit_flag);

 void Twink_Display(uint8_t BitFlag,uint16_t stayTime);
 void LedStay_Display(uint8_t BitFlag,uint16_t stayTime);
uint8_t  Led_Display(_LED_VALUE_TypeDef * LED );
uint8_t LED_Flicker(uint16_t stayTime) ;
void Mode_Twink1hz_Display(uint16_t stayTime);
void MODE_LEVEL_1hz_Display(uint8_t BitFlag, uint16_t stayTime);
#endif
