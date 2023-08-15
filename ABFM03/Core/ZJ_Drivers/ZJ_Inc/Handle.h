#ifndef __HANDLE_H
#define __HANDLE_H

#include "stdint.h"
#include "stm32g0xx_hal_tim.h"

// #define true 1
// #define false 0
typedef enum
{
	True = 0,
	False =1,
}TRUE_FALSE_e;


#define CLI()      __set_PRIMASK(1)
#define SEI()      __set_PRIMASK(0)
#define RF_PERIOD  20
#define ARF001 1    //�»Դｵ��
#define NTC_Compensation_Value 1.8
//#define ARF001 0  //PRO_MAX

//#define Timer_RF  100
//#define Timer_EMS 200
//#define RF_Reminder_Left       210
//#define RF_Reminder_Right      420
//#define RF_Reminder_Forehead   540

//#if ARF001

//#define EMS_Reminder_Left      40
//#define EMS_Reminder_Right     80
//#define EMS_Reminder_Forehead  100
//#define Upkeep_Error_Time  5

//#else

//#define EMS_Reminder_120S  120
//#define EMS_Reminder_240S  240
//#define EMS_Reminder_300S  300

//#define Upkeep_Error_Time  5
//#endif

#define Timer_RF  100
#define Timer_EMS 200

#define RF_Reminder_Left       24000
#define RF_Reminder_Right      48000
#define RF_Reminder_Forehead   60000

#define EMS_Reminder_Left      12000
#define EMS_Reminder_Right     24000
#define EMS_Reminder_Forehead  30000

#define Upkeep_Error_Time  5
/***************����********************/
//#define EMS_Reminder_Left      1200
//#define EMS_Reminder_Right     2400
//#define EMS_Reminder_Forehead  3000


//////////////////////////////////enum//////////////////////////////////////////

//typedef enum
//{
//  Level_None,
//  Level_1,
//  Level_2,
//  Level_3,
//  Level_4,
//  Level_5,
//}_Level_TypeDef;

typedef enum
{
  Batt_Normal,
  Batt_Charging,
  Batt_Low_20,
  Batt_Low_0,
  WAKE_UP_DISP,
  SLEEP_MODE_DISP,
  TEST_MODE_DISP,
  AUTO_MATCH_2_4G,
}_BattState_TypeDef;

typedef enum
{
	BAT_00_00_STATUS	,
	BAT_00_20_STATUS	,
	BAT_20_30_STATUS	,
	BAT_30_40_STATUS	,
	BAT_40_60_STATUS	,
	BAT_60_70_STATUS	,
	BAT_70_80_STATUS	,
	BAT_80_100_STATUS	,
	BAT_100_100_STATUS,
}_Battery_State_TypeDef;

typedef enum
{
  System_OFF,
  System_ON,
  System_Standy,
  System_SHORT_CIRCUIT_PROTECT,
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
  System_Normal,
  System_LowPower_Fault,
  System_OverTemp_Fault,
  System_NTC_Fault,
}_System_Error_TypeDef;

typedef enum
{
  Repair_Duty0= 0,//Repair_Duty0= 6,
/*
  Repair_Duty1= 12,
  Repair_Duty2= 15,
  Repair_Duty3= 18,
*/
  Repair_Duty1= 14,//14,  20230318--�Ļص�ԭ�ȵ�14
  Repair_Duty2= 16,
  Repair_Duty3= 18,
}_Repair_Duty_TypeDef;

typedef enum
{
  UpkeepRF_Duty0=0,//UpkeepRF_Duty0= 6,
  UpkeepRF_Duty1= 15,
  UpkeepRF_Duty2= 17,
  UpkeepRF_Duty3= 19,
}_Upkeep_Duty_TypeDef;

typedef enum
{
  INIT_None,
  INIT_SUCCESS,
  INIT_ERROR,
}_STATE_enum;

typedef enum
{

	Level_None,
	Level_1,
	Level_2,
	Level_3,
	Level_4,
	Level_5,
	Level_6,
	Level_7,
	Level_8,
	Level_9,
	Level_10,
	Level_11,
	Level_12,
	Level_13,
	Level_14,
	Level_15,
	Level_16,
	Level_17,
	Level_18,
	Level_19,
	Level_20,
	Level_21,
	Level_22,
	Level_23,
	Level_24,
	Level_25,
	Level_26,
	Level_27,
	Level_28,
	Level_29,
	Level_30,
}_Level_TypeDef;

