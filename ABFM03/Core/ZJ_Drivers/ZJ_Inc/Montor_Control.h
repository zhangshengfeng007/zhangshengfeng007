#ifndef _MONTOR_CONTROL_H
#define _MONTOR_CONTROL_H
#include "main.h"
#include "includes.h"
#include "Handle.h"

/*该脚位即使5v输出 控制脚，也是充电IC的控制脚
1. 当该脚位电压为低电平时，进行充电，由DC_5V直接输出 5v
2. 当该脚位电压为高电平时，停止充电，由电池升压输出5v  123
*/
#define BOOST_5V_ON()      HAL_GPIO_WritePin(BOOST_5V_EN_PORT, BOOST_5V_EN_PIN, GPIO_PIN_SET)
#define BOOST_5V_OFF()     HAL_GPIO_WritePin(BOOST_5V_EN_PORT, BOOST_5V_EN_PIN, GPIO_PIN_RESET)

#define BOOST_3V3_ON()     HAL_GPIO_WritePin(BOOST_3V3_ON_PORT, BOOST_3V3_ON_PIN, GPIO_PIN_SET)
#define BOOST_3V3_OFF()    HAL_GPIO_WritePin(BOOST_3V3_ON_PORT, BOOST_3V3_ON_PIN, GPIO_PIN_RESET)

#define MOTOR_ON()         HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_SET)
#define MOTOR_OFF()        HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_RESET)

void Montor_Drive(_Sys_Info_TypeDef* SysInfo);
#endif
