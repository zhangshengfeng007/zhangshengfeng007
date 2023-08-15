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
//  static uint32_t xStartWakeTime,xEndWakeTime,xResultWakeTime;
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


  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 100);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityLow, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityLow, 0, 400);//102);
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
	// System_Data_Init();
  TickType_t xLastWakeTime = xTaskGetTickCount();//54

  /* Infinite loop */
  for(;;)
  {
	RF_Process(&RF_Handle);
	SysInfo.Batt_Value.State=Scan_Batter_State();
	Skin_RF_Scan();
	Pole_Change();       //��ת�缫
	vTaskDelayUntil(&xLastWakeTime, 1);
//	Uart_Protocol_Process();
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
//   static uint8_t  TimerCnt;
  TickType_t xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
#ifdef ENABLE_IWDG_FUNC
	HAL_IWDG_Refresh(&hiwdg);
#endif
	//if(SysInfo.Test_Mode.Test_Mode_Flag != OFF && SysInfo.Test_Mode.Ageing_Flag)
	if(SysInfo.Test_Mode.Test_Mode_Flag)
	{
		Test_USB_Plug_Scan();
	}
	else
	{
		USB_Plug_Scan();
	}
	Montor_Drive(&SysInfo);
	Led_Process_Run();
	NTC_Temp_Running();
	Skin_Touch_Scan();
	Key_Enter_Test_Mode_Check();
	Key_2_4G_auto_match_mode_Check();
	HW_Process();
	Change_EMS_Freq();
	Frequency_conversion_Process();

	if(SysInfo.Test_Mode.Test_Mode_Flag == Standy || SysInfo.Test_Mode.Test_Mode_Flag == ON)
	{
		Test_Key_Proc();
		if((AGING_TEST != SysInfo.Test_Mode.STATUS)&&(INSET_LIFE_TEST != SysInfo.Test_Mode.STATUS))
		{
			Vibration_Reminder_Counts_Run();
		}
		Test_mode_ctrl_Process();
		Test_Mode_EMS_RF_ctrl();
		NTC_Temp_SendData();
	}
	else  //
	{
		SLIDE_MODE_Run();
		Vibration_Reminder_Counts_Run();
		RF_Procedure_Run();
		EMS_Procedure_Run();
		System_Standby_Check(); //60s无接触关机�?��??�??次接触皮肤马达振�??
		if(!SysInfo.Batt_Value.Usb_flag)
		{
			Key_Proc();
		}
	}
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
  static uint8_t Timer_25ms_CNT;
  uint8_t Printf_delay_cnt;

  TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {

    if(SysInfo.Power_Value.state == System_ON)
	{
		SysInfo.MotionStateFlage = AttitudeProcessing();    //g_sensor  move_flag
		//SysInfo.MotionStateFlage = 1; // 2023 07 04 debug,强制打开
	}
	if(++Timer_25ms_CNT >= 4)
	{
		Timer_25ms_CNT =0;
		Set_OverTemp_Parameter(); 		  //���±�������������
		System_Error_Deal();              //������ʾ��������
//		Frequency_conversion_Process();   //��Ƶ
		if(EMS_Handle.vol_value <= Level_20) //VEMS��ѹ���� ��λС��20����ÿ��һ����1V
		{
			Set_Ems_Value(&EMS_Handle,100);
		}
		else 							  //VEMS��ѹ���� ��λ���ڵ���20����ÿ��һ����0.5V
		{
			Set_Ems_Value(&EMS_Handle,50);
		}

		Set_RF_Level(&SysInfo);
		System_Data_Save();
		System_Sleep();

		 Printf_delay_cnt ++;
		 if(Printf_delay_cnt >= 10)
		  {
		 	Printf_delay_cnt = 0;
			//Write_Parameter_To_Eeprom(); //debug --flash
			//
			// printf ("\n\r Dev_id =0x%0x \n\r", SysInfo.Device_id);
			//hw_2_4G_data.mode
			//printf ("\n\r 2.4G_mode =0x%0x \n\r", hw_2_4G_data.mode);
			// printf ("\n\r skin_adc_val: %d \n\r", RF_Handle.ADC_value);
			// printf ("\n\r Led_Value.Mode %d \n\r", Led_Value.Mode);
			// printf ("\n\r NTC: %0.2f C\n\r", SysInfo.NTC_Temp);
			//printf ("\n\r bat_val =%d mv,BAT_Data.status=%d,State:%d \n\r", BAT_Data.adc_Value * 122/100, BAT_Data.status, SysInfo.Batt_Value.State);

			//printf ("\n\r Power_Display_Flag: %d,Disp_mode: %d\n\r", SysInfo.Batt_Value.Power_Display_Flag, Led_Value.Mode);
			//printf ("\n\r Bat: %f Mv\n\r",SysInfo.Test_Mode.BatValue*1.22);
			// printf ("\n\r SysInfo.state =%d, OverTemp_Flag = %d\r", SysInfo.Power_Value.state, SysInfo.OverTemp_Flag);
		 	//printf ("SysInfo.Sleep_Flag = %d, Idle_Run_Cnt =%d, Power_Value.state=%d \n\r", SysInfo.Sleep_Flag, SysInfo.Idle_Run_Cnt, SysInfo.Power_Value.state);

			//printf ("\n\r skin_ADC :%d \n\r", RF_Handle.ADC_value); //测试

	       	// printf ("\n\r self_cal :%d, offset:%d \n\r", dac_param.self_cal, dac_param.offset); //测试
	        // for(int i = 0; i < 5; i ++)
	        // {
		    //     printf ("\n\r ADC buff[%d] :%d, %d, |%d|, %d, %d\n\r", i, ADC_Value[i][0], ADC_Value[i][1], ADC_Value[i][2], ADC_Value[i][3]); //测试
	        // }


		  }

	}
	Uart_Protocol_Process();
#ifdef ENABLE_FREERTOS_HEAP_STACK_LOG
	UBaseType_t istack1,istack2,istack3;
	size_t FreeHeap,MinEverFreeHeap;

	istack1=uxTaskGetStackHighWaterMark( defaultTaskHandle );
	istack2=uxTaskGetStackHighWaterMark( myTask02Handle);
	istack3=uxTaskGetStackHighWaterMark( myTask03Handle);
	FreeHeap=xPortGetFreeHeapSize();
	MinEverFreeHeap=xPortGetMinimumEverFreeHeapSize();

	//printf1 ("\n\r heap:%d,%d stack:%d,%d,%d\n\r",
	FreeHeap,////��ȡ��ǰδ������ڴ�Ѵ�С
	MinEverFreeHeap,//��ȡδ������ڴ����ʷ��Сֵ
	istack1,istack2,istack3);//����ջ�ռ����ʷ��Сʣ���?,1���ֳ�Ϊ4���ֽ�
#endif
    vTaskDelayUntil(&xLastWakeTime, 25);
  }

  /* USER CODE END StartTask03 */
}
/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

