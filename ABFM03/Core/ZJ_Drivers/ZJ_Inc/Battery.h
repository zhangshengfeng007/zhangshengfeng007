#ifndef _BATTERY_H
#define _BATTERY_H
#include "includes.h"

/*******************USB������***************************123****************/
//#define DC5V_IN_DET_PIN        	DC5V_IN_DET_PIN
//#define VIN_DET_PORT		   	DC5V_IN_DET_PORT
#define VIN_DET_IN()		   	HAL_GPIO_ReadPin(DC5V_IN_DET_PORT, DC5V_IN_DET_PIN)
#define DC_5V_EXIST()			(GPIO_PIN_RESET == VIN_DET_IN())
#define DC_5V_INEXIST()			(GPIO_PIN_SET == VIN_DET_IN())  // 高电平时，dc 5v不存在
/*******************���״̬���*******************************************/
// #define CHARGE_STAT_PIN     	CHARGE_STAT_Pin
// #define CHARGE_STAT_PORT		CHARGE_STAT_Port
#define BAT_CHARGE_STATE_IN()	HAL_GPIO_ReadPin(CHARGE_STAT_Port, CHARGE_STAT_Pin)
#define BAT_CHARGE_FULL()	    (GPIO_PIN_SET == BAT_CHARGE_STATE_IN())             // 开启内部上拉，不充电或充满为高电平
#define BAT_CHARGING_NOW()	    (GPIO_PIN_RESET == BAT_CHARGE_STATE_IN())           // 充电中，电平为低

#define CountNum  5

/******************��ص�ѹ��⿪��*****************************************/
// #define BAT_ADC_EN_PIN			VBAT_OUT_EN_PIN
// #define BAT_ADC_EN_PORT			VBAT_OUT_EN_PORT

#define BAT_OUT_ENABLE()    	HAL_GPIO_WritePin(VBAT_OUT_EN_PORT, VBAT_OUT_EN_PIN, GPIO_PIN_SET)
#define BAT_OUT_DISABLE()   	HAL_GPIO_WritePin(VBAT_OUT_EN_PORT, VBAT_OUT_EN_PIN, GPIO_PIN_RESET)

#define ADC_ADD_COUNT   	 24
#define ADC_CHANNEL_NUM 	 4
#define	BAT_ADC_COUNT   	 130

#define Vems_ADC_CHANNEL     0
#define NTC_ADC_CHANNEL      1
#define Battery_ADC_CHANNEL  2
#define Skin_ADC_CHANNEL     3

#if ARF001
	#define error  0
#else
	#define error  0
#endif
#define BAT_VOL_5V00	(4096+error)	/// 2.5      3604
#define BAT_VOL_4V40	(3604+error)	/// 2.1      3604
#define BAT_VOL_4V20	(3441+error)	/// 2.1      3441
#define BAT_VOL_4V17	(3416+error)	/// 2.085    3400
#define BAT_VOL_4V15	(3400+error)	/// 2.075    3400
#define BAT_VOL_4V13	(3383+error)	/// 2.065    3400
#define BAT_VOL_4V10	(3359+error)    /// 2.05     3359
#define BAT_VOL_4V05	(3317+error)    /// 2.024    3317
#define BAT_VOL_4V00	(3277+error)    /// 2.0      3277
#define BAT_VOL_3V95	(3236+error)    /// 1.975    3236
#define BAT_VOL_3V90	(3194+error)    /// 1.949    3236
#define BAT_VOL_3V85	(3154+error)    /// 1.925    3154
#define BAT_VOL_3V80	(3113+error)    /// 1.900    3113
#define BAT_VOL_3V75	(3072+error)    /// 1.875    3072
#define BAT_VOL_3V65	(2990+error)    /// 1.825    2990
#define BAT_VOL_3V55	(2908+error)    /// 1.775    2908
#define BAT_VOL_3V30	(2816+error)    /// 1.650    2816
#define BAT_VOL_3V20	(2621+error)    /// 1.599    2621
#define BAT_VOL_3V15	(2581+error)    /// 1.575    2581
#define BAT_VOL_3V10	(2539+error)    /// 1.549    2539
#define BAT_VOL_3V05	(2499+error)    /// 1.525    2499
#define BAT_VOL_HYS		10      /// 20


#define BAT_VOL_3V60	(2949+error)
#define BAT_VOL_3V50	(2867+error)

typedef struct
{
	uint8_t  status;
	uint8_t  read_flag;
	uint16_t bat_lvl_full_disp_delay;
	uint16_t charge_ic_pin_full_disp_delay;
	uint16_t adc_Value;
	uint16_t real_Val;

	/* data */
}BAT_DATA_t;

extern BAT_DATA_t BAT_Data;

extern uint16_t ADC_Value[ADC_ADD_COUNT][ADC_CHANNEL_NUM] ;
extern uint16_t Bat_ADC_Value[BAT_ADC_COUNT];
extern void USB_Plug_Scan(void);
void ADC_DMA_COV_START(void);
uint16_t filter(uint16_t *pData, uint8_t channel, uint8_t len, uint8_t count);
// uint8_t Check_Batter_State(void) ;
uint8_t Scan_Batter_State(void) ;
uint16_t Get_Battery_Read(uint16_t* ADC_Source,uint16_t Counts);
// uint8_t CheckWorking_Batter_State(uint16_t WorkBatValue);
// uint8_t CheckIdle_Batter_State(uint16_t CheckBatValue);
// uint8_t CheckCharge_Batter_State(uint16_t ChargeBatValue);
#endif


