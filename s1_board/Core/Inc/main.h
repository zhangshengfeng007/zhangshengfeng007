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
#define POW_KEY_Pin GPIO_PIN_13
#define POW_KEY_GPIO_Port GPIOC
#define LEVEL_KEY_Pin GPIO_PIN_14
#define LEVEL_KEY_GPIO_Port GPIOC
#define CHARGE_STATE_Pin GPIO_PIN_15
#define CHARGE_STATE_GPIO_Port GPIOC

#define EMS_POW_PWM_Pin GPIO_PIN_0
#define EMS_POW_PWM_GPIO_Port GPIOF

#define LOAD_BUS_SW_Pin GPIO_PIN_1
#define LOAD_BUS_SW_GPIO_Port GPIOF




#define BATTER_ADC_IN_Pin GPIO_PIN_0
#define BATTER_ADC_IN_GPIO_Port GPIOA
#define USB_BUS_ADC_IN_Pin GPIO_PIN_1
#define USB_BUS_ADC_IN_GPIO_Port GPIOA


#define CHARGE_EN_Pin GPIO_PIN_4
#define CHARGE_EN_GPIO_Port GPIOA
#define BACKLIGHT_EN_Pin GPIO_PIN_5
#define BACKLIGHT_EN_GPIO_Port GPIOA
#define USB_BUS_SW_Pin GPIO_PIN_6
#define USB_BUS_SW_GPIO_Port GPIOA

#define EMS_PWR_ADC_IN_Pin GPIO_PIN_7
#define EMS_PWR_ADC_IN_GPIO_Port GPIOA
#define RF_PWR_ADC_IN_Pin GPIO_PIN_0
#define RF_PWR_ADC_IN_GPIO_Port GPIOB
#define RF_IBUS_ADC_IN_Pin GPIO_PIN_1
#define RF_IBUS_ADC_IN_GPIO_Port GPIOB
#define RF_TEMP_ADC_IN_Pin GPIO_PIN_2
#define RF_TEMP_ADC_IN_GPIO_Port GPIOB
#define BAT_TEMP_ADC_IN_Pin GPIO_PIN_10
#define BAT_TEMP_ADC_IN_GPIO_Port GPIOB
#define RF_OUT_SW_Pin GPIO_PIN_11
#define RF_OUT_SW_GPIO_Port GPIOB
#define EMS_OUT_SW_Pin GPIO_PIN_12
#define EMS_OUT_SW_GPIO_Port GPIOB
#define SYS_BUS_SW_Pin GPIO_PIN_13
#define SYS_BUS_SW_GPIO_Port GPIOB
#define C_SENSOR_LED_Pin GPIO_PIN_14
#define C_SENSOR_LED_GPIO_Port GPIOB
#define RGB_DRIVER_Pin GPIO_PIN_15
#define RGB_DRIVER_GPIO_Port GPIOB
#define EMS_OUT_N_Pin GPIO_PIN_8
#define EMS_OUT_N_GPIO_Port GPIOA
#define G_SENSOR_SCL_Pin GPIO_PIN_9
#define G_SENSOR_SCL_GPIO_Port GPIOA
#define B_LIGHT_LV_Pin GPIO_PIN_6
#define B_LIGHT_LV_GPIO_Port GPIOC
#define SENSOR_PWR_SW_Pin GPIO_PIN_7
#define SENSOR_PWR_SW_GPIO_Port GPIOC
#define G_SENSER_SDA_Pin GPIO_PIN_10
#define G_SENSER_SDA_GPIO_Port GPIOA
#define C_SENSOR_SCL_Pin GPIO_PIN_11
#define C_SENSOR_SCL_GPIO_Port GPIOA
#define C_SENSOR_SDA_Pin GPIO_PIN_12
#define C_SENSOR_SDA_GPIO_Port GPIOA
#define USB_INPUT_CHECK_Pin GPIO_PIN_15
#define USB_INPUT_CHECK_GPIO_Port GPIOA
#define RF_PWR_PWM_Pin GPIO_PIN_0
#define RF_PWR_PWM_GPIO_Port GPIOD
#define MOTOR_EN_Pin GPIO_PIN_1
#define MOTOR_EN_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOD
#define RF_IBUS_INT_Pin GPIO_PIN_3
#define RF_IBUS_INT_GPIO_Port GPIOD
#define CHARGE_ILMIT_Pin GPIO_PIN_3
#define CHARGE_ILMIT_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOB
#define EMS_OUT_P_Pin GPIO_PIN_6
#define EMS_OUT_P_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define OVP_CTRL_Pin GPIO_PIN_8
#define OVP_CTRL_GPIO_Port GPIOB
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
