#ifndef __HANDLE_H
#define __HANDLE_H

#include "stm32g0xx_hal.h"

//���ʹ��翪��
#define LOAD_BUS_SW_ON          HAL_GPIO_WritePin(LOAD_BUS_SW_GPIO_Port,LOAD_BUS_SW_Pin,GPIO_PIN_SET)
#define LOAD_BUS_SW_OFF         HAL_GPIO_WritePin(LOAD_BUS_SW_GPIO_Port,LOAD_BUS_SW_Pin,GPIO_PIN_RESET)
//ϵͳ���翪��
#define SYS_BUS_SW_ON           HAL_GPIO_WritePin(SYS_BUS_SW_GPIO_Port,SYS_BUS_SW_Pin,GPIO_PIN_SET)
#define SYS_BUS_SW_OFF          HAL_GPIO_WritePin(SYS_BUS_SW_GPIO_Port,SYS_BUS_SW_Pin,GPIO_PIN_RESET)
//��س��ʹ��
#define BAT_CHARGE_ENABLE       HAL_GPIO_WritePin(CHARGE_EN_GPIO_Port,CHARGE_EN_Pin,GPIO_PIN_RESET)
#define BAT_CHARGE_DISABLE      HAL_GPIO_WritePin(CHARGE_EN_GPIO_Port,CHARGE_EN_Pin,GPIO_PIN_SET)
//USB ����͵�Դ����ת�� 
#define USB_BUS_SW_ON           HAL_GPIO_WritePin(USB_BUS_SW_GPIO_Port,USB_BUS_SW_Pin,GPIO_PIN_SET)
#define USB_BUS_SW_OFF          HAL_GPIO_WritePin(USB_BUS_SW_GPIO_Port,USB_BUS_SW_Pin,GPIO_PIN_RESET)
//���������翪��
#define SENSOR_PWR_SW_ON        HAL_GPIO_WritePin(SENSOR_PWR_SW_GPIO_Port,SENSOR_PWR_SW_Pin,GPIO_PIN_RESET)
#define SENSOR_PWR_SW_OFF       HAL_GPIO_WritePin(SENSOR_PWR_SW_GPIO_Port,SENSOR_PWR_SW_Pin,GPIO_PIN_SET)
//������ƿ���
#define BACKLIGHT_ON            HAL_GPIO_WritePin(BACKLIGHT_EN_GPIO_Port,BACKLIGHT_EN_Pin,GPIO_PIN_SET)
#define BACKLIGHT_OFF           HAL_GPIO_WritePin(BACKLIGHT_EN_GPIO_Port,BACKLIGHT_EN_Pin,GPIO_PIN_RESET)
//��������  �͵�ƽ����  �ߵ�ƽ����
#define B_LIGHT_LV_UP           HAL_GPIO_WritePin(B_LIGHT_LV_GPIO_Port,B_LIGHT_LV_Pin,GPIO_PIN_SET)//
#define B_LIGHT_LV_DOWN         HAL_GPIO_WritePin(B_LIGHT_LV_GPIO_Port,B_LIGHT_LV_Pin,GPIO_PIN_RESET)
//RF���ʹ��
#define RF_OUTPUT_ENABLE        HAL_GPIO_WritePin(RF_OUT_SW_GPIO_Port,RF_OUT_SW_Pin,GPIO_PIN_SET)
#define RF_OUTPUT_DISABLE       HAL_GPIO_WritePin(RF_OUT_SW_GPIO_Port,RF_OUT_SW_Pin,GPIO_PIN_RESET)
//EMS���ʹ��
#define EMS_OUTPUT_ENABLE       HAL_GPIO_WritePin(EMS_OUT_SW_GPIO_Port,EMS_OUT_SW_Pin,GPIO_PIN_SET)
#define EMS_OUTPUT_DISABLE      HAL_GPIO_WritePin(EMS_OUT_SW_GPIO_Port,EMS_OUT_SW_Pin,GPIO_PIN_RESET)
//MOTOʹ��
#define MOTO_ENABLE             HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port,MOTOR_EN_Pin,GPIO_PIN_SET)
#define MOTO_DISABLE            HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port,MOTOR_EN_Pin,GPIO_PIN_RESET)

//ovp Ӱ���繦�ܣ����ʱ����enable
#define OVP_ENABLE				HAL_GPIO_WritePin(OVP_CTRL_GPIO_Port, OVP_CTRL_Pin, GPIO_PIN_RESET);
#define OVP_DISABLE				HAL_GPIO_WritePin(OVP_CTRL_GPIO_Port, OVP_CTRL_Pin, GPIO_PIN_SET);

typedef enum
{
  MACHINEOFF,
  MACHINEON,
  CHECK_BATTER,
}_sys_state_e;

typedef enum
{
  SEAL_MODE,     //����ģʽ
  SLIDE_MODE,    //����ģʽ
}_choice_mode_e;

typedef enum
{
  RF_WITH_EMS  = 0,
  RF_WITHOUT_EMS = 1,
  //  RF_OVER  = 30, //����EMS �������
  //  EMS_OVER = 31, //��ems �������  
}_rf_ems_out_e;

typedef enum
{
  No_CHARGE_STA,
  CHARGING_STA,
}_charge_state_e;

typedef enum
{
  GEARONE,
  GEARTWO,
  GEARTHREE,
  //GearsMAX,
}_gear_level_e;

typedef enum
{
  NoHEAT,
  OverHEAT,
}_temp_protect_e;

typedef enum
{
  NORMAL,
  CALIBRATION,
  SYS_ERROR,
  TESTMODE,
}_work_mode_e;

