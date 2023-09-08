/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for aaa
  *          the i2c.c file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

#define I2C_SCL_PIN     EMS_SCL_Pin
#define I2C_SCL_PORT    EMS_SCL_PORT
#define I2C_SCL_H()     (HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_SET))
#define I2C_SCL_L()     (HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_RESET))

#define I2C_SDA_PIN     EMS_SDA_Pin
#define I2C_SDA_PORT    EMS_SDA_PORT
#define I2C_SDA_H()     (HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET))
#define I2C_SDA_L()     (HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_RESET))


#define IS_SDA_H()     (GPIO_PIN_SET == HAL_GPIO_ReadPin(I2C_SDA_PORT, I2C_SDA_PIN))
#define IS_SDA_L()     (GPIO_PIN_RESET == HAL_GPIO_ReadPin(I2C_SDA_PORT, I2C_SDA_PIN))

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

extern I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */

void gpio_iic_send_bytes(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
void gpio_iic_read_bytes(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
void only_send_one_byte(uint8_t data);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

