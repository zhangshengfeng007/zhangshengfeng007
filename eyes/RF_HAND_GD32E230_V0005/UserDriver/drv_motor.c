/***********************************************************
* @file    drv_motor.c
* @brief  �����������
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_motor.h"

 Motor_Type Motor;

/*
*************************************************************
* ����˵��: ����ʼ��
* ��    ��: ��
* �� �� ֵ: ��
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
* ����˵��: �������һ��ʱ��
* ��    ��: Time����ʱ��
* �� �� ֵ: ��
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
* ����˵��: ���ֹͣ
* ��    ��: ��
* �� �� ֵ: ��
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
* ����˵��: ����������к���
* ��    ��: ��
* �� �� ֵ: ��
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
