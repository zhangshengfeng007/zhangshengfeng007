/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "stdbool.h"
#include <string.h>
#include "Test_Mode.h"
#include "stdio.h"
#include "stdarg.h"
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

UART_HandleTypeDef huart2;

uint8_t Rx_buff[RX_BUF_MAX_LEN];
uint8_t Tx_buff[TX_BUF_MAX_LEN];
uint8_t recv_cnt = 0;
uint8_t recv_finish_flag = 0;

extern uint16_t crc16_compute(uint8_t const * p_data, uint32_t size);


//HAL_UART_Receive_DMA(uartHandle, FrameBuf.frame[FrameBuf.index].data, RX_BUF_MAX_LEN);
//__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXFNE);//UART_IT_IDLE); 123

//download test peer code:
//0x00 0A 00 15 AA 55 55 AA 37 74
//0x00 0A 00 15 A2 00 00 14 B0 D6
//0x00 08 00 16 00 00 FD FE


/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE); // 20230425  �����ǿ��ж�
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); //  20230425 ���������ж�
  /* USER CODE END USART2_Init 2 */

}



void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;//GPIO_NOPULL;  GPIO_PULLUP
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //   GPIO_SPEED_FREQ_LOW
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */



/*
串口发送函数，增加进入临界区保护，否则串口发送数据容易被打断。导致上位机软件读取解析失败
*/
void send_data_to_usart(uint8_t *pbuf, uint8_t len)
{
  taskENTER_CRITICAL();
	HAL_UART_Transmit(&huart2, pbuf, len, 5);
  taskEXIT_CRITICAL();
}

/**********************************************/
// ���ڷ��ͺ���������crcУ��
/**********************************************/
void Send_Data_with_crc(uint8_t *send_buff ,uint8_t len)
{
	uint16_t Crc;

	Crc= crc16_compute(send_buff, len-2);
	send_buff[len-2] = (Crc>>8)&0xff;
	send_buff[len-1] = Crc & 0xff;

	send_data_to_usart(send_buff, len);
}

/************************************************
  	�����жϴ�������
*************************************************/
void uart_idle_callback(UART_HandleTypeDef *huart)//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET)
	{
		if(recv_cnt >= RX_BUF_MAX_LEN -1)
		{
			recv_cnt = 0;
		}
		Rx_buff[recv_cnt ++] = huart2.Instance->RDR;
	}
	else if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)
	{
		//
		__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_IDLE);
		recv_finish_flag = 1;
	}
}

/* USER CODE END 1 */
