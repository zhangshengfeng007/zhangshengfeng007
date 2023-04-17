#ifndef __DRV_SENSOR_H
#define __DRV_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/

#define SENSOR_NUM	2

typedef struct{
	u8 State;		//状态
	u8 Level;		//当前电平
	u8 LastLevel;	//上一次电平
	u16 Count;		//计时
	u16 ShakeTime;	//消抖时间
	u8 (*Read_Key_Level)(void); 	/* 读取按键电平函数 */
	void (*Sensor_CallBack)(void); 	/* 传感器变化回调函数 */
}Sensor_Type;


extern Sensor_Type SensorCharge;
extern Sensor_Type SensorPPR;

/* Exported functions ------------------------------------------------------- */
void Sensor_Init(void);
void Sensor_Process(void);

#endif

