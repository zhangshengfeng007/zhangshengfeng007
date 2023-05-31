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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "Handle.h"

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
#define VRF9V_CTL_Pin GPIO_PIN_13
#define VRF9V_CTL_GPIO_Port GPIOC
#define MOTOR_ON_Pin GPIO_PIN_14
#define MOTOR_ON_GPIO_Port GPIOC
#define VBAT_DET_ON_Pin GPIO_PIN_15
#define VBAT_DET_ON_GPIO_Port GPIOC
#define VBAT_OUT_ON_Pin GPIO_PIN_0
#define VBAT_OUT_ON_GPIO_Port GPIOF
#define VCC_3V3_OUT_Pin GPIO_PIN_1
#define VCC_3V3_OUT_GPIO_Port GPIOF
#define CHARGE_STAT_Pin GPIO_PIN_0
#define CHARGE_STAT_GPIO_Port GPIOA
#define VEMS_Feedback_Pin GPIO_PIN_1
#define VEMS_Feedback_GPIO_Port GPIOA
#define TEMP_AD_Pin GPIO_PIN_2
#define TEMP_AD_GPIO_Port GPIOA
#define VBAT_DET_Pin GPIO_PIN_3
#define VBAT_DET_GPIO_Port GPIOA
#define Vref_en_Pin GPIO_PIN_4
#define Vref_en_GPIO_Port GPIOA
#define VIN_DET_Pin GPIO_PIN_5
#define VIN_DET_GPIO_Port GPIOA
#define VIN_DET_EXTI_IRQn EXTI4_15_IRQn
#define LED_IR_ON_Pin GPIO_PIN_6
#define LED_IR_ON_GPIO_Port GPIOA
#define RF_LEVEL2_Pin GPIO_PIN_7
#define RF_LEVEL2_GPIO_Port GPIOA
#define RF_LEVEL3_Pin GPIO_PIN_0
#define RF_LEVEL3_GPIO_Port GPIOB

#if ((ARF001 == DEVICE_R1_RPO) || (ARF001 == DEVICE_R1_RPO_MAX))
  #define RF_LEVEL4_Pin       GPIO_PIN_1
  #define RF_LEVEL4_GPIO_Port GPIOB
  #define WLED2_Pin           GPIO_PIN_2
  #define WLED2_GPIO_Port     GPIOB
#elif (ARF001 == DEVICE_R1_HAIWAI)
  #define RF_LEVEL4_Pin       GPIO_PIN_2
  #define RF_LEVEL4_GPIO_Port GPIOB

  #define RF_LEVEL5_Pin       GPIO_PIN_1
  #define RF_LEVEL5_GPIO_Port GPIOB

  #define WLED2_Pin           GPIO_PIN_10
  #define WLED2_GPIO_Port     GPIOB

#endif

#if (G_SENSOR_SELECT == USE_G_SENSOR)
  #define G_SENSOR_SCL_Pin GPIO_PIN_10
  #define G_SENSOR_SCL_GPIO_Port GPIOB
  #define G_SENSOR_SDA_Pin GPIO_PIN_11
  #define G_SENSOR_SDA_GPIO_Port GPIOB
#endif

#if ((ARF001 == DEVICE_R1_RPO) || (ARF001 == DEVICE_R1_RPO_MAX))
  #define RF_Current_Pin        GPIO_PIN_12
  #define RF_Current_GPIO_Port  GPIOB
#elif (ARF001 == DEVICE_R1_HAIWAI)
  #define RF_Current_Pin        GPIO_PIN_11
  #define RF_Current_GPIO_Port  GPIOB

  #define EMS_DET_Pin           GPIO_PIN_12
  #define EMS_DET_GPIO_Port     GPIOB

#endif


#define PLED_PWM_Pin GPIO_PIN_14
#define PLED_PWM_GPIO_Port GPIOB
#define WLED1_Pin GPIO_PIN_15
#define WLED1_GPIO_Port GPIOB
#define CHANNEL_P_Pin GPIO_PIN_8
#define CHANNEL_P_GPIO_Port GPIOA
#define WLED3_Pin GPIO_PIN_6
#define WLED3_GPIO_Port GPIOC
#define WLED4_Pin GPIO_PIN_7
#define WLED4_GPIO_Port GPIOC
#define WLED5_Pin GPIO_PIN_11
#define WLED5_GPIO_Port GPIOA
#define KEY_DOWN_Pin GPIO_PIN_12
#define KEY_DOWN_GPIO_Port GPIOA
#define MBI5020_LE_Pin GPIO_PIN_15
#define MBI5020_LE_GPIO_Port GPIOA
#define KEY_UP_Pin GPIO_PIN_0
#define KEY_UP_GPIO_Port GPIOD
#define KEY_ON_OFF_Pin GPIO_PIN_1
#define KEY_ON_OFF_GPIO_Port GPIOD
#define KEY_ON_OFF_EXTI_IRQn EXTI0_1_IRQn
#define OLED_PWM_Pin GPIO_PIN_2
#define OLED_PWM_GPIO_Port GPIOD
#define BOOST_ON_5V_Pin GPIO_PIN_3
#define BOOST_ON_5V_GPIO_Port GPIOD
#define MBI5020_CLK_Pin GPIO_PIN_3
#define MBI5020_CLK_GPIO_Port GPIOB
#define MBI5020_SDI_Pin GPIO_PIN_4
#define MBI5020_SDI_GPIO_Port GPIOB
#define MBI5020_SDO_Pin GPIO_PIN_5
#define MBI5020_SDO_GPIO_Port GPIOB
#define CHANNEL_N_Pin GPIO_PIN_6
#define CHANNEL_N_GPIO_Port GPIOB
#define OSC_RF_P_Pin GPIO_PIN_7
#define OSC_RF_P_GPIO_Port GPIOB
#define OSC_RF_N_Pin GPIO_PIN_9
#define OSC_RF_N_GPIO_Port GPIOB
/* USER CODE END Prototypes */


/* USER CODE BEGIN Includes */
void MX_GPIO_DISInit(void);
void MX_GPIO_ONT_Init(void);
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void SystemClock_Config(void);


#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

