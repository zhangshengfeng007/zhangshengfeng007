#ifndef __ZJ_LTR831_H
#define	__ZJ_LTR831_H

#include "main.h"

/* LTR-3801 Registers */
#define LTR381_MAIN_CTRL        0x00
#define LTR381_ALS_CS_MEAS_RATE	0x04
#define LTR381_ALS_CS_GAIN      0x05
#define LTR381_PART_ID          0x06
#define LTR381_MAIN_STATUS      0x07
#define LTR381_DATA_IR_0        0x0A
#define LTR381_DATA_IR_1        0x0B
#define LTR381_DATA_IR_2        0x0C
#define LTR381_DATA_GREEN_0     0x0D
#define LTR381_DATA_GREEN_1     0x0E
#define LTR381_DATA_GREEN_2     0x0F

#if 0
#define LTR381_DATA_BLUE_0      0x10
#define LTR381_DATA_BLUE_1      0x11
#define LTR381_DATA_BLUE_2      0x12
#define LTR381_DATA_RED_0       0x13
#define LTR381_DATA_RED_1       0x14
#define LTR381_DATA_RED_2       0x15
#else
#define LTR381_DATA_RED_0       0x10
#define LTR381_DATA_RED_1       0x11
#define LTR381_DATA_RED_2       0x12
#define LTR381_DATA_BLUE_0      0x13
#define LTR381_DATA_BLUE_1      0x14
#define LTR381_DATA_BLUE_2      0x15
#endif

#define LTR381_INT_CFG          0x19
#define LTR381_INT_PST          0x1A
#define LTR381_ALS_THRES_UP_0   0x21
#define LTR381_ALS_THRES_UP_1   0x22
#define LTR381_ALS_THRES_UP_2   0x23
#define LTR381_ALS_THRES_LOW_0	0x24
#define LTR381_ALS_THRES_LOW_1	0x25
#define LTR381_ALS_THRES_LOW_2	0x26

/* Basic Operating Modes */
#define MODE_ALS_Range1         0x00  ///for als gain x1
#define MODE_ALS_Range3         0x01  ///for als gain x3
#define MODE_ALS_Range6         0x02  ///for als gain x6
#define MODE_ALS_Range9         0x03  ///for als gain x9
#define MODE_ALS_Range18        0x04  ///for als gain x18

#define ALS_RANGE_1             1
#define ALS_RANGE_3             3
#define ALS_RANGE_6             6
#define ALS_RANGE_9             9
#define ALS_RANGE_18            18

#define CTRL_SW_RESET           0x10
#define CTRL_CS_MODE_ALS        0x00
#define CTRL_CS_MODE_CS         0x04
#define CTRL_ALS_CS_EN          0x02

#define ALS_RESO_MEAS           0x22

#define ALS_MIN_VAL             0x0
#define ALS_MAX_VAL             0xFFFFF

#define ALS_CS_RES_16BIT        0x40
#define ALS_CS_RES_17BIT        0x30
#define ALS_CS_RES_18BIT        0x20
#define ALS_CS_RES_19BIT        0x10
#define ALS_CS_RES_20BIT        0x00

#define ALS_CS_MEAS_25MS        0x00
#define ALS_CS_MEAS_50MS        0x01
#define ALS_CS_MEAS_100MS       0x02
#define ALS_CS_MEAS_200MS       0x03
#define ALS_CS_MEAS_500MS       0x04
#define ALS_CS_MEAS_1000MS	0x05
#define ALS_CS_MEAS_2000MS	0x06

#define LTR381_SUCCESS          0
#define LTR381_ERR_I2C          -1
#define LTR381_ERR_STATUS       -3
#define LTR381_ERR_SETUP_FAILURE  -4
#define LTR381_ERR_GETGSENSORDATA -5
#define LTR381_ERR_IDENTIFICATION -6

//#define LTR831_ADDRESS   0x53 // (0x53 <<1)
#define LTR831_R_ADDRESS 0xA7 // (0x53 <<1)
#define LTR831_W_ADDRESS 0xA6 // (0x53 <<1)

#define WFAC 1

typedef enum
{
  COLOUR_ORANGE = 1, //橙色
  COLOUR_GREEN  = 2, //绿色
  COLOUR_BLUE   = 3, //蓝色
  COLOUR_BROWN  = 4, //黄色
  COLOUR_OTHER  = 20,
  READ_ERR = 99,
}_COLOUR_Enum_Typedef;

typedef struct
{
  float Calibration_R;
  float Calibration_G;
  float Calibration_B;
  float Calibration_Lux;
  uint8_t Calibration_Data_Flag;
}_White_Balance_Typedef;

