#ifndef __KEY_PROCESS_H
#define __KEY_PROCESS_H

#include "stm32g0xx_hal.h"

#define _KEY_NONE               0 
#define _REENTER                1 
#define _NO_REENTER             2

#define KEY_SET_DOWN		GPIO_PIN_RESET
#define KEY_SET_UP              GPIO_PIN_SET

#define Power_key_Read_In       HAL_GPIO_ReadPin(POW_KEY_GPIO_Port,POW_KEY_Pin)
#define Gear_key_Read_In        HAL_GPIO_ReadPin(LEVEL_KEY_GPIO_Port,LEVEL_KEY_Pin)

typedef enum                   //按键序号
{   
  KEY_None,
  KEY_POWER,
  KEY_GEAR,
  _KEY_MAX,
}_Key_Input;

typedef enum _KEY_STATE        //按键状态
{
  _HAS_NO_KEY,                //0:未按下             
  _HAS_KEY_DOWN,              //1:检测到一次按下,           
  _HAS_KEY_SURE,              //2:又检测到一次按下
  _HAS_KEY_WAITUP,            //3:等待键抬起       
}_Key_State;

typedef struct   
{
  uint8_t Key_Short           :1;
  uint8_t Key_Long            :1;
  uint8_t Key_Short_Set       :1; 
}_Key_Flag_Struct;

typedef struct   
{
  uint8_t  Long_Key_t;     
  uint8_t  LastKey;      
  uint8_t  PreDownKey; 
  uint8_t  KeyState;   
  uint16_t SameKeyCntr;     
  uint8_t  CurKey;                                
  uint8_t  (*KeyDownCallBack)(uint16_t, uint16_t); 
  void    (*KeyUpCallBack)(uint16_t);            
}struct_KeyInfo;

uint8_t KeyDownCallBack(uint16_t Key, uint16_t Times); 
void KeyUpCallBack(uint16_t Key);                      
void DitherlessKey(struct_KeyInfo* pInfo);              
uint8_t Key_Scan(void);
void Key_Stroke(void);
void Key_Proc(void);
void Key_S1_SetDown_Str(void);
void Key_S1_Str(void);
void Key_S1_Long_Str(void);
void Key_S2_Str(void);
void Key_S2_Long_Str(void);
void Key_S3_Str(void);
void Key_S3_Long_Str(void);

extern _Key_Flag_Struct        Key_Flag;

#endif

