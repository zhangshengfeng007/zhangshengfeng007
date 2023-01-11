#include "includes.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

//--------------------ϵͳ���Ź�+�����Ź�----------------------//
//---------------------------------------------------------------//
TaskHandle_t os_watchdog_handle;
extern IWDG_HandleTypeDef hiwdg;

typedef struct
{
  TaskHandle_t handle; // ������
  const char *name;    // �����Ź�����
  uint16_t timeout;    // �����Ź���ʱ  
  uint16_t time;       // �����Ź�����
  uint8_t  suspended;  // �����Ź���ͣ  
} watchdog_t;

static SemaphoreHandle_t os_watchdog_lock;// �����ź���

#define OS_PRIO_MAX             3
#define WATCHDOG_LOCK()         xSemaphoreTake(os_watchdog_lock, 100);
#define WATCHDOG_UNLOCK()       xSemaphoreGive(os_watchdog_lock)
#define WATCHDOG_CHECK_INTERVAL 100  // 100msιһ�ι�
#define WATCHDOG_POOL_SIZE      (OS_PRIO_MAX)// + OS_PRIO_MAX - 1) // ����� 2��������-1
#define OS_WATCHDOG_STK_SIZE    64   
#define OS_PRIO_WATCHDOG        osPriorityNormal

static watchdog_t watchdogs[WATCHDOG_POOL_SIZE];
/*************************************************************************************
* FunctionName	: WatchdogFeed
* Description   : ι��
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void WatchdogFeed(void)
{
#ifdef IWDG_ENABLE
  HAL_IWDG_Refresh(&hiwdg);
#endif
}
/*************************************************************************************
* FunctionName	: task_watchdog
* Description   : ���Ź��߳�  ����ռ��26bytes��ջ�ռ�
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
//uint32_t task1_stack_size4 ;
static void task_watchdog(void *argument)
{
  int i;
  for(;;)
  {
    WatchdogFeed();

    //task1_stack_size4 = uxTaskGetStackHighWaterMark(NULL);
    
    for(i=0; i < WATCHDOG_POOL_SIZE; ++i)
    {
      if(0 == watchdogs[i].handle)
      {
        continue;
      }
      
      if(!watchdogs[i].suspended && watchdogs[i].time)
      {
        --(watchdogs[i].time);
      }
      
      if(!watchdogs[i].suspended && (0 == watchdogs[i].time))
      {
        while(1);  // reset
      }      
    }    
    osDelay(WATCHDOG_CHECK_INTERVAL);
  }
}

/*************************************************************************************
* FunctionName	: WatchdogInit
* Description   : ��ʼ���������Ź�
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void WatchdogInit(void)
{
#ifdef IWDG_ENABLE
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_8;
  hiwdg.Init.Reload = 4000;  //1000ms
  hiwdg.Init.Window = IWDG_WINDOW_DISABLE;//IWDG_WINDOW_DISABLE; // �رմ��ڹ���
  HAL_IWDG_Init(&hiwdg);
#endif
  os_watchdog_lock = xSemaphoreCreateMutex();
  
  // xTaskCreate((TaskFunction_t)task_watchdog,
  //             (const char*   )"task_watchdog",
  //             (uint16_t      )OS_WATCHDOG_STK_SIZE,
  //             (void*         )NULL,
  //             (UBaseType_t   )OS_PRIO_WATCHDOG,
  //             (TaskHandle_t* )&os_watchdog_handle);
}
/*************************************************************************************
* FunctionName	: WatchdogRegister
* Description   : �����Ź�ע��
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void WatchdogRegister(const char *name, uint16_t timeout)
{
  int i;
  
  TaskHandle_t handle = xTaskGetCurrentTaskHandle();

  WATCHDOG_LOCK();
  
  for(i = 0; i < WATCHDOG_POOL_SIZE; ++i)
  {
    if (handle == watchdogs[i].handle)
    {
      WATCHDOG_UNLOCK();
      return;
    }    
  }
  
  for(i = 0; i < WATCHDOG_POOL_SIZE; ++i)
  {    
    if (0 == watchdogs[i].handle)
    {
      watchdogs[i].handle = handle;
      watchdogs[i].name = name;
      watchdogs[i].timeout = (timeout + WATCHDOG_CHECK_INTERVAL - 1) / WATCHDOG_CHECK_INTERVAL;
      watchdogs[i].time = watchdogs[i].timeout;
      watchdogs[i].suspended = 0;
      break;
    }    
  }  
  WATCHDOG_UNLOCK();
}
/*************************************************************************************
* FunctionName	: WatchdogKick
* Description   : �����Ź�ι��
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void WatchdogKick(void)
{
  int i;
  
  TaskHandle_t handle = xTaskGetCurrentTaskHandle();
  
  for(i = 0; i < WATCHDOG_POOL_SIZE; ++i)
  {
    if (handle == watchdogs[i].handle)
    {
      WATCHDOG_LOCK();
      watchdogs[i].time = watchdogs[i].timeout;
      WATCHDOG_UNLOCK();
      break;
    }
  }
}
/*************************************************************************************
* FunctionName	: WatchdogSuspend
* Description   : �����Ź���ͣ
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void WatchdogSuspend(void)
{
  int i;
  
  TaskHandle_t handle = xTaskGetCurrentTaskHandle();
  
  for(i = 0; i < WATCHDOG_POOL_SIZE; ++i)
  {
    if (handle == watchdogs[i].handle)
    {
      WATCHDOG_LOCK();
      watchdogs[i].suspended = 1;
      WATCHDOG_UNLOCK();
      break;
    }    
  }  
}
/*************************************************************************************
* FunctionName	: WatchdogResume
* Description   : �����Ź��ָ�
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void WatchdogResume(void)
{
  int i;
  
  TaskHandle_t handle = xTaskGetCurrentTaskHandle();
  
  for(i = 0; i < WATCHDOG_POOL_SIZE; ++i)
  {
    if (handle == watchdogs[i].handle)
    {
      WATCHDOG_LOCK();
      watchdogs[i].time = watchdogs[i].timeout;
      watchdogs[i].suspended = 0;
      WATCHDOG_UNLOCK();
      break;
    }
  }
}


