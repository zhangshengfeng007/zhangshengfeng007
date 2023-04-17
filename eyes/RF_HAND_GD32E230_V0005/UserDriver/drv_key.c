/***********************************************************
* @file    drv_key.c
* @brief  按键驱动程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_key.h"

static  Key_Type* pHead_Key = NULL;	//链表头

static void Add_Key(Key_Type* key);


/**************************  硬件层start  **********************************/
/*
*************************************************************
* 功能说明: 按键硬件初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Key_Hardware_Init(void)
{
	/* enable the  GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);

	//INPUT
	gpio_mode_set(KEY_POWER_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY_POWER_PIN);
}

uint8_t Read_KeyPower_Level(void)
{
	if(RESET == gpio_input_bit_get(KEY_POWER_PORT,KEY_POWER_PIN))
	{
		return 0;
	}
	return 1;
}

/**************************  硬件层end  **********************************/


/**************************  驱动层start  **********************************/

/************************************************************
  * @brief   按键创建
  * @param   key : 按键结构体
  * @param   read_key_level : 按键电平读取函数，需要用户自己实现返回uint8_t类型的电平
  * @param   key_trigger_level : 按键触发电平
  * @param   key_callback : 按键触发之后的回调处理函数。需要用户实现
  * @return  NULL
  * @version v1.0
  * @note    NULL
  ***********************************************************/
void Key_Create(Key_Type *key, uint8_t (*read_key_level)(void),uint8_t key_trigger_level,Key_CallBack key_callback)
{
	if(key == NULL)
	{
		while(1){};
	}

	key->Key_State = KEY_NONE;           //按键状态
	key->Key_Trigger_Level = key_trigger_level;  //按键触发电平
	key->Timer_Count = 0;
	key->Debounce_Time = 0;
	key->Long_Time = 0;
	key->Read_Key_Level = read_key_level;    //按键读电平函数
	key->CallBack_Function = key_callback; //按键事件触发的回调函数，用于处理按键事件

	Add_Key(key);          //创建的时候添加到单链表中
}

/*
*************************************************************
* 功能说明: 按键复位
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Key_Reset(void)
{
	struct Key_S* pass_btn;

	for(pass_btn = pHead_Key; pass_btn != NULL; pass_btn = pass_btn->Next)
	{
		pass_btn->Key_State = KEY_NONE;	//按键状态
		pass_btn->Timer_Count = 0;
		pass_btn->Debounce_Time = 0;
		pass_btn->Long_Time = 0;
	}
}

/************************************************************
  * @brief   删除一个已经创建的按键
  * @param   key 需要删除的按键
  * @return  NULL
  * @version v1.0
  * @note    NULL
  ***********************************************************/
void Key_Delete(Key_Type *key)
{
	struct Key_S** curr;

	for(curr = &pHead_Key; *curr;)
	{
		struct Key_S* entry = *curr;

		if (entry == key)
		{
			*curr = entry->Next;
		}
		else
		{
			curr = &entry->Next;
		}
	}
}

/************************************************************
  * @brief   按键周期处理函数
  * @param   key:处理的按键
  * @return  NULL
  * @version v1.0
  * @note    必须以一定周期调用此函数
  ***********************************************************/
void Key_Process(Key_Type *key)
{
	uint8_t current_level = (uint8_t)key->Read_Key_Level();//获取当前按键电平

	/*  消抖处理  */
	if(current_level != key->Key_Last_Level){	//按键电平发生变化
		if(key->Debounce_Time < KEY_DEBOUNCE_TIME){
			key->Debounce_Time += KEY_SCAN_CYCLE;
			return;
		}else{
			key->Key_Last_Level = current_level; //更新当前按键电平
			key->Debounce_Time = 0;
		}
	}

	/*  状态更新与事件回调处理  */
	switch(key->Key_State)
	{
	case KEY_PRESS:
		if(current_level == key->Key_Trigger_Level)
		{
			key->Long_Time += KEY_SCAN_CYCLE;
			if(key->Long_Time >= KEY_LONG_TIME)
			{
				key->Long_Time = 0;
				key->Key_State = KEY_LONG;
				if(key->CallBack_Function != NULL){
					key->CallBack_Function(KEY_LONG);    //长按
				}
			}
		}else{
			key->Long_Time = 0;
			key->Key_State = KEY_PRESS_UP;
			if(key->CallBack_Function != NULL){
				key->CallBack_Function(KEY_PRESS_UP);	//放手
			}
		}
		break;
	case KEY_LONG:
		if(current_level == key->Key_Trigger_Level)
		{
			key->Long_Time += KEY_SCAN_CYCLE;
			if(key->Long_Time >= KEY_CONTINUOS_TIME)
			{
				key->Long_Time = 0;
				key->Key_State = KEY_CONTINUOS;
				if(key->CallBack_Function != NULL){
					key->CallBack_Function(KEY_CONTINUOS);    //连续按
				}
			}
		}else{
			key->Long_Time = 0;
			key->Key_State = KEY_LONG_UP;
			if(key->CallBack_Function != NULL){
				key->CallBack_Function(KEY_LONG_UP);	//放手
			}
		}
		break;
	case KEY_CONTINUOS:
		if(current_level == key->Key_Trigger_Level)
		{
			key->Long_Time += KEY_SCAN_CYCLE;
			if(key->Long_Time >= KEY_CONTINUOS_TIME)
			{
				key->Long_Time = 0;
				key->Key_State = KEY_CONTINUOS;
				if(key->CallBack_Function != NULL){
					key->CallBack_Function(KEY_CONTINUOS);    //连续按
				}
			}
		}else{
			key->Long_Time = 0;
			key->Key_State = KEY_CONTINUOS_UP;
			if(key->CallBack_Function != NULL){
				key->CallBack_Function(KEY_CONTINUOS_UP);	//放手
			}
		}
		break;
	default:
		if(current_level == key->Key_Trigger_Level) //按键按下
		{
			key->Key_State = KEY_PRESS;
			if(key->CallBack_Function != NULL){
				key->CallBack_Function(KEY_PRESS);
			}
		}
		break;
	}

	if(key->count_press_delay)
	{
		key->count_press_delay --;
	}
	else
	{
		key->press_cnt = 0;
	}

}

/************************************************************
  * @brief   遍历的方式扫描按键，不会丢失每个按键
	* @param   NULL
  * @return  NULL
  * @version v1.0
  * @note    此函数要周期调用
  ***********************************************************/
void Key_Scan_Process(void)
{
	struct Key_S* pass_btn;

	for(pass_btn = pHead_Key; pass_btn != NULL; pass_btn = pass_btn->Next)
	{
		Key_Process(pass_btn);
	}
}


/************************************************************
  * @brief   获取按键触发的状态
	* @param   NULL
  * @return  NULL
  * @version v1.0
  ***********************************************************/
uint8_t Get_Key_State(Key_Type *key)
{
	return (uint8_t)(key->Key_State);
}

/************************************************************
  * @brief   使用单链表将按键连接起来
	* @param   NULL
  * @return  NULL
  * @version v1.0
  * @note    NULL
  ***********************************************************/
static void Add_Key(Key_Type* key)
{
	key->Next = pHead_Key;
	pHead_Key = key;
}

/**************************  驱动层end  **********************************/


















