#include "system.h"

//0:输入模式 1:输出模式
#define		P00M		0
#define		P01M		0
#define		P02M		0		//T_DATA
#define		P03M		1		//T_CLOCK
#define		P04M		1		//LEDB
#define		P05M		1		//SDIO
#define		P06M		1		//SCKO
#define		P07M		1		//LCKO
#define		P10M		1		//IG_CTR
#define		P11M		1		//DISCHR
#define		P12M		0
#define		P13M		0
#define		P14M		0		//ROTARY
#define		P15M		0		//ROTARY
#define		P16M		0		//ROTARY
#define		P17M		0		//VB1
#define		P20M		0		//AD_NTC
#define		P21M		0
#define		P22M		0		//AD_VB2
#define		P23M		0		//FAN_AD
#define		P24M		0		//PULESE
#define		P25M		1		//FAN


void GPIO_Init(void)
{
	P0=0;P1=0;P2=0;
	P0M=0;P1M=0;P2M=0;
	//输入输出模式
	P0M = P00M|(P01M<<1)|(P02M<<2)|(P03M<<3)|(P04M<<4)|(P05M<<5)|(P06M<<6)|(P07M<<7);
	P1M = P10M|(P11M<<1)|(P12M<<2)|(P13M<<3)|(P14M<<4)|(P15M<<5)|(P16M<<6)|(P17M<<7);
	P2M |= P20M|(P22M<<2)|(P23M<<3)|(P24M<<4)|(P25M<<5);
	
	P0UR = 0;P1UR = 0;P2UR = 0;
	//PWM
	P2UR |= 0x30;
	//KEY
	P1UR |= 0x04;
	
	P0UR |= 0x04;
	//ADC
	P2UR &= ~0x0d;
	P1UR &= ~0x80;
	//DISPLAY
	SCK = 0;
	RCK = 0;
	LEDB = 1;
}


void SYSCLK_Init(void)
{
	CLKSEL |= 0x07;		//Fcpu = Fosc/1
	CLKCMD = 0x69;
}

/***********************************************************************************/
static idata TASK tasks[ARRAY_SIZE];
TIM_Period_T TIM_Period;

static TASK Task_Register(uint8_t FLAGS, void(*HANDLE)(void))
{
	TASK task;
	task.flags = FLAGS;
	task.handle = HANDLE;
	return (task);
}

void Task_Handle(void)// 任务执行函数，按任务的创建顺序执行任务；                     
{
	static uint8_t taskindex;
	for(taskindex=0; taskindex<ARRAY_SIZE; taskindex++)
	{
		if (tasks[taskindex].flags==1)                    // 查询任务时间是否到
		{
			(*tasks[taskindex].handle)();                  // 执行任务
			tasks[taskindex].flags = 0;                    // 消除任务标记位
		}
	}
}

void System_Run(void)
{
	Task_Handle();
	WDTR = 0x5a;
}

void Task_Init(void)
{
	tasks[0] = Task_Register(0, Work_Handler);					/* 打光 */
	tasks[1] = Task_Register(0, Charge_Handler);				/* 電容充電 */
	tasks[2] = Task_Register(0, Key_Handler);					/* 按鍵掃描 */
	tasks[3] = Task_Register(0, Rotary_Handler);				/* 旋鈕判斷 */
	tasks[4] = Task_Register(0, Touch_Key_Handler);				/* 觸摸按鍵掃描 */
	tasks[5] = Task_Register(0, VDD_Test_Handler);				/* 供電穩定檢測 */
	tasks[6] = Task_Register(0, Fan_Handler);					/* 風扇任務 */
	tasks[7] = Task_Register(0, NTC_Handler);					/* 溫度報警任務 */
	tasks[8] = Task_Register(0, Error_Det_Handler);				/* 錯誤報警任務 */
	tasks[9] = Task_Register(0, Display_Handler);				/* 顯示任務 */
	tasks[10] = Task_Register(0, Sleep_Handler);				/* 休眠任務 */
	tasks[11] = Task_Register(0, Touch_Handler);				/* 觸摸任務 */
	tasks[12] = Task_Register(0, Burnin_Handler);				/* 老化模式 */
}

void Task_Roll(void)
{
	tasks[9].flags = 1;
	switch(TIM_Period.systick)
	{
		case 1:
		//	tasks[9].flags = 1;
		case 10:
			tasks[0].flags = 1;//Work_Handler
			tasks[1].flags = 1;//Charge_Handler
			tasks[2].flags = 1;//Key_Handler
			tasks[3].flags = 1;//Rotary_Handler
			tasks[4].flags = 1;//Touch_Key_Handler
			tasks[11].flags = 1;//Touch_Handler
		break;
		case 5:
			tasks[5].flags = 1;//VDD_Test_Handler
		break;
		case 15:
			tasks[6].flags = 1;//Fan_Handler
			tasks[7].flags = 1;//NTC_Handler
			tasks[8].flags = 1;//Error_Det_Handler
		//	tasks[9].flags = 1;//Display_Handler
			tasks[10].flags = 1;//Sleep_Handler
			tasks[12].flags = 1;//Burnin_Handler
		break;
		default:break;
	}
}

idata uint8_t SupplyTestFinishedFlag = false;
void System_Init(void)
{
	//Delay500ms();
	TIMER0_Configuration();
	TIMER1_Configuration();
	TIMER2_Configuration();
	
	/* T2 L3 T0 L2 T1 L1 */
	IP1 |= 0x22;
	IP0 |= 0x28;
	
	RELEASE_400V_EN();
	ADCInit();
	Pwm_Init();
	Key_init();
	//UartInit();
	
	Touch_Key_Init();
	
	Work_Init();
	Charge_Init();
	Fan_Init();
	
	
//	NTC_Init();
	Burnin_init();
	Display_init();
	
	Task_Init();
	
	SupplyTestFinishedFlag = true;
}

//void Task_DeInit(void)
//{
//	uint8_t i;
//	
//	for(i=0;i<ARRAY_SIZE;i++)
//		tasks[i].flags = 0;
//}