typedef enum
{
  Err_None,
	Err_Electric_Low,
	Err_Temp_High,
	Err_Temp_Low,
	Sys_Err,
}_ERR_STATE;

typedef enum
{
	 Attonity,
	 Lock_OFF,
	 Lock_ON,

}_LOCK_STATE;

//////////////////////////////////struct////////////////////////////////////////
typedef struct
{
  uint16_t ADC_BAT_VALUE;
  uint16_t ADC_TEMP_VALUE;
  uint16_t ADC_VEMS_Feedback_VALUE;
  uint16_t ADC_RF_VALUE;
	uint16_t ADC_EMS_DET_VALUE;

  uint32_t BAT_VALUE;
  uint16_t MCU_TEMP;
  uint32_t ADC_COUNT;
}_ADC_VALUE_TypeDef;
typedef struct
{
  uint8_t Power_Display_Flag:1;
	uint8_t Usb_flag:1;
	uint8_t State:6;
	uint16_t BattCount;
}_Batt_VALUE_TypeDef;

// typedef struct
// {
//   uint8_t Get_1ms_Flag:1;
//   uint8_t Get_5ms_Flag:1;
//   uint8_t Get_10ms_Flag:1;
//   uint8_t Get_100ms_Flag:1;
// }_Timer_VALUE_TypeDef;

typedef struct
{
  uint8_t state:2;
	uint8_t BattState:1;
	uint8_t Enter_Sleep_Flag:2;
}_Power_TypeDef;

typedef struct
{
	uint8_t state:1;
	uint8_t Mode;
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

    // uint32_t save_mode_count;
    uint8_t save_Data_flag:1;
	uint8_t   repair_level:3;
	uint8_t   upkeep_level:3;
	uint8_t   WorkState:2;
	uint8_t   BattState:6;
}_Save_Data_TypeDef;

typedef struct
{
  uint8_t Duty;
  uint8_t Period;
  uint8_t Mode:1;
  uint8_t Flag:1;
  uint8_t Run_Flag:1;
  uint8_t Read_ADC_Flag:1;
  uint16_t ADC_value;
//  uint16_t RF_Cnt;
}_RF_HANDLE_TypeDef;

typedef enum{
	NONE_TEST = 0,
	LED_TISHI_TEST,
	TILA_MODE_RF_TEST = 4,
	XIUFU_MODE_RF_TEST = 5,
	TILA_MODE_EMS_TEST = 6,
	WIRELESS_TEST = 7,
	READ_NTC_TEST = 8,
	NTC_WITH_OUTPUT_TEST,  // ntc读取,附带输出，能够快速升温
	KEY_TEST,
	LED_TEST,
	Gsensor_TEST,
	AGING_TEST,      // 老化测试
	INSET_LIFE_TEST,  // 寿命测试--即循环进行老化测试，直至手动退出
	ADJUST_NTC_TEST,  //

}Test_mode_e;


#define AGING_EMS_RF_Continue_CNT   300//5min
#define AGING_EMS_RF_Stop_CNT   	60//1min
#define AGING_TOTAL_Run_CNT   	    3600//1h

typedef struct
{
	uint8_t  Ageing_Flag:1;
	uint8_t  Sleep_Flag:1;

	uint8_t  RF_Flag:1;
	uint8_t  EMS_Flag:1;
	uint8_t  Printf_NTC_Flag:1;
	uint8_t  Ageing_finish_Flag:1;
	uint8_t Aging_run_once_over_flag:1;  // 老化测试,运行一次标记位，

	uint8_t  Test_Mode_Flag;
	uint8_t EMS_RF_out_flag;	// 老化模式，ems_rf输出标志位
	uint8_t Charge_flag;		 // 老化模式，充电状态标志位
	uint8_t display_Charge_flag; // led显示充电状态标志位
	int Ems_RF_run_cnt;   		 // 老化测试射频时长，按 秒计算，
	int EMS_RF_stop_cnt;    	 // 为了防止里面硬件电路，持续输出过热，需要暂停一些时间
	int Charge_cnt;    			 // 老化测试运行时长，按 秒计算，运行20min
	int Ageing_total_run_cnt;    // 老化运行总时长--

	uint8_t  STATUS;
	uint8_t  Set_EMS_Freq;
	uint8_t  Set_RF_Freq;

	uint8_t	 Is_Enter_Ageing_Mode;
	uint8_t  NTC_TARGET;
	uint8_t  ADJUST_NTC_Delay;

	uint16_t  Quit_Test_30s_Cnt;
	// uint16_t  BatValue;
}_TEST_MODE_HANDLE_TypeDef;




