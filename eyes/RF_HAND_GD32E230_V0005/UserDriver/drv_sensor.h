#ifndef __DRV_SENSOR_H
#define __DRV_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/

#define SENSOR_NUM	2

typedef struct{
	u8 State;		//״̬
	u8 Level;		//��ǰ��ƽ
	u8 LastLevel;	//��һ�ε�ƽ
	u16 Count;		//��ʱ
	u16 ShakeTime;	//����ʱ��
	u8 (*Read_Key_Level)(void); 	/* ��ȡ������ƽ���� */
	void (*Sensor_CallBack)(void); 	/* �������仯�ص����� */
}Sensor_Type;


extern Sensor_Type SensorCharge;
extern Sensor_Type SensorPPR;

/* Exported functions ------------------------------------------------------- */
void Sensor_Init(void);
void Sensor_Process(void);

#endif

