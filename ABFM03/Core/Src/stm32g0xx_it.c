/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS. 123
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */

  /* USER CODE END EXTI4_15_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(IRQ_2_4G_Pin);
  HAL_GPIO_EXTI_IRQHandler(POWER_KEY_PIN);
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */

  /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */


	if(__HAL_DMA_GET_FLAG(&hdma_adc1, DMA_FLAG_TC1)!=RESET)
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_adc1, DMA_FLAG_TC1);
	}
	if(__HAL_DMA_GET_FLAG(&hdma_adc1, DMA_FLAG_HT1)!=RESET)
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_adc1,DMA_FLAG_HT1);
	}
	if(__HAL_DMA_GET_FLAG(&hdma_adc1, DMA_FLAG_TE1)!=RESET)
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_adc1, DMA_FLAG_TE1);
	}
	if(__HAL_DMA_GET_FLAG(&hdma_adc1, DMA_FLAG_GI1)!=RESET)
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_adc1, DMA_FLAG_GI1);
	}
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */
  /* USER CODE END DMA1_Channel2_3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);

	if(__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_TC2)!=RESET)
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_FLAG_TC2);
	}
	if(__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_HT2)!=RESET)
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx,DMA_FLAG_HT2);
	}
	if(__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_TE2)!=RESET)
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_FLAG_TE2);
	}
	if(__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_GI2)!=RESET)
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_FLAG_GI2);
	}


  // HAL_DMA_IRQHandler(&hdma_usart2_rx);



	// if(__HAL_DMA_GET_FLAG(&hdma_usart2_rx, DMA_FLAG_TC3)!=RESET)
	// {
	// 	__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx, DMA_FLAG_TC3);
	// }
	// if(__HAL_DMA_GET_FLAG(&hdma_usart2_rx, DMA_FLAG_HT3)!=RESET)
	// {
	// 	__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx,DMA_FLAG_HT3);
	// }
	// if(__HAL_DMA_GET_FLAG(&hdma_usart2_rx, DMA_FLAG_TE3)!=RESET)
	// {
	// 	__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx, DMA_FLAG_TE3);
	// }
	// if(__HAL_DMA_GET_FLAG(&hdma_usart2_rx, DMA_FLAG_GI3)!=RESET)
	// {
	// 	__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx, DMA_FLAG_GI3);
	// }

  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */
  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 4, channel 5 and DMAMUX1 interrupts.
  */
void DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Ch4_5_DMAMUX1_OVR_IRQn 0 */

  /* USER CODE END DMA1_Ch4_5_DMAMUX1_OVR_IRQn 0 */
  // HAL_DMA_IRQHandler(&hdma_usart2_tx);
  /* USER CODE BEGIN DMA1_Ch4_5_DMAMUX1_OVR_IRQn 1 */

  /* USER CODE END DMA1_Ch4_5_DMAMUX1_OVR_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
	uart_idle_callback(&huart2);
  /* USER CODE END USART2_IRQn 1 */
}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
