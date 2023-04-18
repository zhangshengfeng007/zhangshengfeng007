#ifndef __DRV_KEY_H
#define __DRV_KEY_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/
#define KEY_SCAN_CYCLE		2	//按键扫描周期MS
#define KEY_DEBOUNCE_TIME 	50   	//消抖时间MS
#define KEY_LONG_TIME 	    1500	 //长按触发时间MS
#define KEY_CONTINUOS_TIME  	150	 //连按按触发周期时间MS

typedef enum {
	KEY_PRESS = 0,
	KEY_PRESS_UP,
	KEY_LONG,
	KEY_LONG_UP,
	KEY_CONTINUOS,
	KEY_CONTINUOS_UP,
	KEY_NONE
}Key_Event;

typedef void (*Key_CallBack)(Key_Event);   /* 按键触发回调函数，需要用户实现 */

/*
	每个按键对应1个全局的结构体变量。
	其成员变量是实现滤波和多种按键状态所必须的
*/
typedef struct Key_S
{
	uint8_t Key_State:4;	  			/* 按键当前状态（按下还是弹起） */
	uint8_t Key_Trigger_Level:2;    	/* 按键触发电平 */
	uint8_t Key_Last_Level:2;    		/* 按键上次电平 */
	uint8_t Timer_Count;				/* 计时 */
	uint8_t Debounce_Time;			/* 消抖时间 */
	uint8_t press_cnt;				// 20230403 增加按键按下次数
	uint16_t count_press_delay;		// 20230403 增加按键按下次数

	uint16_t Long_Time;		  		/* 按键按下持续时间 */
	/* 下面是一个函数指针，指向判断按键是否按下的函数 */
	uint8_t (*Read_Key_Level)(void); 	/* 读取按键电平函数，需要用户实现 */
	Key_CallBack CallBack_Function;	/* 按键回调函数 */
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

