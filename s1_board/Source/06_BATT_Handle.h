#ifndef __BATT_HANDLE_H
#define __BATT_HANDLE_H

///充电状态判断脚
// DC5V 接入+脚位为高点平时，为充满状态
#define BAT_CHARGE_STATE_IN   HAL_GPIO_ReadPin(CHARGE_STATE_GPIO_Port, CHARGE_STATE_Pin)
#define IS_CHAGE_FULL()       (GPIO_PIN_SET == HAL_GPIO_ReadPin(CHARGE_STATE_GPIO_Port, CHARGE_STATE_Pin))

///DC5v接入判断脚
// DC5V 接入时为低电平
#define USB_INPUT_CHECK_IN    HAL_GPIO_ReadPin(USB_INPUT_CHECK_GPIO_Port, USB_INPUT_CHECK_Pin)
#define IS_DC5V_IN()          (GPIO_PIN_RESET == HAL_GPIO_ReadPin(USB_INPUT_CHECK_GPIO_Port, USB_INPUT_CHECK_Pin))

#define CHARGE_LIMIT_ENABLE   HAL_GPIO_WritePin(CHARGE_ILMIT_GPIO_Port, CHARGE_ILMIT_Pin, GPIO_PIN_SET)
#define CHARGE_LIMIT_DISABLE  HAL_GPIO_WritePin(CHARGE_ILMIT_GPIO_Port, CHARGE_ILMIT_Pin, GPIO_PIN_RESET)

typedef enum{
	BAT_3200mV_ADC_VAL	= 2624,	// 2624 ---  1.6V
	BAT_3250mV_ADC_VAL	= 2662,	// 2662 ---  1.625V
	BAT_3300mV_ADC_VAL	= 2703,	// 2785 ---  1.65V
	BAT_3350mV_ADC_VAL	= 2744,	// 2744 ---  1.675V
	BAT_3400mV_ADC_VAL	= 2785,	// 2785 ---  1.7V
	BAT_3450mV_ADC_VAL	= 2826,	// 2785 ---  1.725V
	BAT_3500mV_ADC_VAL	= 2867,	// 2826 ---  1.75V
	BAT_3550mV_ADC_VAL	= 2908,	// 2867 ---  1.775V
	BAT_3600mV_ADC_VAL	= 2949,	// 2949 ---  1.8V
	BAT_3650mV_ADC_VAL	= 2990,	// 2990 ---  1.825V
	BAT_3700mV_ADC_VAL	= 3031,	// 3031 ---  1.85V
	BAT_3750mV_ADC_VAL	= 3072,	// 3072 ---  1.875V
	BAT_3800mV_ADC_VAL	= 3112,	// 3112 ---  1.9V
	BAT_3850mV_ADC_VAL	= 3153,	// 3153 ---  1.925V
	BAT_3900mV_ADC_VAL	= 3194,	// 3194 ---  1.95V
	BAT_3950mV_ADC_VAL	= 3235, // 3235 ---  1.975V
	BAT_4000mV_ADC_VAL	= 3276,	// 3276 ---  2.0V
	BAT_4050mV_ADC_VAL	= 3317,	// 3317 ---  2.025V
	BAT_4100mV_ADC_VAL	= 3358,	// 3358 ---  2.05V
	BAT_4150mV_ADC_VAL	= 3399,	// 3399 ---  2.075V
  BAT_4200mV_ADC_VAL	= 3440,	// 3440 ---  2.1V
}Bat_adc_val_e;

typedef enum{
  DC_4000mV_ADC_VAL	= 2736, //  1.67V  --- 1.67*4096/2.5 = 2736
  DC_4100mV_ADC_VAL	= 2801, //  1.71V
  DC_4200mV_ADC_VAL	= 2867, //  1.75V
  DC_4300mV_ADC_VAL	= 2933, //  1.79V
  DC_4400mV_ADC_VAL	= 2998, //  1.83V
	DC_4500mV_ADC_VAL	= 3080,	//  1.88V
	DC_4600mV_ADC_VAL	= 3129,	//  1.91V
	DC_4700mV_ADC_VAL	= 3211,	//  1.96V
	DC_4800mV_ADC_VAL	= 3276,	//  2.0V
	DC_4900mV_ADC_VAL	= 3342,	//  2.04V
	DC_5000mV_ADC_VAL	= 3407,	//  2.08V
  DC_5400mV_ADC_VAL	= 3686,	//  2.25V  ----输入异常--电压过高
}DC5v_adc_val_e;


typedef enum
{
  Bat_Level0 = 0,
  Bat_Level1 = 1,
  Bat_Level2 = 2,
  Bat_Level3 = 3,
  Bat_Level_SP20 = 5,
  Bat_Level_SP10 = 6,
  Bat_Level_Full = 7,
}_bat_level_e;

typedef struct
{
  _bat_level_e  disp_level;
  uint16_t  adc_val;
  uint16_t  DC5v_adc_val;
}_bat_data_t;

extern _bat_data_t bat_data;
extern void BatteryPowerJudgment(_sys_state_e state);

#endif
