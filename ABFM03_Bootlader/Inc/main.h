/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_ON_OFF_Pin        LL_GPIO_PIN_13
#define KEY_ON_OFF_GPIO_Port  GPIOC

#define BOOST_5V_Pin          LL_GPIO_PIN_5
#define BOOST_5V_GPIO_Port    GPIOA

#define KEY_UP_Pin            LL_GPIO_PIN_7
#define KEY_UP_GPIO_Port      GPIOC

#define KEY_DOWN_PIN          LL_GPIO_PIN_9
#define KEY_DOWN_PORT         GPIOB
/* USER CODE BEGIN Private defines */
// 档位灯枚举
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
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
