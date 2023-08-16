#ifndef __KEY_READ_H
#define __KEY_READ_H

#include "stm32g0xx_hal.h"
#include "gpio.h"
#include "includes.h"

// 111
#define Key_Scap_Read_In        HAL_GPIO_ReadPin(POWER_KEY_PORT, POWER_KEY_PIN)
#define Key_Plus_Read_In        HAL_GPIO_ReadPin(LEVEL_ADD_KEY_PORT, LEVEL_ADD_KEY_PIN)
#define Key_minus_Read_In       HAL_GPIO_ReadPin(LEVEL_DEC_KEY_PORT, LEVEL_DEC_KEY_PIN)

#define IS_POWER_KEY_PRESS()        (GPIO_PIN_RESET == HAL_GPIO_ReadPin(POWER_KEY_PORT, POWER_KEY_PIN))
#define IS_LEVEL_ADD_KEY_PRESS()    (GPIO_PIN_RESET == HAL_GPIO_ReadPin(LEVEL_ADD_KEY_PORT, LEVEL_ADD_KEY_PIN))
#define IS_LEVEL_DEC_KEY_PRESS()    (GPIO_PIN_RESET == HAL_GPIO_ReadPin(LEVEL_DEC_KEY_PORT, LEVEL_DEC_KEY_PIN))

#define IS_POWER_KEY_UP()        (GPIO_PIN_SET == HAL_GPIO_ReadPin(POWER_KEY_PORT, POWER_KEY_PIN))
#define IS_LEVEL_ADD_KEY_UP()    (GPIO_PIN_SET == HAL_GPIO_ReadPin(LEVEL_ADD_KEY_PORT, LEVEL_ADD_KEY_PIN))
#define IS_LEVEL_DEC_KEY_UP()    (GPIO_PIN_SET == HAL_GPIO_ReadPin(LEVEL_DEC_KEY_PORT, LEVEL_DEC_KEY_PIN))


typedef enum
{
  KEY_None,
  POWER_KEY_Press,
  LEVEL_ADD_KEY_Press,
  LEVEL_DEC_KEY_Press,
  POWER_KEY_LONG,
  LEVEL_ADD_KEY_LONG,
  LEVEL_DEC_KEY_LONG,
//  KEY_TEST
}_Key_Input;


#endif

