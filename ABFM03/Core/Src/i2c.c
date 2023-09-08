/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c 123
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00707CBB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}
/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  // hi2c2.Instance = I2C2;
  // hi2c2.Init.Timing = 0x00707CBB;
  // hi2c2.Init.OwnAddress1 = 0;
  // hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  // hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  // hi2c2.Init.OwnAddress2 = 0;
  // hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  // hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  // hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  // if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  // {
  //   Error_Handler();
  // }

  // /** Configure Analogue filter
  // */
  // if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  // {
  //   Error_Handler();
  // }

  // /** Configure Digital filter
  // */
  // if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  // {
  //   Error_Handler();
  // }
  /* USER CODE BEGIN I2C2_Init 2 */
   GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = EMS_SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    // GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
    HAL_GPIO_Init(EMS_SCL_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = EMS_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
    HAL_GPIO_Init(EMS_SDA_PORT, &GPIO_InitStruct);
  /* USER CODE END I2C2_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PA9     ------> I2C1_SCL
    PA10     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = G_SENSOR_SCL_Pin|G_SENSOR_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_I2C1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     /**I2C2 GPIO Configuration
//     PA11 [PA9]     ------> I2C2_SCL
//     PA12 [PA10]     ------> I2C2_SDA
//     */
//     GPIO_InitStruct.Pin = EMS_SCL_Pin|EMS_SDA_Pin;
//     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
// //    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
// //    GPIO_InitStruct.Pull = GPIO_PULLUP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     /* I2C2 clock enable */
//     __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PA9     ------> I2C1_SCL
    PA10     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(G_SENSOR_SCL_PORT, G_SENSOR_SCL_Pin);

    HAL_GPIO_DeInit(G_SENSOR_SDA_PORT, G_SENSOR_SDA_Pin);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    // __HAL_RCC_I2C2_CLK_DISABLE();

    // /**I2C2 GPIO Configuration
    // PA11 [PA9]     ------> I2C2_SCL
    // PA12 [PA10]     ------> I2C2_SDA
    // */
    HAL_GPIO_DeInit(EMS_SCL_PORT, EMS_SCL_Pin);

    HAL_GPIO_DeInit(EMS_SDA_PORT, EMS_SDA_Pin);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/*************************************/
/*************************************/

void my_delya_us(uint8_t cnt)
{
    int i, j;

    for(j = 0; j < cnt; j++)
    {
      for(i = 0; i < 15; i++)
      {

      }
    }
}

/*************************************/
/*************************************/
void sda_cfg_output(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
   __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = EMS_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		//	GPIO_MODE_OUTPUT_OD   GPIO_MODE_OUTPUT_PP
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
  HAL_GPIO_Init(EMS_SDA_PORT, &GPIO_InitStruct);
}

/*************************************/
/*************************************/
void sda_cfg_input(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

   __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = EMS_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;  //GPIO_NOPULL  GPIO_PULLUP
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
  HAL_GPIO_Init(EMS_SDA_PORT, &GPIO_InitStruct);

}

/************************************/
//  SCL 为高电平时，SDA 由高电平转变为低电平
/************************************/
void gpio_iic_start(void)
{
  I2C_SCL_L();
  my_delya_us(5);
  sda_cfg_output();
  I2C_SCL_H();
  my_delya_us(5);
  I2C_SDA_L();
}

/************************************/
//  SCL 为高电平时，SDA 由高电平转变为低电平
/************************************/
void gpio_iic_stop(void)
{
  I2C_SCL_H();
  sda_cfg_output();
  I2C_SDA_L();
  my_delya_us(5);
  I2C_SDA_H();
}

/************************************/
//send one byte
//
/************************************/
void only_send_one_byte(uint8_t data)
{
    uint8_t tmp , i;

    tmp = data;
    for(i = 0; i < 8; i ++)
    {
			I2C_SCL_L();
			if(i == 0)
			{
				sda_cfg_output();
			}

     if(tmp & 0x80)
     {
        I2C_SDA_H();
     }
     else
     {
        I2C_SDA_L();
     }
     my_delya_us(5);
     tmp <<= 1;
     I2C_SCL_H();
     my_delya_us(5);
    }
}

/************************************/
//dev_addr 为 设备8bit 写入时的地址
//
/************************************/
void gpio_iic_send_bytes(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t i, j;
    uint8_t tmp;
    uint8_t send_buff[10];

   send_buff[0] = dev_addr;
   send_buff[1] = reg;

   for(i = 0; i < len; i++)
   {
    send_buff[i + 2] = data[i];
   }

   taskENTER_CRITICAL();  // 进入临界区， 通讯时码值尽量保持完整

    gpio_iic_start();
    for(i = 0; i < len + 2; i++)
    {
        only_send_one_byte(send_buff[i]);
        // wait ack************
    		I2C_SCL_L();
    		sda_cfg_input();   // 等待从机将 sda 拉低
    		my_delya_us(5);

    		I2C_SCL_H();
    		my_delya_us(10);
    }
   sda_cfg_output();
   I2C_SDA_H();
   taskEXIT_CRITICAL();		// 退出临界区
}

/************************************/
//
// dev_addr 为8bit 写入时的设备addr
/************************************/
void gpio_iic_read_bytes(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len)
{

  uint8_t i, j;
  uint8_t send_buff[10];
  uint8_t dev_addr_r;

  send_buff[0] = dev_addr + 1;

  taskENTER_CRITICAL();  // 进入临界区
  gpio_iic_send_bytes(dev_addr, reg, send_buff,  0);     // 先把 dev_addr 和reg发出去
  gpio_iic_stop();   // 发送一个stop 信号
  my_delya_us(5);
  gpio_iic_start();
  only_send_one_byte(send_buff[0]); //  发送读取指令

  // wait ack************
	I2C_SCL_L();
	sda_cfg_input();   // 等待从机将 sda 拉低
	my_delya_us(5);
	I2C_SCL_H();
	my_delya_us(10);

  for(i = 0; i < len ; i++)
  {
    I2C_SCL_L();
    sda_cfg_input();
    data[i] = 0;
    for(j = 0; j < 8; j++)
    {
        data[i] <<= 1;
        I2C_SCL_L();
        my_delya_us(5);
        I2C_SCL_H();

        if(IS_SDA_H())
        {
          data[i] |= 0x01;
        }
        else
        {
          data[i] |= 0x00;
        }
        my_delya_us(5);
    }

  	// 主机发送   ACK, 表示接收到数据成功，从机可以继续发送数据*******
		I2C_SCL_L();
		sda_cfg_output();
		I2C_SDA_L();
		my_delya_us(5);
		I2C_SCL_H();
		my_delya_us(5);
  }
 sda_cfg_output();
 I2C_SDA_H();

 taskEXIT_CRITICAL();
}

/* USER CODE END 1 */