typedef struct
{
  uint32_t  IR;      //[0-65536]传感器值
  uint32_t  r;
  uint32_t  g;
  uint32_t  b;
  
  float R_Factor;    //比例因子
  float B_Factor;
  
  uint32_t  RGB_R;   //校准值
  uint32_t  RGB_G;
  uint32_t  RGB_B;
  uint32_t  RGB_Lux;

  uint32_t RGB_Average; //均值
  uint32_t RGB_Peak;    //最大值
  uint32_t Cct;         //色温
  uint32_t Lux_Calc;    //照度
  
  uint16_t R_SCALED;     //比例值
  uint16_t G_SCALED;
  uint16_t B_SCALED;
  uint16_t Lux_SCALED;
  
  _COLOUR_Enum_Typedef Colour_Type;
  
}_COLOR_RGBC_Typedef;//RGBC

typedef enum//														num30
{
  RGB_ORANGE_R_MAX  = 255,  //橙色				R_Factor:0.6    0.7   0.8
  RGB_ORANGE_R_MIN  = 255,
  RGB_ORANGE_G_MAX  = 150,   //org120			B_Factor:3.2		2.3		1.8
  RGB_ORANGE_G_MIN  = 80,
  RGB_ORANGE_B_MAX  = 70,
  RGB_ORANGE_B_MIN  = 20,//25 org30
  
  RGB_BLUE_R_MAX    = 180,  //蓝色 140 org120		R_Factor:3.2
  RGB_BLUE_R_MIN    = 60,
  RGB_BLUE_G_MAX    = 210,//180		org160				B_Factor:0.5
  RGB_BLUE_G_MIN    = 100,
  RGB_BLUE_B_MAX    = 255,
  RGB_BLUE_B_MIN    = 255,
  
  RGB_GREEN_R_MAX   = 200,  //绿色 	180					R_Factor:3.3 		4.6
  RGB_GREEN_R_MIN   = 90,//100 org110					B_Factor:1.5		1.8
  RGB_GREEN_G_MAX   = 255,
  RGB_GREEN_G_MIN   = 255,
  RGB_GREEN_B_MAX   = 220,//200
  RGB_GREEN_B_MIN   = 125,//135		org150
  
  RGB_BROWN_R_MAX   = 255,  //棕色				R_Factor:1.2     1.3
  RGB_BROWN_R_MIN   = 255,	//						B_Factor:2.1		 1.8
  RGB_BROWN_G_MAX   = 220,
  RGB_BROWN_G_MIN   = 160,
  RGB_BROWN_B_MAX   = 200,//180
  RGB_BROWN_B_MIN   = 71,//71   org     
  
}_RGB_COMP_Typedef;

typedef enum
{
  ORANGE_Lux_Calc_Min  = 11,//4000,  20  13		org22
  ORANGE_Lux_Calc_Max  = 45,//5500,
  
  BLUE_Lux_Calc_Min    = 12,//4000,  12		org18
  BLUE_Lux_Calc_Max    = 45,//35,
  
  GREEN_Lux_Calc_Min   = 12,//500,		12		org20
  GREEN_Lux_Calc_Max   = 40,//1500,
  
  BROWN_Lux_Calc_Min   = 9,//500,	11		org13
  BROWN_Lux_Calc_Max   = 35,//1500,
  
}_LUX_MAX_MIN_Typedef;


typedef enum
{
  _No_Key,
  _Pow_Key,
  _Gear_Key
}_Key_Enum;

typedef struct 
{
  uint8_t set_power_key_flag;
  uint8_t set_gears_key_flag;
  uint8_t Calibraton_Start_flag;
  uint8_t Calibraton_Start_Cnt;
  uint32_t timer1_cnt;
  uint32_t timer2_cnt;
  float Calibraton_R_Avg;
  float Calibraton_B_Avg;
  float Calibraton_Lux_Avg;
}_Calibraton_Key;

extern _COLOR_RGBC_Typedef RGB_Handle;
extern _White_Balance_Typedef  White_Balance;
extern _Calibraton_Key  				Cal_Key;

#define EVAL_I2Cx_TIMEOUT_MAX  10

#define RGB_BACKLIGHT_ON     HAL_GPIO_WritePin(C_SENSOR_LED_GPIO_Port,C_SENSOR_LED_Pin,GPIO_PIN_SET)
#define RGB_BACKLIGHT_OFF    HAL_GPIO_WritePin(C_SENSOR_LED_GPIO_Port,C_SENSOR_LED_Pin,GPIO_PIN_RESET)

#ifdef COLOR_SENSOR_ENABLE
extern void cool_writeV(float coolV);
extern void SGM62110_INIT(void);
extern uint8_t LTR831_GetRawData(_COLOR_RGBC_Typedef *rgbc);


extern void LTR831_INIT(void);

extern uint8_t ColorRecognition(void);
extern void RGB_Sensor_Calibration(void);
extern void RGB_SENSER_RESET(void);
#endif


#endif


