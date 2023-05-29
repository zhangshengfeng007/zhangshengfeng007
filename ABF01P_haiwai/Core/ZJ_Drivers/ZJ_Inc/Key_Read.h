#ifndef __KEY_READ_H
#define __KEY_READ_H

#include "stm32g0xx_hal.h"
#include "gpio.h"
#include "includes.h"


#define KEY1_INPUT_Pin       KEY_ON_OFF_Pin 
#define KEY1_INPUT_GPIO_Port KEY_ON_OFF_GPIO_Port 
#define KEY2_INPUT_Pin       KEY_UP_Pin 
#define KEY2_INPUT_GPIO_Port KEY_UP_GPIO_Port 
#define KEY3_INPUT_Pin       KEY_DOWN_Pin 
#define KEY3_INPUT_GPIO_Port KEY_DOWN_GPIO_Port 

#define Key_Scap_Read_In        HAL_GPIO_ReadPin(KEY1_INPUT_GPIO_Port,KEY1_INPUT_Pin)
#define Key_Plus_Read_In        HAL_GPIO_ReadPin(KEY2_INPUT_GPIO_Port,KEY2_INPUT_Pin)
#define Key_minus_Read_In       HAL_GPIO_ReadPin(KEY3_INPUT_GPIO_Port,KEY3_INPUT_Pin)


typedef enum 
{   
  KEY_None,
  KEY_MSCAP,
  KEY_PLUS,
  KEY_MINUS,
//  KEY_TEST	
}_Key_Input;

void Key_Stroke(void);
//extern void Key_Proc(void);
void Key_S1_Str(void);
void Key_S1_Long_Str(void);
void Key_S2_Str(void);
void Key_S2_Long_Str(void);
void Key_S3_Str(void);
void Key_S3_Long_Str(void);
void Key_Call_Data(void);

void Test_Key_Press_Process(_Key_Input key);
void Test_Key_PutUp_Process(_Key_Input key);
void Test_Key_Press_Long_Process(_Key_Input key);
void Test_Key_S1_Str(void);
void Test_Key_S2_Str(void);
void Test_Key_S3_Str(void);
void Test_Key_S1_Long_Str(void);


#endif

