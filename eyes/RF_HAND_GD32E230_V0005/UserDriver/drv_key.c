/***********************************************************
* @file    drv_key.c
* @brief  ������������
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_key.h"

static  Key_Type* pHead_Key = NULL;	//����ͷ

static void Add_Key(Key_Type* key);


/**************************  Ӳ����start  **********************************/
/*
*************************************************************
* ����˵��: ����Ӳ����ʼ��
* ��    ��: ��
* �� �� ֵ: ��
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

/**************************  Ӳ����end  **********************************/


/**************************  ������start  **********************************/

/************************************************************
  * @brief   ��������
  * @param   key : �����ṹ��
  * @param   read_key_level : ������ƽ��ȡ��������Ҫ�û��Լ�ʵ�ַ���uint8_t���͵ĵ�ƽ
  * @param   key_trigger_level : ����������ƽ
  * @param   key_callback : ��������֮��Ļص�����������Ҫ�û�ʵ��
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

	key->Key_State = KEY_NONE;           //����״̬
	key->Key_Trigger_Level = key_trigger_level;  //����������ƽ
	key->Timer_Count = 0;
	key->Debounce_Time = 0;
	key->Long_Time = 0;
	key->Read_Key_Level = read_key_level;    //��������ƽ����
	key->CallBack_Function = key_callback; //�����¼������Ļص����������ڴ������¼�

	Add_Key(key);          //������ʱ����ӵ���������
}

/*
*************************************************************
* ����˵��: ������λ
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void Key_Reset(void)
{
	struct Key_S* pass_btn;

	for(pass_btn = pHead_Key; pass_btn != NULL; pass_btn = pass_btn->Next)
	{
		pass_btn->Key_State = KEY_NONE;	//����״̬
		pass_btn->Timer_Count = 0;
		pass_btn->Debounce_Time = 0;
		pass_btn->Long_Time = 0;
	}
}

/************************************************************
  * @brief   ɾ��һ���Ѿ������İ���
  * @param   key ��Ҫɾ���İ���
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
  * @brief   �������ڴ�����
  * @param   key:����İ���
  * @return  NULL
  * @version v1.0
  * @note    ������һ�����ڵ��ô˺���
  ***********************************************************/
void Key_Process(Key_Type *key)
{
	uint8_t current_level = (uint8_t)key->Read_Key_Level();//��ȡ��ǰ������ƽ

	/*  ��������  */
	if(current_level != key->Key_Last_Level){	//������ƽ�����仯
		if(key->Debounce_Time < KEY_DEBOUNCE_TIME){
			key->Debounce_Time += KEY_SCAN_CYCLE;
			return;
		}else{
			key->Key_Last_Level = current_level; //���µ�ǰ������ƽ
			key->Debounce_Time = 0;
		}
	}

	/*  ״̬�������¼��ص�����  */
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
					key->CallBack_Function(KEY_LONG);    //����
				}
			}
		}else{
			key->Long_Time = 0;
			key->Key_State = KEY_PRESS_UP;
			if(key->CallBack_Function != NULL){
				key->CallBack_Function(KEY_PRESS_UP);	//����
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
					key->CallBack_Function(KEY_CONTINUOS);    //������
				}
			}
		}else{
			key->Long_Time = 0;
			key->Key_State = KEY_LONG_UP;
			if(key->CallBack_Function != NULL){
				key->CallBack_Function(KEY_LONG_UP);	//����
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
					key->CallBack_Function(KEY_CONTINUOS);    //������
				}
			}
		}else{
			key->Long_Time = 0;
			key->Key_State = KEY_CONTINUOS_UP;
			if(key->CallBack_Function != NULL){
				key->CallBack_Function(KEY_CONTINUOS_UP);	//����
			}
		}
		break;
	default:
		if(current_level == key->Key_Trigger_Level) //��������
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
  * @brief   �����ķ�ʽɨ�谴�������ᶪʧÿ������
	* @param   NULL
  * @return  NULL
  * @version v1.0
  * @note    �˺���Ҫ���ڵ���
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
  * @brief   ��ȡ����������״̬
	* @param   NULL
  * @return  NULL
  * @version v1.0
  ***********************************************************/
uint8_t Get_Key_State(Key_Type *key)
{
	return (uint8_t)(key->Key_State);
}

/************************************************************
  * @brief   ʹ�õ�����������������
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

/**************************  ������end  **********************************/


















