/***********************************************************
* @file    drv_sensor.c
* @brief  通用传感器程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_sensor.h"

Sensor_Type * SensorArr[SENSOR_NUM];
Sensor_Type SensorCharge;
Sensor_Type SensorPPR;

u8 SensorCharge_Read(void);
u8 SensorPPR_Read(void);

/*
*************************************************************
* 功能说明: 传感器初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Sensor_Init(void)
{
	/* enable the  GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOF);

	// INPUT
	gpio_mode_set(CHARGE_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, CHARGE_PIN);
	gpio_mode_set(PPR_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, PPR_PIN);

	SensorCharge.State = 1;
	SensorCharge.Level = 1;
	SensorCharge.LastLevel = 1;
	SensorCharge.Count = 0;
	SensorCharge.ShakeTime = 200;
	SensorCharge.Read_Key_Level = SensorCharge_Read;

	SensorPPR.State = 1;
	SensorPPR.Level = 1;
	SensorPPR.LastLevel = 1;
	SensorPPR.Count = 0;
	SensorPPR.ShakeTime = 200;
	SensorPPR.Read_Key_Level = SensorPPR_Read;

	SensorArr[0] = &SensorCharge;
	SensorArr[1] = &SensorPPR;
}

u8 SensorCharge_Read(void)
{
	if(RESET == gpio_input_bit_get(CHARGE_PORT,CHARGE_PIN))
	{
		return 0;
	}
	return 1;
}

u8 SensorPPR_Read(void)
{
	if(RESET == gpio_input_bit_get(PPR_PORT,PPR_PIN))
	{
		return 0;
	}
	return 1;
}

/*
*************************************************************
* 功能说明: 传感器初始化,需要周期调用
* 形    参: 无
* 返 回 值: 无 1ms调用一次
*************************************************************
*/
void Sensor_Process(void)
{
	u8 i = 0;

	for(i = 0; i < SENSOR_NUM; i++){
		SensorArr[i]->Level = SensorArr[i]->Read_Key_Level();

		if((SensorArr[i]->LastLevel == SensorArr[i]->Level)&&(SensorArr[i]->Level != SensorArr[i]->State)){
			SensorArr[i]->Count++;

			if(SensorArr[i]->Count > SensorArr[i]->ShakeTime){
				SensorArr[i]->Count = 0;
				SensorArr[i]->State = SensorArr[i]->Level;
			}
		}else{
			SensorArr[i]->LastLevel = SensorArr[i]->Level;
			SensorArr[i]->Count = 0;
		}
	}
}




