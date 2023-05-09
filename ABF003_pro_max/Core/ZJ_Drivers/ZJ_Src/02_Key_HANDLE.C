#include "includes.h"
//**********************************************************
//宏定义****************************************************
#define  unchar     unsigned char 
#define  unint      unsigned int
#define  unlong     unsigned long
 
#define KEY_LONG_DOWN_DELAY  50   //长按最短时间  50*20ms=1000ms
/**********************enum***************************/
typedef enum _KEY_STATUS_LIST
{
	KEY_NULL = 0x00,
	KEY_SURE = 0x01,
	KEY_UP   = 0x02,
	KEY_DOWN = 0x04,
	KEY_LONG = 0x08,
}KEY_STATUS_LIST;

typedef enum _KEY_LIST
{
	KEY0,
	KEY1,
	KEY2,
	KEY_NUM,
}KEY_LIST;

/**************************************************************/
/**************************struct******************************/
typedef struct _KEY_VALUE_typedef
{
    uint8_t KEY_SHIELD;       //按键屏蔽0:屏蔽，1:不屏蔽
	  uint8_t KEY_COUNT;        //按键长按计数
    uint8_t KEY_LEVEL;        //虚拟当前IO电平，按下1，抬起0
    uint8_t KEY_DOWN_LEVEL;   //按下时IO实际的电平
    uint8_t KEY_STATUS;       //按键状态
    uint8_t KEY_EVENT;        //按键事件
    uint8_t (*READ_PIN)(void);//读IO电平函数
}KEY_VALUE_typedef;
extern KEY_VALUE_typedef Key_Handle_Buf[KEY_NUM];
KEY_VALUE_typedef Key_Handle_Buf[KEY_NUM];


static uint8_t KEY0_ReadPin(void);
static uint8_t KEY1_ReadPin(void);
static uint8_t KEY2_ReadPin(void);
static void Get_Key_Level(void);
void ReadKeyStatus(void);
extern void Task_KEY_Scan(void);
void Task_KEY_Scan(void);
KEY_VALUE_typedef Key_Handle_Buf[KEY_NUM] = {
{1,0,0,0,KEY_NULL,KEY_NULL,KEY0_ReadPin},
{1,0,0,0,KEY_NULL,KEY_NULL,KEY1_ReadPin},
{1,0,0,0,KEY_NULL,KEY_NULL,KEY2_ReadPin},
};
/********************************************************************/

static uint8_t KEY0_ReadPin(void)
{
    uint8_t _KEY0=0;
    if(Key_Scap_Read_In == GPIO_PIN_RESET)
    {
        _KEY0=1;
    }

    
    return _KEY0;
}

static uint8_t KEY1_ReadPin(void)
{
    uint8_t _KEY1=0;    
    
    if(Key_Plus_Read_In == GPIO_PIN_RESET)
    {
        _KEY1=1;
    }    
    
    return _KEY1;
}

static uint8_t KEY2_ReadPin(void)
{
     uint8_t _KEY2=0;   
     if(Key_minus_Read_In == GPIO_PIN_RESET)
    {
        _KEY2=1;
    }        
    return _KEY2;
}
/*-------------------------------------------------
 * 函数名称：Get_Key_Level
//描  述: 获取IO电平函数
//输入值: 无
//输出值: 无
//返回值: 无
 -------------------------------------------------*/
static void Get_Key_Level(void)
{
    uint8_t i;
    
    for(i = 0;i < KEY_NUM;i++)
    {
        if(Key_Handle_Buf[i].KEY_SHIELD == 0)
            continue;
        if(Key_Handle_Buf[i].READ_PIN() == Key_Handle_Buf[i].KEY_DOWN_LEVEL)
            Key_Handle_Buf[i].KEY_LEVEL = 1;
        else
            Key_Handle_Buf[i].KEY_LEVEL = 0;
    }
}
/*-------------------------------------------------
 * 函数名称ReadKeyStatus
//描  述: 按键状态函数
//输入值: 无
//输出值: 无
//返回值: 无
 -------------------------------------------------*/
