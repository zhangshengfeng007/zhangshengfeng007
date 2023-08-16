/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for 123
  *          the usart.c file
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
#ifndef _16_USART_APP_H_
#define _16_USART_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */

void Uart_Protocol_Process(void);

void dealwith_KEY_test_power_key_up(void);
void dealwith_KEY_test_level_add_key_up(void);
void dealwith_KEY_test_level_dec_key_up(void);
void G_SENSOR_run_send_data(void);
void G_SENSOR_idle_send_data(void);
void enter_agine_test_init(void);
void enter_inset_life_test_init(void);

void send_ntc_temp(void);
/* USER CODE END Private defines */

void cali_ntc_result(TRUE_FALSE_e res);


#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

