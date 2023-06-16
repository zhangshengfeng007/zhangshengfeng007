/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "Montor_Control.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void ONLY_MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, VRF9V_CTL_Pin|MOTOR_ON_Pin|VBAT_DET_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, VBAT_OUT_ON_Pin|VCC_3V3_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Vref_en_Pin|MBI5020_LE_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(BOOST_ON_5V_GPIO_Port, BOOST_ON_5V_Pin, GPIO_PIN_RESET);

//------PROT A
  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = CHARGE_STAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   GPIO_InitStruct.Pin = KEY_DOWN_Pin;   // KEY ---PULL UP r
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = Vref_en_Pin|RF_LEVEL2_Pin|WLED5_Pin|MBI5020_LE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = VIN_DET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VIN_DET_GPIO_Port, &GPIO_InitStruct);

//--------PORT B
  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin */
  GPIO_InitStruct.Pin = RF_LEVEL3_Pin|RF_LEVEL4_Pin|WLED2_Pin|PLED_PWM_Pin
                          |WLED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#if (ARF001 == DEVICE_R1_HAIWAI) // 海外版，特殊IO口处理
  GPIO_InitStruct.Pin = RF_LEVEL5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RF_LEVEL5_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = EMS_DET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EMS_DET_GPIO_Port, &GPIO_InitStruct);
#endif

// ----------PORT C
  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin */
  GPIO_InitStruct.Pin = VRF9V_CTL_Pin|MOTOR_ON_Pin|VBAT_DET_ON_Pin|WLED3_Pin
                          |WLED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//---------PORT D
  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = KEY_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_UP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = KEY_ON_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_ON_OFF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin */
  GPIO_InitStruct.Pin = OLED_PWM_Pin|BOOST_ON_5V_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

// ---------PORT F
  /*Configure GPIO pins : PFPin PFPin */
  GPIO_InitStruct.Pin = VBAT_OUT_ON_Pin|VCC_3V3_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

 // close gpio
  Upkeep_LED_OFF();
	Repair_LED_OFF();

  GEAR_LED1_DOWN();
  GEAR_LED2_DOWN();
  GEAR_LED3_DOWN();
  GEAR_LED4_DOWN();
  GEAR_LED5_DOWN();

  GEAR2_DOWN();
  GEAR3_DOWN();
  GEAR4_DOWN();

#if (ARF001 == DEVICE_R1_HAIWAI)
  GEAR5_DOWN();
#endif
}


/* USER CODE BEGIN 2 */
/***************************************
*  enter sleep 时，关闭IO 口
**************************************/
void MX_close_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

                  // 未关闭串口 和 swd，clk 时，待机功耗约为 100uA
  HAL_GPIO_DeInit(GPIOA, UART_TX_Pin | UART_RX_Pin | SW_DIO_Pin | SW_CLK_Pin);

#if (G_SENSOR_SELECT == USE_G_SENSOR)   // IIC 引脚，进入低功耗时，配置为输入引脚
	GPIO_InitStruct.Pin =GPIO_PIN_10| GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif

	GEAR_LED1_DOWN();  // 档位灯关闭
	GEAR_LED2_DOWN();
	GEAR_LED3_DOWN();
	GEAR_LED4_DOWN();
	GEAR_LED5_DOWN();

	GEAR2_DOWN(); // 档位控制 io口关闭
	GEAR3_DOWN();
	GEAR4_DOWN();
#if(ARF001 == DEVICE_R1_HAIWAI)
	GEAR5_DOWN();
#endif

  BOOST_5V_OFF();
  RF_CHANNEL_OFF();
  EMS_CHANNEL_OFF();
	VCC3V3_OUT_OFF();
	Vref_EN_OFF();
	BAT_ADC_DISABLE();
	MOTOR_OFF();
	RF_EN_DOWN();
	VBAT_OUT_OFF();
}
/*************************************/
void MX_GPIO_ONT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, VRF9V_CTL_Pin | MOTOR_ON_Pin | VBAT_DET_ON_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOF, VBAT_OUT_ON_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, Vref_en_Pin | RF_LEVEL2_Pin| MBI5020_LE_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, RF_LEVEL3_Pin | RF_LEVEL5_Pin | GPIO_PIN_13 | PLED_PWM_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, BOOST_ON_5V_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PC13 PC14 PC15 PC6
							 PC7 */
	GPIO_InitStruct.Pin = VRF9V_CTL_Pin | MOTOR_ON_Pin | VBAT_DET_ON_Pin | WLED3_Pin | WLED4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PF0 */
	GPIO_InitStruct.Pin = VBAT_OUT_ON_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pins : PA0 PA12 */
	GPIO_InitStruct.Pin = CHARGE_STAT_Pin | KEY_DOWN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PA4 PA7 PA11 PA15 */
	GPIO_InitStruct.Pin = Vref_en_Pin | RF_LEVEL2_Pin | WLED5_Pin | MBI5020_LE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB2 PB10
							 PB13 PB14 PB15 */
	GPIO_InitStruct.Pin = RF_LEVEL3_Pin | RF_LEVEL5_Pin | RF_LEVEL4_Pin | WLED2_Pin | GPIO_PIN_13 | PLED_PWM_Pin | WLED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PD0 */
	GPIO_InitStruct.Pin = KEY_UP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : PD2 PD3 */
	GPIO_InitStruct.Pin = OLED_PWM_Pin | BOOST_ON_5V_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_2, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_7 | GPIO_PIN_15, GPIO_PIN_RESET);
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
	// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);
	// HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
	// HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);

  Upkeep_LED_OFF();  // 将IO 口置为低电平
	Repair_LED_OFF();

	GEAR_LED1_DOWN();  // 档位灯关闭
	GEAR_LED2_DOWN();
	GEAR_LED3_DOWN();
	GEAR_LED4_DOWN();
	GEAR_LED5_DOWN();

	GEAR2_DOWN();      // 档位控制 io口关闭
	GEAR3_DOWN();
	GEAR4_DOWN();
#if(ARF001 == DEVICE_R1_HAIWAI)
  GEAR5_DOWN();
#endif

}
/******************************************************
 * gpio 初始化
*******************************************************/
void MX_GPIO_Init(void)
{
  ONLY_MX_GPIO_Init();

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/* USER CODE END 2 */