typedef struct
{
  uint8_t Duty;
  uint8_t Mode:1;
  uint8_t Flag:1;
  uint8_t Run_Flag:1;
  uint16_t Period;
  uint16_t vol_value;
  uint16_t EMS_Cnt;
  uint16_t TargetVal;
  uint16_t CurrentVal;
}_EMS_HANDLE_TypeDef;

// typedef struct
// {
// 	float Kp;
// 	float Ki;
// 	float Kd;
//   uint8_t Flag;
// 	int TargetVal;
// 	int CurrentVal;

// 	float CurErr;  //��ǰ���
// 	float LastErr; //��һ�����
// 	float PreErr;  //��һһ�����

// 	int16_t temp ;
// 	float OUT;
// 	int16_t Constant ;
// } _PID_HANDLE_TypeDef;


typedef enum{
	OUT_PUT_NONE = 0,
	OUT_PUT_RF = 1,
	OUT_PUT_EMS = 2,
	OUT_PUT_IDLE = 3,
}EMS_RF_OUT_e;



typedef struct
{
	uint8_t   WorkState:2;
	uint8_t   repair_level:3;
	uint8_t   upkeep_level:3;
	uint8_t   EMS_level:3;
	uint8_t   Montor_Flag:1;
	uint8_t   OverTemp_Flag:1;
	uint8_t   Skin_Touch_RF_Flag:1;
	uint8_t	  Skin_Touch_Flag:1;
	uint8_t	  Flag:1;
	uint8_t	  Skin_Touch_Montor_Flag:1;
	uint8_t	  Sleep_Flag:1;
	uint8_t   Ems_ON_Flag:1;
	uint8_t   Heating_Flag:1;
	uint8_t   Receive_Flag:1;
	uint8_t   Mode_Switch_Flag:3;
	uint8_t   System_Err_status:3; //ϵͳ���ϱ�־λ
	uint8_t   MotionStateFlage:1; //���ᴫ�����˶���־

	uint8_t   Freq_Flag;

	uint16_t  StayTime ;
	uint16_t  StayTime_30s_Flag ;
	uint16_t  Sleep_Counts;
	uint16_t  Freq_Cnt;
	uint16_t  Mode_Cnt;
	uint16_t  Reminder_Cnt;
	uint32_t  Idle_Run_Cnt ;
	float     NTC_Temp;
	float     NTC_offset;

	_STATE_enum          		  RGB_SENSER_STATE; //RGB������״̬
	_STATE_enum          		  GYRO_SENSER_STATE;//������״̬
	_LOCK_STATE                   Lock_State;
	_ERR_STATE                 	  Err_State;
	_ADC_VALUE_TypeDef            ADC_Value;
	// _Timer_VALUE_TypeDef		  Timer_Value;
	_MONTOR_VALUE_TypeDef         Montor_Value;
	_Power_TypeDef                Power_Value;
	_Save_Data_TypeDef            Save_Data;
	_Batt_VALUE_TypeDef           Batt_Value;
	_TEST_MODE_HANDLE_TypeDef     Test_Mode;
	uint32_t  Device_id;
	uint8_t   handle_WDT_Flag;


}_Sys_Info_TypeDef;


/////////////////////////////////extern/////////////////////////////////////////
extern TIM_HandleTypeDef  htim1;
extern TIM_HandleTypeDef  htim3;
extern _Sys_Info_TypeDef  SysInfo;
extern _LED_VALUE_TypeDef Led_Value;
extern _IRLED_VALUE_TypeDef IRled_Value;
extern _RF_HANDLE_TypeDef RF_Handle;
extern _EMS_HANDLE_TypeDef EMS_Handle;
// extern _PID_HANDLE_TypeDef PID;
extern uint32_t adcbuf[5];
void Led_Process_Run(void);
void RF_Procedure_Run(void);
void System_Sleep(void);
void RF_Mode_Run(void);
void System_Data_Save(void);
void System_Standby_Check(void);
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
void System_Error_Deal(void);
void SLIDE_MODE_Run(void);
void System_Run_Procedure(void);
////////////////////////////////////////////////////////////////////////////////

#endif
