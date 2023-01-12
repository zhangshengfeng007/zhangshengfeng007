#ifndef __BATT_HANDLE_H
#define __BATT_HANDLE_H

#define BAT_CHARGE_STATE_IN   HAL_GPIO_ReadPin(CHARGE_STATE_GPIO_Port,CHARGE_STATE_Pin) ///充电状态  充满  正在充电
#define USB_INPUT_CHECK_IN    HAL_GPIO_ReadPin(USB_INPUT_CHECK_GPIO_Port,USB_INPUT_CHECK_Pin)
#define CHARGE_LIMIT_ENABLE   HAL_GPIO_WritePin(CHARGE_ILMIT_GPIO_Port,CHARGE_ILMIT_Pin,GPIO_PIN_SET)
#define CHARGE_LIMIT_DISABLE  HAL_GPIO_WritePin(CHARGE_ILMIT_GPIO_Port,CHARGE_ILMIT_Pin,GPIO_PIN_RESET)

//typedef enum
//{
//  Charge_None,
//  Charge_Start,
//}_Charge_State_Typedef;

//typedef enum
//{
//  BATT_LEVEL_10,
//  BATT_LEVEL_20,
//  BATT_LEVEL_30,
//  BATT_LEVEL_40,
//  BATT_LEVEL_50,
//  BATT_LEVEL_60,
//  BATT_LEVEL_70,
//  BATT_LEVEL_80,
//  BATT_LEVEL_90,
//  BATT_LEVEL_FULL
//}_Batt_Level_Typedef;

typedef enum
{
  Batt_Level_Gear_0 = 0,
  Batt_Level_Gear_1 = 1,
  Batt_Level_Gear_2 = 2,
  Batt_Level_Gear_3 = 3,
  Batt_Level_SP20 = 5,
  Batt_Level_SP10 = 6,
  Batt_Level_Full = 7,
}_BatteryLevel_typedef;

typedef struct
{
//  _Charge_State_Typedef	  Charge_State;
//  _Batt_Level_Typedef           Batt_Level;
  _BatteryLevel_typedef         BatteryLevel;  // 0 1 2 3   电池电量  5 电量剩余20%   6  电量剩余 10%  7  电池满电量
  uint16_t BATT_TRUE_Value;
  uint16_t USB_TRUE_Value;
}_Batt_State_Typedef;

extern _Batt_State_Typedef Batt_State;
extern void BatteryPowerJudgment(_sys_state_e state);
extern void LCD_output_transf(uint8_t *data);

#endif

