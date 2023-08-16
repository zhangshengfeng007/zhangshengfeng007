/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "includes.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
pFunction Jump_To_Application;
uint32_t JumpAddress;
/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Jump_App_Handle(void)
{
  if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
  {
    JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
    /* Jump to application */
    Jump_To_Application();
  }
}

//----------------led 相关
uint8_t Led_Colour[15] = {0};//G R B
uint8_t LCD_data[120];

#define CodeOne  0x7f
#define CodeZero 0x70

#define White_R  255
#define White_G  255
#define White_B  255




/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void RGB_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
	static uint8_t Last_LCD_Data[121] = {0};
	static uint8_t Flag, Counts;

	Flag = memcmp(Last_LCD_Data, LCD_data, Size);

	if(Flag)// 显示有变化了再刷新
	{
		memcpy(Last_LCD_Data, LCD_data,Size);
		HAL_SPI_Transmit_DMA(hspi, LCD_data, sizeof(LCD_data));
	}
}
/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void LCD_Refresh(void)
{
  uint8_t i, j;
  uint8_t temp;

  for(i = 0; i < 15; i++)
  {
	  temp = Led_Colour[i];

    for(j = 0; j < 8; j ++)
    {
      if((temp & 0x80) == 0x80)
      {
        LCD_data[8 * i + j] = CodeOne;
      }
      else
      {
        LCD_data[8 * i + j] = CodeZero;
      }
      temp <<= 1;
    }
  }
	//  LCD_data[120] = 0;
  RGB_SPI_Transmit(&hspi1, LCD_data, sizeof(LCD_data));
}

/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void Set_Color_Handle(uint8_t R, uint8_t G, uint8_t B, uint8_t num)
{
  Led_Colour[num * 3 + 0] = G;
  Led_Colour[num * 3 + 1] = R;
  Led_Colour[num * 3 + 2] = B;
}
/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void RGB_Init(void)
{
  uint8_t i;

  for(i = 0; i < sizeof(Led_Colour); i++)
  {
    Led_Colour[i] = 0x00;
  }
  LCD_Refresh();
}

static void set_white_color(uint8_t led_index)
{
	Set_Color_Handle(White_R, White_R, White_R, led_index - 1);
}

/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
static void my_delay_ms(int delay_time)
{
    int i, j = 0;

    for(i = 0; i < 15000; i++)
    {
        for(j = 0; j < delay_time; j++)
        {

        }
    }
}

/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
static void print_16hex_asc(uint8_t data)
{
	uint8_t print_data[3];

	print_data[0] = (data >> 4) + '0';

	if(print_data[0] > '9')
	{
		print_data[0] = print_data[0] + ('a' - '9' -1);
	}

	print_data[1] = (data & 0x0f )+ '0';

	if(print_data[1] > '9')
	{
		print_data[1] = print_data[1] + ('a' - '9' -1);
	}

	//SerialPutChar(print_data);

//	print_data[1] = data & 0x0f + '0';
	//SerialPutChar(print_data);
	print_data[2]= '\0';
	Serial_PutString(&print_data[0]);

}

//uint8_t app_data[1024] = {0};
uint32_t app_data[10] = {0};
//---------------read app program--------------------
/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void Read_APP_Program(void)
{
    int i = 0, j = 0;
  uint32_t * temp_data_p = (uint32_t *)&app_data[0];
	uint32_t tmp_addr;
  uint8_t print_data;

    Serial_PutString("Read_APP_Program begin:\r\n");
	// SerialPutChar(0x55);    // 座位打印 app_data 的其实位置
	// print_16hex_asc(0x12);
	// print_16hex_asc(0x32);
	// print_16hex_asc(0x56);
	// print_16hex_asc(0x78);
	// print_16hex_asc(0xAA);

  // print_16hex_asc(0x55);
	Serial_PutString(" \r\n");
  for(i = 0; i < (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS)/ 1024; i ++)
   {
			for(j = 0; j < 256; j ++)
			{
        if(j % 4 == 0)
        {
          Serial_PutString(" \r\n");
        }

				tmp_addr = APPLICATION_ADDRESS + 4 *j + i * 1024;
				app_data[0] = *(__IO uint32_t*)(tmp_addr);

				print_16hex_asc(app_data[0]);
				print_16hex_asc(app_data[0] >> 8);
				print_16hex_asc(app_data[0] >> 16);
				print_16hex_asc(app_data[0] >> 24);
        Serial_PutString("  ");
			}
   }

    //Serial_PutString("Read_APP_Program finish:\r\n");


	SerialPutChar(0xFF);    // 座位打印 app_data 的结束位置
	SerialPutChar(0xAA);
	SerialPutChar(0xFF);
	SerialPutChar(0xAA);
	SerialPutChar(0xFF);
	SerialPutChar(0xAA);
}


//---------------------------------------------------

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int level_up_press_cnt = 0;
  int level_down_press_cnt = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  // MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  // RGB_Init();


   set_white_color(LEVEL1_DISP);
        LCD_Refresh();
//  Set_Color_Handle(255,255,255,0);
  /* USER CODE END 2 */
  SerialPutString("sys begin run bootloader\r\n");
//    my_delay_ms(1000);
//    SerialPutString("delay 2\r\n");
//    my_delay_ms(1000);
//    SerialPutString("delay 3\r\n");
//    my_delay_ms(1000);
//    SerialPutString("delay 4\r\n");
//    my_delay_ms(1000);
//    SerialPutString("delay 5\r\n");
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // if(LL_GPIO_IsInputPinSet(KEY_ON_OFF_GPIO_Port, KEY_ON_OFF_Pin)==RESET
		//        && LL_GPIO_IsInputPinSet(KEY_UP_GPIO_Port, KEY_UP_Pin)==RESET)

   if(LL_GPIO_IsInputPinSet(KEY_UP_GPIO_Port, KEY_UP_Pin) == RESET)
    {

      my_delay_ms(10);
      level_up_press_cnt ++;

      if(LL_GPIO_IsInputPinSet(KEY_DOWN_PORT, KEY_DOWN_PIN) == RESET)
      {
        level_down_press_cnt ++;

        if(level_down_press_cnt > 20)
        {
          level_down_press_cnt = 0;
           Main_Menu();
          // SerialPutString("key dec press\r\n");
        }
      }

      if(level_up_press_cnt > 300)
      {
        SerialPutString("key add press 3s\r\n");

        Read_APP_Program();
        level_up_press_cnt = 0;
      }



//       {
// //		 LCD_Refresh(Led_Colour);
//         if(HAL_GetTick() > 2000)
//         {
//           HAL_DeInit();
//           HAL_UART_MspDeInit(&huart2);
//           HAL_SPI_MspDeInit(&hspi1);
//           __disable_irq();
//           //			__set_CONTROL(0);
//           //			SysTick->VAL=0x00;
//           Jump_App_Handle();
//         }
//       }
      // Main_Menu();
    }
    else
    {
      if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
      {
         SerialPutString("jump to app\r\n");
          HAL_DeInit();
          HAL_UART_MspDeInit(&huart2);
          HAL_SPI_MspDeInit(&hspi1);
          __disable_irq();
          //		__set_CONTROL(0);
          //		  SysTick->VAL=0x00;
          Jump_App_Handle();
      }
      else
      {
        Main_Menu();
      }
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */












