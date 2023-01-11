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

/* USER CODE BEGIN Includes */
#define A1_P_PORT   	(GPIOC)
#define A1_N_PORT   	(GPIOC)
#define A1_P_PIN  	 	(GPIO_PIN_14)
#define A1_N_PIN  	 	(GPIO_PIN_13) 
#define A1_P_ON()   	(HAL_GPIO_WritePin(A1_P_PORT, A1_P_PIN, GPIO_PIN_SET))
#define A1_N_ON()   	(HAL_GPIO_WritePin(A1_N_PORT, A1_N_PIN, GPIO_PIN_SET))
#define A1_P_OFF()    (HAL_GPIO_WritePin(A1_P_PORT, A1_P_PIN, GPIO_PIN_RESET))
#define A1_N_OFF()    (HAL_GPIO_WritePin(A1_N_PORT, A1_N_PIN, GPIO_PIN_RESET))

#define A2_P_PORT    (GPIOF)
#define A2_N_PORT    (GPIOC)
#define A2_P_PIN     (GPIO_PIN_0) 
#define A2_N_PIN     (GPIO_PIN_15)
#define A2_P_ON()    (HAL_GPIO_WritePin(A2_P_PORT, A2_P_PIN, GPIO_PIN_SET))
#define A2_N_ON()    (HAL_GPIO_WritePin(A2_N_PORT, A2_N_PIN, GPIO_PIN_SET))				 
#define A2_P_OFF()   (HAL_GPIO_WritePin(A2_P_PORT, A2_P_PIN, GPIO_PIN_RESET))
#define A2_N_OFF()   (HAL_GPIO_WritePin(A2_N_PORT, A2_N_PIN, GPIO_PIN_RESET))			 

#define A3_P_PORT    (GPIOA)
#define A3_N_PORT    (GPIOF)
#define A3_P_PIN     (GPIO_PIN_0) 
#define A3_N_PIN     (GPIO_PIN_1)
// #define A3_P_ON()    (HAL_GPIO_WritePin(A3_P_PORT, A3_P_PIN, GPIO_PIN_SET))
// #define A3_N_ON()    (HAL_GPIO_WritePin(A3_N_PORT, A3_N_PIN, GPIO_PIN_SET))				 
// #define A3_P_OFF()   (HAL_GPIO_WritePin(A3_P_PORT, A3_P_PIN, GPIO_PIN_RESET))
// #define A3_N_OFF()   (HAL_GPIO_WritePin(A3_N_PORT, A3_N_PIN, GPIO_PIN_RESET))		

#define A4_P_PORT    (GPIOA)
#define A4_N_PORT    (GPIOA)
#define A4_P_PIN     (GPIO_PIN_2) 
#define A4_N_PIN     (GPIO_PIN_1)
#define A4_P_ON()    (HAL_GPIO_WritePin(A4_P_PORT, A4_P_PIN, GPIO_PIN_SET))
#define A4_N_ON()    (HAL_GPIO_WritePin(A4_N_PORT, A4_N_PIN, GPIO_PIN_SET))				 
#define A4_P_OFF()   (HAL_GPIO_WritePin(A4_P_PORT, A4_P_PIN, GPIO_PIN_RESET))
#define A4_N_OFF()   (HAL_GPIO_WritePin(A4_N_PORT, A4_N_PIN, GPIO_PIN_RESET))	

#define A5_P_PORT    (GPIOA)
#define A5_N_PORT    (GPIOA)
#define A5_P_PIN     (GPIO_PIN_4) 
#define A5_N_PIN     (GPIO_PIN_3)
#define A5_P_ON()    (HAL_GPIO_WritePin(A5_P_PORT, A5_P_PIN, GPIO_PIN_SET))
#define A5_N_ON()    (HAL_GPIO_WritePin(A5_N_PORT, A5_N_PIN, GPIO_PIN_SET))				 
#define A5_P_OFF()   (HAL_GPIO_WritePin(A5_P_PORT, A5_P_PIN, GPIO_PIN_RESET))
#define A5_N_OFF()   (HAL_GPIO_WritePin(A5_N_PORT, A5_N_PIN, GPIO_PIN_RESET))	


#define A6_P_PORT    (GPIOA)
#define A6_N_PORT    (GPIOA)
#define A6_P_PIN     (GPIO_PIN_6) 
#define A6_N_PIN     (GPIO_PIN_5)
#define A6_P_ON()    (HAL_GPIO_WritePin(A6_P_PORT, A6_P_PIN, GPIO_PIN_SET))
#define A6_N_ON()    (HAL_GPIO_WritePin(A6_N_PORT, A6_N_PIN, GPIO_PIN_SET))				 
#define A6_P_OFF()   (HAL_GPIO_WritePin(A6_P_PORT, A6_P_PIN, GPIO_PIN_RESET))
#define A6_N_OFF()   (HAL_GPIO_WritePin(A6_N_PORT, A6_N_PIN, GPIO_PIN_RESET))	


#define A7_P_PORT    (GPIOB)
#define A7_N_PORT    (GPIOA)
#define A7_P_PIN     (GPIO_PIN_0) 
#define A7_N_PIN     (GPIO_PIN_7)
#define A7_P_ON()    (HAL_GPIO_WritePin(A7_P_PORT, A7_P_PIN, GPIO_PIN_SET))
#define A7_N_ON()    (HAL_GPIO_WritePin(A7_N_PORT, A7_N_PIN, GPIO_PIN_SET))				 
#define A7_P_OFF()   (HAL_GPIO_WritePin(A7_P_PORT, A7_P_PIN, GPIO_PIN_RESET))
#define A7_N_OFF()   (HAL_GPIO_WritePin(A7_N_PORT, A7_N_PIN, GPIO_PIN_RESET))	

//------------第二排B --------------------------

#define B1_P_PORT    (GPIOB)
#define B1_N_PORT    (GPIOB)
#define B1_P_PIN     (GPIO_PIN_2) 
#define B1_N_PIN     (GPIO_PIN_1)
#define B1_P_ON()    (HAL_GPIO_WritePin(B1_P_PORT, B1_P_PIN, GPIO_PIN_SET))
#define B1_N_ON()    (HAL_GPIO_WritePin(B1_N_PORT, B1_N_PIN, GPIO_PIN_SET))				 
#define B1_P_OFF()   (HAL_GPIO_WritePin(B1_P_PORT, B1_P_PIN, GPIO_PIN_RESET))
#define B1_N_OFF()   (HAL_GPIO_WritePin(B1_N_PORT, B1_N_PIN, GPIO_PIN_RESET))	

#define B2_P_PORT    (GPIOB)
#define B2_N_PORT    (GPIOB)
#define B2_P_PIN     (GPIO_PIN_11) 
#define B2_N_PIN     (GPIO_PIN_10)
#define B2_P_ON()    (HAL_GPIO_WritePin(B2_P_PORT, B2_P_PIN, GPIO_PIN_SET))
#define B2_N_ON()    (HAL_GPIO_WritePin(B2_N_PORT, B2_N_PIN, GPIO_PIN_SET))				 
#define B2_P_OFF()   (HAL_GPIO_WritePin(B2_P_PORT, B2_P_PIN, GPIO_PIN_RESET))
#define B2_N_OFF()   (HAL_GPIO_WritePin(B2_N_PORT, B2_N_PIN, GPIO_PIN_RESET))	

#define B3_P_PORT    (GPIOB)
#define B3_N_PORT    (GPIOB)
#define B3_P_PIN     (GPIO_PIN_13) 
#define B3_N_PIN     (GPIO_PIN_12)
#define B3_P_ON()    (HAL_GPIO_WritePin(B3_P_PORT, B3_P_PIN, GPIO_PIN_SET))
#define B3_N_ON()    (HAL_GPIO_WritePin(B3_N_PORT, B3_N_PIN, GPIO_PIN_SET))				 
#define B3_P_OFF()   (HAL_GPIO_WritePin(B3_P_PORT, B3_P_PIN, GPIO_PIN_RESET))
#define B3_N_OFF()   (HAL_GPIO_WritePin(B3_N_PORT, B3_N_PIN, GPIO_PIN_RESET))	