typedef enum
{
	Cycing_TestMode,
  Aging_TestModel,
  Life_TestModel,
	Auto_TestModel,
}_test_mode_e;

typedef enum
{
  INIT_None,
  INIT_SUCCESS,
  INIT_ERROR,
}_init_result_e;

typedef enum
{
  Light_Off,
  Light_Long,
  Light_Twinkle,
}_BL_state_e;

typedef union
{
  uint16_t error;
  struct
  {
    uint8_t gyro_error_flag :1;
    uint8_t rgb_error_flag  :1;
    uint8_t head_ntc_error_flag :1;
    uint8_t batt_ntc_error_flag :1;
    uint8_t batt_value_error_flag :1;
    uint8_t usb_value_error_flag:1;
    uint8_t head_short_error_falg:1;
    uint8_t ems_value_error_flag:1;
    uint8_t rf_value_error_flag:1;
  }bit;
}_err_data_t;

typedef struct
{ 
  _choice_mode_e          Mode_choice_key; //����ģʽ
  //_gear_level_e        Gears_key;     //��λ����
  _gear_level_e        Slide_gear;     //����ģʽ��λ
  _gear_level_e        Seal_gear;      //����ģʽ��λ
  _rf_ems_out_e        Mode_key;       //����ģʽEMS����
  uint8_t               Bus_Power_Up_Flag;
}_key_state_t;

//typedef enum
//{       
//  batter_power_on,
//  batter_power_off,
//  batter_check_quantity,
//  usb_power_on,
//  usb_power_off,
//  usb_power_off_sys,
//}_PowerBus_Stare_Enum;

typedef struct
{
	uint8_t							 chargetimes;
	uint8_t							 TestMode;
	uint8_t              Test_charge_Flag:1;
	uint8_t              First_charge_Flag:1;
	uint8_t							 Receive_Flag:1;
	uint8_t							 Send_Flag:1;
	uint8_t							 RF_EMSTEST_Flag:1;
}_test_mode_t;


typedef struct
{
	_test_mode_t	 	  Test_Mode;
  _err_data_t       Err_State;
  _key_state_t      Key_State;
  _work_mode_e      Work_Mode;
  _sys_state_e      Power_State;      //��Դ״̬
  _charge_state_e   Charge_State;     //���״̬
  _temp_protect_e   Temp_Protect;     //����ͷ���±�־
  _init_result_e    RGB_sensor_state; //RGB������״̬
  _init_result_e    G_sensor_state;   //������״̬
  _BL_state_e       BKlight_State;    //���״̬
  uint8_t           Slide_effective_flag:1;//����ģʽ��Чʱ���־
  uint8_t           G_sensor_active_flage:1;      //���ᴫ�����˶���־
	uint8_t						timestart_cnt;
  uint8_t           Adc_First_Run_Flage:1;       //ADC��һ��ȡֵ
	uint8_t						Chargestop_Flag:1;
	uint8_t						TestMode_RF_Fre:3;        //���ڲ���RFƵ��
  int16_t           Head_Temperature;         //����ͷNTC�¶�ֵ
	int16_t           Test_onff_value;         //���ڲ��Կ���ֵ
	int16_t           KEY_Value;               //���ڲ��԰���ֵ
  uint32_t          SealMode_Conter;         //����ģʽLoop��ʱ
  uint32_t          SildeMode_Conter;        //����ģʽ��ʱ
  uint32_t          PowerOn_Time_Counter;    //������ʱ   10ms
  uint32_t          PowerOff_Time_Counter;   //�ػ���ʱ   10ms
}_sys_Info_t;

typedef enum                 //ADC˳��ɨ��
{
  BAT_ADC_AN,
  USB_IN_ADC_AN,
  EMS_U_ADC_AN,
  RF_U_ADC_AN,
  RF_I_ADC_AN,
  NTC_R_ADC_AN,
  BAT_TEMP_ADC_AN,
  MCU_VREF,                 //STM32G0 �ڲ��ο����ӵ�ADC_CH13
  _CH_MAX,
}_adc_chanel_index_e;

typedef struct
{
  uint16_t bat_val;
  uint16_t Usb_Val;
  uint16_t RF_U_Val;
  uint16_t RF_I_Val;
  uint16_t EMS_U_Val;
  uint16_t RF_Temp_Val;
  uint16_t bat_Temp_Val;
  uint16_t Mcu_Vref_Val;	
}ADC_data_t;

typedef enum
{
  Motor_Start_None,
  Motor_Start_Power,
  Motor_Start_Key,
  Motor_Start_Color,
  Motor_Start_Time,
}_Motor_Start_e;

typedef enum
{
  Motor_Off,
  Motor_On,
}_Motor_State_e;

typedef struct
{
  _Motor_Start_e motor_mode;
  _Motor_State_e motor_sta;
}_Moto_Typedef;

#define ADC_CHANNLE_NUM	_CH_MAX
#define ADC_ADD_COUNT	5

#define Temp_Protect_Value  45

extern uint16_t Adc_Value[ADC_ADD_COUNT][ADC_CHANNLE_NUM];

extern ADC_data_t       ADC_Data;
extern _sys_Info_t      Sys_Info;


extern void SYSTEM_Init_Function(void);
extern void ADC_DMA_COV_START(void);
extern void Charge_Insertion_Status(void);
extern void Sys_Power_On(void);
extern void Sys_Power_Off(void);
extern void Control_Funtion(void);
extern uint8_t loadCurrent(void);
extern void MOTO_Handle(void);
extern void System_Error_Check_Funtion(void);
extern void Backlight_Display_Funtion(void);
extern void System_Error_Handle(void);

#endif

