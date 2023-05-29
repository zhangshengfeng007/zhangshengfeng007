#ifndef _MONTOR_CONTROL_H
#define _MONTOR_CONTROL_H
#include "main.h"
#include "includes.h"
#include "Handle.h"

///ϵͳ5V���翪��
#define BOOST_5V_ON_PIN      GPIO_PIN_3 
#define BOOST_5V_ON_PORT		 GPIOD

#define BOOST_5V_ON()        HAL_GPIO_WritePin(BOOST_5V_ON_PORT,BOOST_5V_ON_PIN,GPIO_PIN_SET)
#define BOOST_5V_OFF()       HAL_GPIO_WritePin(BOOST_5V_ON_PORT,BOOST_5V_ON_PIN,GPIO_PIN_RESET)
///�������
#define MOTOR_ON_PIN         GPIO_PIN_14 
#define MOTOR_ON_PORT			   GPIOC

#define MOTOR_OFF()          HAL_GPIO_WritePin(MOTOR_ON_PORT,MOTOR_ON_PIN,GPIO_PIN_RESET)
#define MOTOR_ON()           HAL_GPIO_WritePin(MOTOR_ON_PORT,MOTOR_ON_PIN,GPIO_PIN_SET)

void Montor_Drive(_Sys_Info_TypeDef* SysInfo);
#endif
