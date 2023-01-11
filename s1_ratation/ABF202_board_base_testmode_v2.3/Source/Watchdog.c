#include "includes.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

//--------------------系统看门狗+任务看门狗----------------------//
//---------------------------------------------------------------//
TaskHandle_t os_watchdog_handle;
extern IWDG_HandleTypeDef hiwdg;

typedef struct
{
  TaskHandle_t handle; // 任务句柄
  const char *name;    // 任务看门狗名称
  uint16_t timeout;    // 任务看门狗超时  
  uint16_t time;       // 任务看门狗计数
  uint8_t  suspended;  // 任务看门狗暂停  
} watchdog_t;

static SemaphoreHandle_t os_watchdog_lock;// 互斥信号量

#define OS_PRIO_MAX             3
#define WATCHDOG_LOCK()         xSemaphoreTake(os_watchdog_lock, 100);
#define WATCHDOG_UNLOCK()       xSemaphoreGive(os_watchdog_lock)
#define WATCHDOG_CHECK_INTERVAL 100  // 100ms喂一次狗
#define WATCHDOG_POOL_SIZE      (OS_PRIO_MAX)// + OS_PRIO_MAX - 1) // 任务池 2倍的任务-1
#define OS_WATCHDOG_STK_SIZE    64   
#define OS_PRIO_WATCHDOG        osPriorityNormal

static watchdog_t watchdogs[WATCHDOG_POOL_SIZE];
/*************************************************************************************
* FunctionName	: WatchdogFeed
* Description   : 喂狗
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
* Description   : 看门狗线程  额外占用26bytes堆栈空间
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
* Description   : 初始化独立看门狗
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void WatchdogInit(void)
{
#ifdef IWDG_ENABLE
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_8;
  hiwdg.Init.Reload = 4000;  //1000ms
  hiwdg.Init.Window = IWDG_WINDOW_DISABLE;//IWDG_WINDOW_DISABLE; // 关闭窗口功能
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
* Description   : 任务看门狗注册
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
* Description   : 任务看门狗喂狗
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
* Description   : 任务看门狗暂停
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
* Description   : 任务看门狗恢复
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


