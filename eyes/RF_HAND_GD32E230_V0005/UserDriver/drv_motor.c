/***********************************************************
* @file    drv_motor.c
* @brief  马达驱动程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_motor.h"

 Motor_Type Motor;

/*
*************************************************************
* 功能说明: 马达初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Motor_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	
	gpio_mode_set(MOTOR_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MOTOR_PIN);
	gpio_output_options_set(MOTOR_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, MOTOR_PIN);

	MOTOR_OFF;

	Motor.State = Func_DISABLE;
	Motor.OutTime = 0;
}

/*
*************************************************************
* 功能说明: 马达启动一定时间
* 形    参: Time启动时间
* 返 回 值: 无
*************************************************************
*/
void Motor_Start_Time(u16 Time)
{
	if(Motor.State == Func_DISABLE){
		Motor.OutTime = Time;
		
		if(Motor.OutTime > 0)
		{
			Motor.State = Func_ENABLE;
			MOTOR_ON;
		}
	}
}

/*
*************************************************************
* 功能说明: 马达停止
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Motor_Stop(void)
{
	Motor.State = Func_DISABLE;
	Motor.OutTime = 0;
	MOTOR_OFF;
}


/*
*************************************************************
* 功能说明: 马达周期运行函数
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Motor_Process_Cycle(void)
{
	if(Motor.OutTime > 0)
	{
		Motor.OutTime--;
		MOTOR_ON;
	}else{
		MOTOR_OFF;
		Motor.State = Func_DISABLE;
 	}
}
