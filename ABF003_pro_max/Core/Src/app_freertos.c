/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "includes.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 80);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityIdle, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */

//  System_Data_Init();
//  LedIR_Init();
//#if LIS2DH
//  //		MX_I2C2_Init();
//  Lis2dh_Init();
//#else
//  //		IIC_Init();
//  lis3dhinit();
//#endif
//#if ARF001
//  ;
//#else
//  MX_TIM3_Init();
//#endif
  /*****************测试******************/
  //	VBAT_OUT_ON(); //
  //	BOOST_5V_ON();
  /***************************************/
  TickType_t lasttick = xTaskGetTickCount();
  /* Infinite loop */
  for (;;)
  {
    System_1mS_Procedure();
    vTaskDelayUntil(&lasttick, 1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the myTask02 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  TickType_t lasttick = xTaskGetTickCount();
  static uint8_t TimerCnt;
  /* Infinite loop */
  for (;;)
  {

#if DEBUG_IDWG
    HAL_IWDG_Refresh(&hiwdg);
#endif

    if (++TimerCnt >= 2)
    {
      TimerCnt = 0;
      if (!SysInfo.Batt_Value.Usb_flag && !SysInfo.Test_Mode.Test_Mode_Flag)
      {
        Key_Proc();
      }

      if (SysInfo.Test_Mode.Test_Mode_Flag == ON || SysInfo.Test_Mode.Auto_Mode_Level == 0x04) // 测试模式按键检测
      {
        Test_Key_Proc();
      }
    }
    System_10mS_Procedure();
    vTaskDelayUntil(&lasttick, 10);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the myTask03 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
  TickType_t lasttick = xTaskGetTickCount();
  static uint8_t Counts_100ms, Counts_1000ms;
  /* Infinite loop */
  for (;;)
  {

    SLIDE_MODE_Run();
//		if(Counts_1000ms++>40)
//		{
//			printf ("\n\r%.1f\n\r",SysInfo.NTC_Temp);	             //测试
////			HAL_UART_Transmit(&huart1, (uint8_t *)(SysInfo.NTC_Temp), 1, 50);
//			Counts_1000ms = 0;
//	//		printf ("\n\r NTC: %.1f ??\n\r",SysInfo.NTC_Temp);	 //测试

//		}

    if (++Counts_100ms > 4)
    {
      Counts_100ms = 0;
      System_100mS_Procedure();
    }
#if ENABLE_FREERTOS_HEAP_STACK_LOG
	UBaseType_t istack1,istack2,istack3;
	size_t FreeHeap,MinEverFreeHeap;
//	HeapStats_t xHeapStats;
	istack1=uxTaskGetStackHighWaterMark( defaultTaskHandle );
	istack2=uxTaskGetStackHighWaterMark( myTask02Handle);
	istack3=uxTaskGetStackHighWaterMark( myTask03Handle);
	FreeHeap=xPortGetFreeHeapSize();
	MinEverFreeHeap=xPortGetMinimumEverFreeHeapSize();
//	vPortGetHeapStats( &xHeapStats);

	printf ("\n\r heap:%d,%d stack:%d,%d,%d\n\r",
	FreeHeap,////获取当前未分配的内存堆大小
	MinEverFreeHeap,//获取未分配的内存堆历史最小值
	istack1,istack2,istack3);//任务栈空间的历史最小剩余值,1个字长为4个字节
#endif	
    vTaskDelayUntil(&lasttick, 25);
  }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