#define B4_P_PORT    (GPIOB)
#define B4_N_PORT    (GPIOB)
#define B4_P_PIN     (GPIO_PIN_15) 
#define B4_N_PIN     (GPIO_PIN_14)
#define B4_P_ON()    (HAL_GPIO_WritePin(B4_P_PORT, B4_P_PIN, GPIO_PIN_SET))
#define B4_N_ON()    (HAL_GPIO_WritePin(B4_N_PORT, B4_N_PIN, GPIO_PIN_SET))				 
#define B4_P_OFF()   (HAL_GPIO_WritePin(B4_P_PORT, B4_P_PIN, GPIO_PIN_RESET))
#define B4_N_OFF()   (HAL_GPIO_WritePin(B4_N_PORT, B4_N_PIN, GPIO_PIN_RESET))	

#define B5_P_PORT    (GPIOA)
#define B5_N_PORT    (GPIOA)
#define B5_P_PIN     (GPIO_PIN_9) 
#define B5_N_PIN     (GPIO_PIN_8)
#define B5_P_ON()    (HAL_GPIO_WritePin(B5_P_PORT, B5_P_PIN, GPIO_PIN_SET))
#define B5_N_ON()    (HAL_GPIO_WritePin(B5_N_PORT, B5_N_PIN, GPIO_PIN_SET))				 
#define B5_P_OFF()   (HAL_GPIO_WritePin(B5_P_PORT, B5_P_PIN, GPIO_PIN_RESET))
#define B5_N_OFF()   (HAL_GPIO_WritePin(B5_N_PORT, B5_N_PIN, GPIO_PIN_RESET))	

//-----------第三排 c 排
#define C1_P_PORT    (GPIOA)
#define C1_N_PORT    (GPIOA)
#define C1_P_PIN     (GPIO_PIN_11) 
#define C1_N_PIN     (GPIO_PIN_10)
#define C1_P_ON()    (HAL_GPIO_WritePin(C1_P_PORT, C1_P_PIN, GPIO_PIN_SET))
#define C1_N_ON()    (HAL_GPIO_WritePin(C1_N_PORT, C1_N_PIN, GPIO_PIN_SET))				 
#define C1_P_OFF()   (HAL_GPIO_WritePin(C1_P_PORT, C1_P_PIN, GPIO_PIN_RESET))
#define C1_N_OFF()   (HAL_GPIO_WritePin(C1_N_PORT, C1_N_PIN, GPIO_PIN_RESET))	

#define C2_P_PORT    (GPIOA)
#define C2_N_PORT    (GPIOA)
#define C2_P_PIN     (GPIO_PIN_15) 
#define C2_N_PIN     (GPIO_PIN_12)
#define C2_P_ON()    (HAL_GPIO_WritePin(C2_P_PORT, C2_P_PIN, GPIO_PIN_SET))
#define C2_N_ON()    (HAL_GPIO_WritePin(C2_N_PORT, C2_N_PIN, GPIO_PIN_SET))				 
#define C2_P_OFF()   (HAL_GPIO_WritePin(C2_P_PORT, C2_P_PIN, GPIO_PIN_RESET))
#define C2_N_OFF()   (HAL_GPIO_WritePin(C2_N_PORT, C2_N_PIN, GPIO_PIN_RESET))	

#define C3_P_PORT    (GPIOD)
#define C3_N_PORT    (GPIOD)
#define C3_P_PIN     (GPIO_PIN_1) 
#define C3_N_PIN     (GPIO_PIN_0)
#define C3_P_ON()    (HAL_GPIO_WritePin(C3_P_PORT, C3_P_PIN, GPIO_PIN_SET))
#define C3_N_ON()    (HAL_GPIO_WritePin(C3_N_PORT, C3_N_PIN, GPIO_PIN_SET))				 
#define C3_P_OFF()   (HAL_GPIO_WritePin(C3_P_PORT, C3_P_PIN, GPIO_PIN_RESET))
#define C3_N_OFF()   (HAL_GPIO_WritePin(C3_N_PORT, C3_N_PIN, GPIO_PIN_RESET))	