void ReadKeyStatus(void)
{
    uint8_t i;
	
    Get_Key_Level();
	
    for(i = 0;i < KEY_NUM;i++)
    {
        switch(Key_Handle_Buf[i].KEY_STATUS)
        {
            //状态0：没有按键按下
            case KEY_NULL:
                if(Key_Handle_Buf[i].KEY_LEVEL == 1)//有按键按下
                {
                    Key_Handle_Buf[i].KEY_STATUS = KEY_SURE;//转入状态1
					Key_Handle_Buf[i].KEY_EVENT = KEY_NULL;//空事件
                }
                else
                {
                    Key_Handle_Buf[i].KEY_EVENT = KEY_NULL;//空事件
                }
                break;
            //状态1：按键按下确认
            case KEY_SURE:
                if(Key_Handle_Buf[i].KEY_LEVEL == 1)//确认和上次相同
                {
                    Key_Handle_Buf[i].KEY_STATUS = KEY_DOWN;//转入状态2
					Key_Handle_Buf[i].KEY_EVENT = KEY_DOWN;//按下事件
                    Key_Handle_Buf[i].KEY_COUNT = 0;//计数器清零
                }
                else
                {
                    Key_Handle_Buf[i].KEY_STATUS = KEY_NULL;//转入状态0
                    Key_Handle_Buf[i].KEY_EVENT = KEY_NULL;//空事件
                }
                break;
            //状态2：按键按下
            case KEY_DOWN:
                if(Key_Handle_Buf[i].KEY_LEVEL != 1)//按键释放，端口高电平
                {
                    Key_Handle_Buf[i].KEY_STATUS = KEY_NULL;//转入状态0
                    Key_Handle_Buf[i].KEY_EVENT = KEY_UP;//松开事件
                }
                else if((Key_Handle_Buf[i].KEY_LEVEL == 1) && (++Key_Handle_Buf[i].KEY_COUNT >= KEY_LONG_DOWN_DELAY)) //超过KEY_LONG_DOWN_DELAY没有释放
                {
                    Key_Handle_Buf[i].KEY_STATUS = KEY_LONG;//转入状态3
                    Key_Handle_Buf[i].KEY_EVENT = KEY_LONG;//长按事件
					Key_Handle_Buf[i].KEY_COUNT = 0;//计数器清零
                }
                else
                {
                    Key_Handle_Buf[i].KEY_EVENT = KEY_NULL;//空事件
                }
                break;
            //状态3：按键连续按下
            case KEY_LONG:
                if(Key_Handle_Buf[i].KEY_LEVEL != 1)//按键释放，端口高电平
                {
                    Key_Handle_Buf[i].KEY_STATUS = KEY_NULL;//转入状态0
                    Key_Handle_Buf[i].KEY_EVENT = KEY_UP;//松开事件
					Key_Handle_Buf[i].KEY_EVENT = KEY_NULL;
                }
                else if((Key_Handle_Buf[i].KEY_LEVEL == 1) 
                && (++Key_Handle_Buf[i].KEY_COUNT >= KEY_LONG_DOWN_DELAY)) //超过KEY_LONG_DOWN_DELAY没有释放
                {
                    Key_Handle_Buf[i].KEY_EVENT = KEY_LONG;//长按事件
                    Key_Handle_Buf[i].KEY_COUNT = KEY_LONG_DOWN_DELAY;//计数器清零
                }
                else
                {
                    Key_Handle_Buf[i].KEY_EVENT = KEY_NULL;//空事件
                }
                break;
        }
	}
}
/*-------------------------------------------------
 * 函数名称Task_KEY_Scan
//描  述:按键处理函数
//输入值: 无
//输出值: 无
//返回值: 无
 -------------------------------------------------*/
void Task_KEY_Scan(void)
{
	ReadKeyStatus();
	
	if(Key_Handle_Buf[KEY0].KEY_EVENT == KEY_UP) //开机
	{
		/************写入处理函数********************/
//		printf("KEY0 Down\n");
		Key_S1_Str();
        
	}
	else if(Key_Handle_Buf[KEY0].KEY_EVENT == KEY_LONG)
	{
		/************写入处理函数********************/		
//		printf("KEY0 Long Down\n");
		Key_S1_Long_Str();
	}
	
	if(Key_Handle_Buf[KEY1].KEY_EVENT == KEY_UP) //加档
	{
		/************写入处理函数********************/		
//		printf("KEY1 Down\n");
        
	}
	else if(Key_Handle_Buf[KEY1].KEY_EVENT == KEY_LONG)
	{
		/************写入处理函数********************/		
//		printf("KEY1 Long Down\n");

	}
	
	if(Key_Handle_Buf[KEY2].KEY_EVENT == KEY_UP) //减档
	{
		/************写入处理函数********************/		
//		printf("KEY2 Down\n");

	}
	else if(Key_Handle_Buf[KEY2].KEY_EVENT == KEY_LONG)
	{
		/************写入处理函数********************/		
//		printf("KEY2 Long Down\n");

	}
}


