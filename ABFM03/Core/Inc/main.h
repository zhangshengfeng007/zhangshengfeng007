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
  * If no LICENSE file comes with this software, it is provided AS-IS.aaa
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
#define POWER_KEY_PIN         GPIO_PIN_13
#define POWER_KEY_PORT        GPIOC
#define POWER_KEY_IRQn        EXTI4_15_IRQn

#define LEVEL_DEC_KEY_PIN     GPIO_PIN_9
#define LEVEL_DEC_KEY_PORT    GPIOB
#define LEVEL_ADD_KEY_PIN     GPIO_PIN_7
#define LEVEL_ADD_KEY_PORT    GPIOC

#define IR_LED_PIN            GPIO_PIN_14
#define IR_LED_PORT           GPIOC
#define MOTOR_PIN             GPIO_PIN_15
#define MOTOR_PORT            GPIOC

#define VBAT_OUT_EN_PIN       GPIO_PIN_0
#define VBAT_OUT_EN_PORT      GPIOF
#define CHARGE_STAT_Pin       GPIO_PIN_1
#define CHARGE_STAT_Port      GPIOF

#define VEMS_ADC_PIN          GPIO_PIN_0
#define VEMS_ADC_PORT         GPIOA
#define TEMP_ADC_PIN          GPIO_PIN_1
#define TEMP_ADC_PORT         GPIOA
#define VBAT_ADC_PIN          GPIO_PIN_4
#define VBAT_ADC_PORT         GPIOA
#define BOOST_5V_EN_PIN       GPIO_PIN_5
#define BOOST_5V_EN_PORT      GPIOA

#define DC5V_IN_DET_PIN       GPIO_PIN_6
#define DC5V_IN_DET_PORT      GPIOA

#define RGB_CTRL_PIN          GPIO_PIN_7
#define RGB_CTRL_PORT         GPIOA
#define RF_LEVEL4_Pin         GPIO_PIN_0
#define RF_LEVEL4_PORT        GPIOB
#define RF_LEVEL3_Pin         GPIO_PIN_1
#define RF_LEVEL3_PORT        GPIOB
#define RF_LEVEL2_Pin         GPIO_PIN_2
#define RF_LEVEL2_PORT        GPIOB

#define IRQ_2_4G_Pin          GPIO_PIN_10
#define IRQ_2_4G_PORT         GPIOB
#define IRQ_2_4G_IRQn         EXTI4_15_IRQn

#define IRF_ADC_Pin           GPIO_PIN_11
#define IRF_ADC_PORT          GPIOB
#define CSN_2_4G_Pin          GPIO_PIN_12
#define CSN_2_4G_PORT         GPIOB
#define SPI_CLK_2_4G_Pin      GPIO_PIN_13
#define SPI_CLK_2_4G_PORT     GPIOB

#define MISO_2_4G_Pin         GPIO_PIN_14
#define MISO_2_4G_PORT        GPIOB
#define MOSI_2_4G_Pin         GPIO_PIN_15
#define MOSI_2_4G_PORT        GPIOB

#define CE_2_4G_Pin           GPIO_PIN_6
#define CE_2_4G_PORT          GPIOC

#define CHANNEL_P_Pin         GPIO_PIN_8
#define CHANNEL_P_PORT        GPIOA

#define G_SENSOR_SCL_Pin      GPIO_PIN_9
#define G_SENSOR_SCL_PORT     GPIOA
#define G_SENSOR_SDA_Pin      GPIO_PIN_10
#define G_SENSOR_SDA_PORT     GPIOA

#define EMS_SCL_Pin           GPIO_PIN_11 // EMS 电压通过 电压调节芯片控制，和主控IIC 通讯
#define EMS_SCL_PORT          GPIOA
#define EMS_SDA_Pin           GPIO_PIN_12
#define EMS_SDA_PORT          GPIOA

#define RF_CH1_N_Pin          GPIO_PIN_15
#define RF_CH1_N_PORT         GPIOA
#define RF_CH1_P_Pin          GPIO_PIN_3
#define RF_CH1_P_PORT         GPIOD

#define VRF_EN_Pin            GPIO_PIN_0   // RF电压档位控制芯片，使能引脚
#define VRF_EN_PORT           GPIOD
#define OSC_RF_N_Pin          GPIO_PIN_1
#define OSC_RF_N_PORT         GPIOD

///ϵͳ3.3V���翪��
#define BOOST_3V3_ON_PIN      GPIO_PIN_3
#define BOOST_3V3_ON_PORT	    GPIOB

#define EMS_CH1_N_Pin         GPIO_PIN_4
#define EMS_CH1_N_PORT        GPIOB
#define EMS_CH1_P_Pin         GPIO_PIN_5
#define EMS_CH1_P_PORT        GPIOB
#define CHANNEL_N_Pin         GPIO_PIN_6
#define CHANNEL_N_PORT        GPIOB
#define OSC_RF_P_Pin          GPIO_PIN_7
#define OSC_RF_P_PORT         GPIOB
#define EMS_PWR_EN_Pin        GPIO_PIN_8 // EMS电压升压到31v，使能引脚
#define EMS_PWR_EN_PORT       GPIOB


#define UART_TX_Pin           GPIO_PIN_9
#define UART_TX_Port          GPIOA
#define UART_RX_Pin           GPIO_PIN_10
#define UART_RX_Port          GPIOA

#define SW_DIO_Pin            GPIO_PIN_13
#define SW_DIO_Port           GPIOA

#define SW_CLK_Pin            GPIO_PIN_14
#define SW_CLK_Port           GPIOA

/* USER CODE BEGIN Private defines */
extern void SystemClock_Config(void);
extern void sys_swd_clk_init(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
