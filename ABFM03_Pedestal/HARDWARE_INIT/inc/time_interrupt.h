/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIME_INTERRUPT_H__
#define __TIME_INTERRUPT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "User_System_Config.h"

extern __IO uint16_t Delay_Cnt;

void MX_TIME_Init(void);
void Timer8_PWM_Init(void);
void Delay_MS(uint16_t time);
	
#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

