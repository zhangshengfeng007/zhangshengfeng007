#ifndef _IR_LED_H
#define _IR_LED_H

#include "includes.h"

#define IR_LED_TIM   htim3
#define IR_LED_RED   TIM_CHANNEL_1

//#define VBAT_OUT_TIM   htim14
//#define VBAT_OUT     TIM_CHANNEL_1

#define IR_LED_RED_START()     	HAL_TIM_PWM_Start(&IR_LED_TIM,IR_LED_RED)
#define IR_LED_RED_STOP()      	HAL_TIM_PWM_Stop(&IR_LED_TIM,IR_LED_RED)
#define IR_LED_RED_SET(Compare) {__HAL_TIM_SetCompare(&IR_LED_TIM,IR_LED_RED,Compare);}

#define LED_UP_STATS   0x01
#define LED_DOWN_STATS 0x00

//#define VBAT_OUT_START()     	HAL_TIM_PWM_Start(&VBAT_OUT_TIM,VBAT_OUT)
//#define VBAT_OUT_STOP()      	HAL_TIM_PWM_Stop(&VBAT_OUT_TIM,VBAT_OUT)
//#define VBAT_OUT_SET(Compare) {__HAL_TIM_SetCompare(&VBAT_OUT_TIM,VBAT_OUT,Compare);}

#define	BOOST_ON_PORT 			GPIOD
#define BOOST_ON_PIN       		GPIO_PIN_3 

#define BOOST_ON_5V_START() 	HAL_GPIO_WritePin(BOOST_ON_PORT,BOOST_ON_PIN,GPIO_PIN_SET)
#define BOOST_ON_5V_STOP()  	HAL_GPIO_WritePin(BOOST_ON_PORT,BOOST_ON_PIN,GPIO_PIN_RESET)

/////电池电源开关
#define LED_IR_PIN      GPIO_PIN_6
#define LED_IR_PORT		GPIOA

#define LED_IR_ON()     HAL_GPIO_WritePin(LED_IR_PORT,LED_IR_PIN,GPIO_PIN_SET)
#define LED_IR_OFF()    HAL_GPIO_WritePin(LED_IR_PORT,LED_IR_PIN,GPIO_PIN_RESET)

extern void IRled_stop(void);
extern void IRled_start(void);

extern void IRLED_Brighter_Indication(_IRLED_VALUE_TypeDef *IRLED ,uint8_t pwm_step);	
void LedIR_Init(void);
#endif

