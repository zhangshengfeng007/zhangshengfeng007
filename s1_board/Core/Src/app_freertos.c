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
//extern IWDG_HandleTypeDef hiwdg;
/* USER CODE END PTD */
extern UART_HandleTypeDef huart2;
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern void WatchdogInit(void);
extern void WatchdogKick(void);
extern void WatchdogRegister(const char *name, uint16_t timeout);
extern void WatchdogFeed(void);
//extern uint8_t recv_data_buff[8];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//typedef enum
//{
//  Sys_Task1,
//  Sys_Task2,
//  Sys_Task3,
//}task_num;
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

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
  osThreadDef(myTask02, StartTask02, osPriorityLow, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityLow, 0, 102);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  WatchdogInit();
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
  TickType_t xLastWakeTime = xTaskGetTickCount();//54
  SYSTEM_Init_Function();
  // WatchdogRegister("Sys_Task1",1000);  -- 2023 01 11 �رո����߳�֮��Ŀ��Ź�

  /* Infinite loop */
  for(;;)
  {
    //task1_stack_size1 = uxTaskGetStackHighWaterMark(NULL);    
    if(Sys_Info.Work_Mode == NORMAL)
    {
        RF_Value_PID_Handler();
        EMS_Value_PID_Handler();
        RF_Handler.RF_Load_Flag = loadCurrent();    //RF  ���ص������  Colour_Type
        RF_EMS_OUTPUT_Function();
    }
		else if(Sys_Info.Work_Mode == TESTMODE)
		{
				if(Sys_Info.Test_Mode.TestMode!=Auto_TestModel)
				Sys_Info.Key_State.Mode_choice_key = SLIDE_MODE;
        RF_Value_PID_Handler();
        EMS_Value_PID_Handler();
        RF_Handler.RF_Load_Flag = loadCurrent();    //RF  ���ص������  Colour_Type
//				if(Sys_Info.Test_Mode.TestMode!=Auto_TestModel)
//				if(!Sys_Info.Test_Mode.RF_EMSTEST_Flag)
        RF_EMS_OUTPUT_Function();
		}
    
    // WatchdogKick();
    //----------------------------------
    vTaskDelayUntil(&xLastWakeTime, 1);
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
  static uint8_t time_cnt1 = 0;
  TickType_t xLastWakeTime = xTaskGetTickCount(); //84
  // WatchdogRegister("Sys_Task2",2000);
  /* Infinite loop */
  for(;;)
  {
		//task1_heap_size1 = xPortGetFreeHeapSize();
		//task1_heap_size2 = xPortGetMinimumEverFreeHeapSize();
		//task1_stack_size2 = uxTaskGetStackHighWaterMark(NULL);
    if(Sys_Info.Work_Mode == NORMAL)
    {
	
			Control_Funtion();
		
		  MOTO_Handle();
		  Moto_Set_Funtion();
    }
		else if(Sys_Info.Work_Mode == TESTMODE)
		{
//			Charge_Insertion_Status();    //USB���봦��
			
				Control_Funtion();
			
			  MOTO_Handle();
			  Moto_Set_Funtion();
			if(Sys_Info.Test_Mode.TestMode!=Auto_TestModel)
			{
				Sys_Info.G_sensor_active_flage = 1;
				Sys_Info.Key_State.Slide_gear = GEARTHREE;
			}
			Backlight_Display_Funtion();
			
			
			if(Sys_Info.Test_Mode.TestMode ==Auto_TestModel)
			{
				if(!Sys_Info.Test_Mode.RF_EMSTEST_Flag)
				{
					Sys_Info.Key_State.Mode_choice_key = SEAL_MODE;
					if(Sys_Info.TestMode_RF_Fre == 1)
						Sys_Info.SealMode_Conter = 4000;
					else if(Sys_Info.TestMode_RF_Fre == 2)
						Sys_Info.SealMode_Conter = 2000;
					else if(Sys_Info.TestMode_RF_Fre == 3)
						Sys_Info.SealMode_Conter = 900;
				}	
				else
				{
					Sys_Info.G_sensor_active_flage = 0;
					Sys_Info.Key_State.Mode_choice_key = SLIDE_MODE;
					Sys_Info.Key_State.Mode_key = RF_WITH_EMS;
				}
			}				
			
			
		}	
    if(++time_cnt1 > 2)//2
    {
      time_cnt1 = 0;
      Key_Proc();		
    }
    
    // WatchdogKick();
    //----------------------------------
    vTaskDelayUntil(&xLastWakeTime, 10);
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
  //TickType_t xLastWakeTime = xTaskGetTickCount(); //68
  User_Uart_Init();
  // WatchdogRegister("Sys_Task3",2000);
  /* Infinite loop */
  for(;;)
  {
    //task1_stack_size3 = uxTaskGetStackHighWaterMark(NULL);
    if(Sys_Info.Work_Mode == NORMAL)
    {
//			Uart2_Send_Data();   // 2023 01 11 ���Դ�
      LED_display();
			Backlight_Display_Funtion();
			if(Sys_Info.Adc_First_Run_Flage)
      BatteryPowerJudgment(Sys_Info.Power_State);   //��ص�ѹ���
        
#ifdef ENABLE_CHECK_SYS_ERROR
      System_Error_Check_Funtion();
    }
    else if(Sys_Info.Work_Mode == SYS_ERROR)
    {
      System_Error_Handle();
    }
#else
    }
#endif
    else if(Sys_Info.Work_Mode == TESTMODE)
		{
			if(Sys_Info.Test_Mode.TestMode!=Auto_TestModel)
      LED_display();
      BatteryPowerJudgment(Sys_Info.Power_State);   //��ص�ѹ���
			Aging_Test();
		}
		if(Sys_Info.Test_Mode.Receive_Flag)
		{
			Sys_Info.Test_Mode.Receive_Flag = 0;
			Sys_Command_Handle(recv_data_buff);
			memset(recv_data_buff,0,sizeof(recv_data_buff));
			HAL_UARTEx_ReceiveToIdle_DMA(&huart2, recv_data_buff, sizeof(recv_data_buff));   //��������DMA�����жϽ��գ�������һ�ν�������
		}			
    // WatchdogKick();
    //----------------------------------
    osDelay(100);
    WatchdogFeed(); // 2023 01 11 ͳһ������ι����
    //vTaskDelayUntil(&xLastWakeTime, 100);
  }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

