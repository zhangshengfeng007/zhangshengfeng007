#ifndef __DRV_POWER_H
#define __DRV_POWER_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


//异常电量	     1.5V	// (1.5 * 2/3) * 4096/3.3 =  1241
//很低电量	      3V	// (3 * 2/3) * 4096/3.3	  =  2482
//低电量		3.2V	// (3.2 * 2/3) * 4096/3.3  = 2647

//低电量		3.4V	// (3.4 * 2/3) * 4096/3.3  = 2813

//低电量		3.6V	// (3.6 * 2/3) * 4096/3.3  = 2978
//低电量		3.7V	// (3.7 * 2/3) * 4096/3.3  = 3061
//70%的电量     3.9v	// (3.9 * 2/3) * 4096/3.3  = 3227
//满电量		4.1V	// (4.1 * 2/3) * 4096/3.3  = 3392

// 0.05v       0.05V	// (0.05 * 2/3) * 4096/3.3  = 42
// 0.15v               //  (0.15 * 2/3) * 4096/3.3  = 126

#define BAT_0V_05_ADC_VAL  			42
#define BAT_0V_10_ADC_VAL  			84
#define BAT_CHARGE_0V_15_ADC_VAL  	126  // 充电时，用万用表测到T2处电压浮动 0.1v左右，实际电池电压上浮 0.15v
#define BAT_CHARGE_0V_20_ADC_VAL  	180

#define BAT_3V0_ADC_VAL  			2482
#define BAT_3V2_ADC_VAL  			2647
#define BAT_3V3_ADC_VAL  			2703
#define BAT_3V4_ADC_VAL  			2813

#define BAT_3V6_ADC_VAL  			2978
#define BAT_3V7_ADC_VAL  			3061

#define BAT_3V9_ADC_VAL  			3227
#define BAT_4V1_ADC_VAL  			3392


/* Exported macro ------------------------------------------------------------*/
typedef struct{
	int adc_val;
	BatLevel_Type BatLevel;	//电池电量
	BatLevel_Type Last_Bat_Level;	//电池电量
	s16 BatTemp;			//电池温度,1位小数点
	Charge_Type CharheState;		//充电状态
}Power_Type;

extern Power_Type Power;

/* Exported functions ------------------------------------------------------- */
void Power_Init(void);
void Power_Charge_Control(u8 cmd);
void Bat_Temp_Read(void);
void Bat_Level_Detection(void);

#endif

