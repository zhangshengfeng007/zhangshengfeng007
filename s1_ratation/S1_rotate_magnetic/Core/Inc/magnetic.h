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
#ifndef __MAGNETIC_H
#define __MAGNETIC_H

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


/* USER CODE BEGIN EFP */

typedef enum{
	MAG_STEP1 =0,
	MAG_STEP2,
	MAG_STEP3,
	MAG_STEP4,
	MAG_STEP5,
	MAG_STEP6,
	MAG_STEP7,
}magnetic_step_e;


typedef enum{
	SLOPE_TYPE1 =0,
	SLOPE_TYPE2,
	VERTICAL_TYPE
}magnetic_type_e;

typedef enum{
	P_PIN_ON = 0,
	P_PIN_OFF,
	N_PIN_ON,
	N_PIN_OFF,
}pin_type_e;

//void (*p_magnetic_fun)(void);

typedef struct
{
	magnetic_type_e mag_type;
	void (*p_fun)(void);
	uint16_t set_repeat_times;
}magentic_data_t;


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

void Slope1_magnetic1_out(void);
void Slope2_magnetic2_out(void);
void Vertical_magnetic_out(void);
void Magen_select_n_pins_on(void);


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAGNETIC_H */
