#ifndef __HANDLE_H
#define __HANDLE_H

#include "stdint.h"
#include "stm32g0xx_hal_tim.h"
#define true 1
#define false 0
#define CLI()      __set_PRIMASK(1)
#define SEI()      __set_PRIMASK(0)
#define RF_PERIOD  20
//#define ARF001 1    // pro版本
#define DEVICE_R1_RPO_MAX  0    //宏定义 r1_pro_max = 0, 未修改逻辑
#define DEVICE_R1_RPO      1    //宏定义 r1_pro = 1,     未修改逻辑

#define ARF001            DEVICE_R1_RPO_MAX  //PRO_MAX

#define UNUSE_G_SENSOR    0
#define USE_G_SENSOR      1
#define G_SENSOR_SELECT   UNUSE_G_SENSOR

#if (G_SENSOR_SELECT == USE_G_SENSOR)
#define LIS2DH           1  //g_sensor device
#else
#define LIS2DH           0  //g_sensor device
#endif

//#define LIS3DH 0  //3D������
#define ENABLE_FREERTOS_HEAP_STACK_LOG 0
#define DEBUG_IDWG 1
#define AGEING_TEST 0  //������������

#define TEMP_TEST_MODE 1
#define SENSOR_MOVE 1

#define Timer_RF  100
#define Timer_EMS 200
#define RF_Reminder_240S  240
#define RF_Reminder_480S  480
#define RF_Reminder_600S  600

#if ARF001

#define EMS_Reminder_120S  40
#define EMS_Reminder_240S  80
#define EMS_Reminder_300S  100
#define Upkeep_Error_Time  5

#else

#define EMS_Reminder_120S  120
#define EMS_Reminder_240S  240
#define EMS_Reminder_300S  300

#define Upkeep_Error_Time  5
#endif
//////////////////////////////////enum//////////////////////////////////////////

typedef enum
{
  Level_None,
  Level_1,
  Level_2,
  Level_3,
  Level_4,
  Level_5,
}_Level_TypeDef;

typedef enum
{
  Batt_Normal,
  Batt_Charging,
  Batt_Low_20,
  Batt_Low_0,
}_BattState_TypeDef;

typedef enum
{
	BAT_00_00_STATUS	,
	BAT_00_20_STATUS	,
	BAT_20_40_STATUS	,
	BAT_40_60_STATUS	,
	BAT_60_80_STATUS	,
	BAT_80_100_STATUS	,
	BAT_100_100_STATUS,
}_Battery_State_TypeDef;

typedef enum
{
  System_OFF,
  System_ON,
  System_Standy,
}_System_State_TypeDef;

typedef enum
{
  OFF,
  ON,
  Auto_ON,
  Standy,
}_TestMode_State_TypeDef;

typedef enum
{
	repair_mode,
	upkeep_mode,
	Standy_mode,
}_work_status_TypeDef;

typedef enum
{
  Power_OFF,
  Power_ON,
  Power_STANDBY,
}_System_Power_TypeDef;

typedef enum
{
#if ARF001
  Repair_Duty0= 10,
  Repair_Duty1= 12,
  Repair_Duty2= 14,
  Repair_Duty3= 15,
  Repair_Duty4= 16,
  Repair_Duty5= 17,
#else
  Repair_Duty0= 10,
  Repair_Duty1= 13,
  Repair_Duty2= 14,
  Repair_Duty3= 19,
  Repair_Duty4= 19,
  Repair_Duty5= 19,
#endif
}_Repair_Duty_TypeDef;

typedef enum
{
  Upkeep_Duty0= 10,
  Upkeep_Duty1= 12,
  Upkeep_Duty2= 14,
  Upkeep_Duty3= 15,
  Upkeep_Duty4= 16,
  Upkeep_Duty5= 17,
}_Upkeep_Duty_TypeDef;

//////////////////////////////////struct////////////////////////////////////////
typedef struct
{
  uint16_t ADC_BAT_VALUE;
  uint16_t ADC_TEMP_VALUE;
  uint16_t ADC_VEMS_Feedback_VALUE;
  uint16_t ADC_RF_VALUE;
	uint16_t ADC_EMS_DET_VALUE;
  uint16_t MCU_TEMP;
  uint32_t BAT_VALUE;
  uint32_t ADC_COUNT;
}_ADC_VALUE_TypeDef;
typedef struct
{
  uint8_t Power_Display_Flag:1;
	uint8_t Usb_flag:1;
	uint8_t State:3;
	uint16_t BattCount;
}_Batt_VALUE_TypeDef;

typedef struct
{
  uint8_t Get_1ms_Flag:1;
  uint8_t Get_5ms_Flag:1;
  uint8_t Get_10ms_Flag:1;
  uint8_t Get_100ms_Flag:1;
}_Timer_VALUE_TypeDef;

typedef struct
{
  uint8_t state:2;
	uint8_t BattState:1;
	uint8_t Enter_Sleep_Flag:2;
}_Power_TypeDef;

typedef struct
{
	uint8_t state:1;
	uint8_t Mode:2;
  uint8_t Level;
	uint8_t Counts;
	uint16_t StayTime;
}_LED_VALUE_TypeDef;

typedef struct
{
	uint8_t state:1;
	uint16_t Bright_Value;
	uint16_t Target_Value;
}_IRLED_VALUE_TypeDef;

typedef struct
{
	uint8_t state:1;
}_MONTOR_VALUE_TypeDef;