#define C4_P_PORT    (GPIOD)
#define C4_N_PORT    (GPIOD)
#define C4_P_PIN     (GPIO_PIN_3) 
#define C4_N_PIN     (GPIO_PIN_2)
#define C4_P_ON()    (HAL_GPIO_WritePin(C4_P_PORT, C4_P_PIN, GPIO_PIN_SET))
#define C4_N_ON()    (HAL_GPIO_WritePin(C4_N_PORT, C4_N_PIN, GPIO_PIN_SET))				 
#define C4_P_OFF()   (HAL_GPIO_WritePin(C4_P_PORT, C4_P_PIN, GPIO_PIN_RESET))
#define C4_N_OFF()   (HAL_GPIO_WritePin(C4_N_PORT, C4_N_PIN, GPIO_PIN_RESET))	


#define C5_P_PORT    (GPIOB)
#define C5_N_PORT    (GPIOB)
#define C5_P_PIN     (GPIO_PIN_4) 
#define C5_N_PIN     (GPIO_PIN_3)
#define C5_P_ON()    (HAL_GPIO_WritePin(C5_P_PORT, C5_P_PIN, GPIO_PIN_SET))
#define C5_N_ON()    (HAL_GPIO_WritePin(C5_N_PORT, C5_N_PIN, GPIO_PIN_SET))				 
#define C5_P_OFF()   (HAL_GPIO_WritePin(C5_P_PORT, C5_P_PIN, GPIO_PIN_RESET))
#define C5_N_OFF()   (HAL_GPIO_WritePin(C5_N_PORT, C5_N_PIN, GPIO_PIN_RESET))	

#define C6_P_PORT    (GPIOB)
#define C6_N_PORT    (GPIOB)
#define C6_P_PIN     (GPIO_PIN_6) 
#define C6_N_PIN     (GPIO_PIN_5)
#define C6_P_ON()    (HAL_GPIO_WritePin(C6_P_PORT, C6_P_PIN, GPIO_PIN_SET))
#define C6_N_ON()    (HAL_GPIO_WritePin(C6_N_PORT, C6_N_PIN, GPIO_PIN_SET))				 
#define C6_P_OFF()   (HAL_GPIO_WritePin(C6_P_PORT, C6_P_PIN, GPIO_PIN_RESET))
#define C6_N_OFF()   (HAL_GPIO_WritePin(C6_N_PORT, C6_N_PIN, GPIO_PIN_RESET))	

#define C7_P_PORT    (GPIOB)
#define C7_N_PORT    (GPIOB)
#define C7_P_PIN     (GPIO_PIN_8) 
#define C7_N_PIN     (GPIO_PIN_7)
#define C7_P_ON()    (HAL_GPIO_WritePin(C7_P_PORT, C7_P_PIN, GPIO_PIN_SET))
#define C7_N_ON()    (HAL_GPIO_WritePin(C7_N_PORT, C7_N_PIN, GPIO_PIN_SET))				 
#define C7_P_OFF()   (HAL_GPIO_WritePin(C7_P_PORT, C7_P_PIN, GPIO_PIN_RESET))
#define C7_N_OFF()   (HAL_GPIO_WritePin(C7_N_PORT, C7_N_PIN, GPIO_PIN_RESET))	






typedef enum{
	MAG_A1 =0,
	MAG_A2,
	MAG_A3,
	MAG_A4,
	MAG_A5,
	MAG_A6,
	MAG_A7,
	MAG_B1,
	MAG_B2,
	MAG_B3,
	MAG_B4,
	MAG_B5,
	MAG_C1,
	MAG_C2,
	MAG_C3,
	MAG_C4,
	MAG_C5,
	MAG_C6,
	MAG_C7,
}magen_pin_e;


typedef struct{
  magen_pin_e magen_type;
  GPIO_TypeDef * p_port;
  uint16_t p_pin;
  GPIO_TypeDef * n_port;
  uint16_t n_pin;	
}port_pin_t;

typedef struct 
{
	uint8_t search_index;
	uint8_t cur_magen_step;
	GPIO_TypeDef * test_port;
	//pin_type_e pin_type;
	port_pin_t port_pin;
  magen_pin_e magen;
	/* data */
}mytest_t;

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
uint8_t find_port_index(magen_pin_e mage_type);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

