#ifndef __DRV_KEY_H
#define __DRV_KEY_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/
#define KEY_SCAN_CYCLE		2	//����ɨ������MS
#define KEY_DEBOUNCE_TIME 	50   	//����ʱ��MS
#define KEY_LONG_TIME 	    1500	 //��������ʱ��MS
#define KEY_CONTINUOS_TIME  	150	 //��������������ʱ��MS

typedef enum {
	KEY_PRESS = 0,
	KEY_PRESS_UP,
	KEY_LONG,
	KEY_LONG_UP,
	KEY_CONTINUOS,
	KEY_CONTINUOS_UP,
	KEY_NONE
}Key_Event;

typedef void (*Key_CallBack)(Key_Event);   /* ���������ص���������Ҫ�û�ʵ�� */

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	���Ա������ʵ���˲��Ͷ��ְ���״̬�������
*/
typedef struct Key_S
{
	uint8_t Key_State:4;	  			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t Key_Trigger_Level:2;    	/* ����������ƽ */
	uint8_t Key_Last_Level:2;    		/* �����ϴε�ƽ */
	uint8_t Timer_Count;				/* ��ʱ */
	uint8_t Debounce_Time;			/* ����ʱ�� */
	uint8_t press_cnt;				// 20230403 ���Ӱ������´���
	uint16_t count_press_delay;		// 20230403 ���Ӱ������´���

	uint16_t Long_Time;		  		/* �������³���ʱ�� */
	/* ������һ������ָ�룬ָ���жϰ����Ƿ��µĺ��� */
	uint8_t (*Read_Key_Level)(void); 	/* ��ȡ������ƽ��������Ҫ�û�ʵ�� */
	Key_CallBack CallBack_Function;	/* �����ص����� */
	struct Key_S *Next;
}Key_Type;

/* Exported functions ------------------------------------------------------- */

void Key_Hardware_Init(void);
uint8_t Read_KeyPower_Level(void);
void Key_Create(Key_Type *key, uint8_t (*read_key_level)(void),uint8_t key_trigger_level,Key_CallBack key_callback);
void Key_Reset(void);
void Key_Process(Key_Type *key);
void Key_Scan_Process(void);
void Key_Delete(Key_Type *key);
uint8_t Get_Key_State(Key_Type *key);

#endif