typedef struct
{
  uint8_t save_Data_flag:1;
	uint8_t   repair_level:3;
	uint8_t   upkeep_level:3;
	uint8_t   WorkState:2;
	uint8_t   BattState:3;
  uint32_t save_mode_count;
}_Save_Data_TypeDef;

typedef struct
{
  uint8_t Duty;
  uint8_t Period;
  uint8_t Mode:1;
  uint8_t Flag:1;
  uint8_t Run_Flag:1;
  uint8_t ADC_Flag:1;
//  uint16_t RF_Cnt;
}_RF_HANDLE_TypeDef;

typedef struct
{
  uint8_t  Quit_Flag:1;
  uint8_t  Ageing_Flag:1;
  uint8_t  Auto_Mode_Flag:1;
  uint8_t  Sleep_Flag:1;
  uint8_t  Test_Mode:4;

  uint8_t  Test_Mode_Flag:2;
  uint8_t  Level:3;
  uint8_t  Ageing_Mode:2;
  uint8_t  Send_Flag:1;

  uint8_t  Twink_Flag:4;
  uint8_t  Lock_Flag:4;

  uint8_t  GSensor_Flag:1;

  uint8_t  Set_EMS_Freq;
  uint8_t  Set_RF_Freq;

  uint8_t  Temp_level;
  uint8_t  Auto_Mode_Level;

  uint16_t Quit_Test_30s_Cnt;
  uint16_t Quit_Test_60s_Cnt;

  uint16_t BatValue;
  uint16_t Auto_Mode_Cnt;

  uint8_t  Data[20];

  uint32_t	AgeTimer_Cnt;
  uint32_t Quit_Test_300s_Cnt;
}_TEST_MODE_HANDLE_TypeDef;

typedef struct
{
  uint8_t Mode:1;
  uint8_t Flag:1;
  uint8_t Run_Flag:1;
  uint8_t Duty;
  uint16_t Period;
  uint16_t vol_value;
  uint16_t EMS_Cnt;
}_EMS_HANDLE_TypeDef;

typedef struct
{
	float Kp;
	float Ki;
	float Kd;
  uint8_t Flag;
	int TargetVal;
	int CurrentVal;

	float CurErr;  //��ǰ���
	float LastErr; //��һ�����
	float PreErr;  //��һһ�����

	int16_t temp ;
	float OUT;
	int16_t Constant ;
} _PID_HANDLE_TypeDef;

typedef struct
{
	uint8_t   WorkState:2;
	uint8_t   repair_level:3;
	uint8_t   upkeep_level:3;

	uint8_t   EMS_level:3;
	uint8_t   Montor_Flag:1;
	uint8_t   OverTemp_Flag:2;
	uint8_t   Skin_Touch_RF_Flag:1;
	uint8_t		Skin_Touch_Flag:1;

	uint8_t		Flag:1;
	uint8_t		Skin_Touch_Montor_Flag:1;
	uint8_t		Sleep_Flag:1;
	uint8_t   Ems_ON_Flag:1;
	uint8_t   Period_Flag:1;         //���ڱ�־λ

	uint8_t   Heating_Flag:3;
	uint8_t   Receive_Flag:2;
	uint8_t   Mode_Switch_Flag:3;

	uint8_t   MotionStateFlage:1;    //���ᴫ�����˶���־
	uint8_t   Check_Protect_Flage:1; //�ƶ���Ᵽ��
	uint8_t   Temp_Protect_Flage:5;  //����ֹͣ����
	uint8_t   Restore_Flag:1;

	float   NTC_Temp;
	uint16_t  StayTime ;
	uint16_t  StayTime_30s_Flag ;
	uint16_t  Sleep_Counts;
	uint16_t  Freq_Cnt;
	uint16_t  Mode_Cnt;
	uint16_t  Reminder_Cnt;
	uint32_t  Skin_No_Touch_Timer ;

	_ADC_VALUE_TypeDef            ADC_Value;
	_Timer_VALUE_TypeDef		  Timer_Value;
	_MONTOR_VALUE_TypeDef         Montor_Value;
	_Power_TypeDef                Power_Value;
	_Save_Data_TypeDef            Save_Data;
	_Batt_VALUE_TypeDef           Batt_Value;
	_TEST_MODE_HANDLE_TypeDef     Test_Mode;

}_Sys_Info_TypeDef;


/////////////////////////////////extern/////////////////////////////////////////
extern TIM_HandleTypeDef  htim1;
extern TIM_HandleTypeDef  htim3;
extern _Sys_Info_TypeDef  SysInfo;
extern _LED_VALUE_TypeDef Led_Value;
extern _IRLED_VALUE_TypeDef IRled_Value;
extern _RF_HANDLE_TypeDef RF_Handle;
extern _EMS_HANDLE_TypeDef EMS_Handle;
extern _PID_HANDLE_TypeDef PID;
extern uint32_t adcbuf[5];
void Led_Process_Run(void);
void RF_Procedure_Run(void);
void System_Sleep(void);
void RF_Mode_Run(void);
void System_Data_Save(void);
void System_Standby_Run(void);
void EMS_Procedure_Run(void);
void Enter_Sleep_Mode(void);
void exit_sleep_mode(void);
void System_1mS_Procedure(void);
void System_5mS_Procedure(void);
void System_10mS_Procedure(void);
void System_100mS_Procedure(void);
void RF_Call_Data(void);
void Rx_Init(void);
void System_Data_Init(void);
void Vibration_Reminder_Counts_Run(void) ;
void Set_OverTemp_Parameter(void);
void SLIDE_MODE_Run(void);
////////////////////////////////////////////////////////////////////////////////

#endif
