#ifndef _MAIN_C_
#define _MAIN_C_
/*********************************************************************************************************************/
#include "include/ca51f1s6_config.h"		
#include "include/ca51f1s6sfr.h"
#include "include/ca51f1s6xsfr.h"
#include "include/gpiodef_f1s6.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/delay.h"

/*********************************************************************************************************************
	INT2~INT4控制例程
	1、INT0~1 支持上升沿、下降沿触发
	2、INT2~4 支持上升沿、下降沿、双沿触发
	3、INT0~3 中断输入引脚为固定引脚，
	4、INT4   可配置信号引脚(P1.0~P1.7/P2.0~P2.7/P0.4~P0.7/P3.0/P3.1)作为中断输入引脚
*********************************************************************************************************************/

enum 
{
	P10_INDEX = 0,
	P11_INDEX = 1,
	P12_INDEX = 2,
	P13_INDEX = 3,
	P14_INDEX = 4,
	P15_INDEX = 5,
	P16_INDEX = 6,
	P17_INDEX = 7,
	
	P20_INDEX = 8,
	P21_INDEX = 9,
	P22_INDEX = 10,
	P23_INDEX = 11,
	P24_INDEX = 12,
	P25_INDEX = 13,
	P26_INDEX = 14,
	P27_INDEX = 15,
	
	P04_INDEX = 16,
	P05_INDEX = 17,
	P06_INDEX = 18,
	P07_INDEX = 19,
	
	P30_INDEX = 20,
	P31_INDEX = 21,
};

#define EPIE(n)			(n<<7)
#define EPPL(n)			(n<<5)
#define EPPSEL(n)		(n<<0)

//  equal == 74HC595
#define ANA_74HC595_DATA    (P17)
#define ANA_74HC595_CLK   	(P15)

char recv_led_data;
char recv_index = 0;
unsigned char recv_frame_buff[10] = {0};  // 10times_data same ,display


char disp_led_data = 0xff;
char last_disp_led_data = 0;
char recv_flag;
char recv_cnt;
unsigned int time_out_cnt = 0;
unsigned int disp_delay_cnt = 0;
unsigned int disp_delay_flag = 0;



/*********************************************************************************************************************
	本例程以P0.2为例，设置P0.2为不同模式，所有IO用法与P0.2相同。
*********************************************************************************************************************/

void System_Init(void)
{
	LVDCON = 0xE2;					//设置LVD复位电压为2.7V
#ifdef SYSCLK_16MHZ					//系统时钟为16MHz,设置CKDIV为0
	CKDIV = 0;
#endif
	IHCFG = ReadIHCFG();
	TKCCFG = ReadTKCCFG();
#ifdef UART1_EN
	Uart1_Initial(UART1_BAUTRATE);
#endif	
#ifdef UART2_EN
	Uart2_Initial(UART2_BAUTRATE);
#endif
}


/*外部中断4控制例程****************************************************************************************************/
void INT4_Init(void)
{
	P15F = INPUT;					//INT4可配置信号引脚(P1.0~P1.7/P2.0~P2.7/P0.4~P0.7/P3.0/P3.1)作为中断输入引脚		
	EP2CON = EPIE(1) | EPPL(0) | EPPSEL(P15_INDEX); //使能外部中断，并选择下降沿触发, 设置P12为INT4中断引脚
	INT4EN = 1;						//外部中断4中断使能
	
	//int4_flag = 0;
}


void main(void)
{
	int  i = 0;
	System_Init();

	EA = 1;							//开全局中断

#ifdef PRINT_EN
	uart_printf("GPIO Demo Code\n");
#endif

	INT4_Init();
/***********************************************************************************/
//	P02F = OUTPUT;			//P02设置为推挽输出模式
//	while(1)
//	{
//		P02 = ~P02;			//P02取反输出
//	}
	
	P21F = OUTPUT;
	P22F = OUTPUT;
	P23F = OUTPUT;
	
	P24F = OUTPUT;
	P25F = OUTPUT;
	P26F = OUTPUT;
	
	P27F = OUTPUT;
	
/***********************************************************************************/		


/***********************************************************************************/	
//	P21 = 1;
//	P22 = 1;
//	P23 = 1;

//	P24 = 1;
//	P25 = 1;
//	P26 = 1;

//	P27 = 1;
//	P02F = INPUT;					//P02设置为输入模式
//	P02F = INPUT | PU_EN;			//P02设置为输入模式，并使能上拉电阻
		P15F = INPUT | PU_EN;	
		P16F = INPUT | PU_EN;	
		P17F = INPUT | PU_EN;	
	while(1)
	{
//		if(P02)
//		{
//		#ifdef PRINT_EN
//			uart_printf("P02 Input High!\n");
//		#endif
//		}
//		else
//		{
//		#ifdef PRINT_EN
//			uart_printf("P02 Input Low!\n");
//		#endif
//		}
		
		{
			time_out_cnt ++;


			if(recv_cnt == 8)
			{


				recv_index ++;
				recv_index %= 10;
				recv_frame_buff[recv_index] = recv_led_data;
				recv_cnt = 0;
				
				if(recv_index == 9)
				{
				
					#ifdef PRINT_EN
//						uart_printf("buff[]:%d,:%d ,:%d, %d,%d \r\n", recv_frame_buff[0], recv_frame_buff[1], recv_frame_buff[2], recv_frame_buff[3], 100);
					//uart_printf("   :%d,:%d ,:%d,%d,%d \r\n", recv_frame_buff[5], recv_frame_buff[6], recv_frame_buff[7], recv_frame_buff[8], recv_frame_buff[9]);
				//	Uart1_PutChar(recv_frame_buff[0]);
//					Uart1_PutChar(recv_frame_buff[1]);
//					Uart1_PutChar(recv_frame_buff[2]);
//					Uart1_PutChar(recv_frame_buff[3]);
				//	Uart1_PutChar(recv_frame_buff[4]);
//					Uart1_PutChar(recv_frame_buff[5]);
				//	Uart1_PutChar(recv_frame_buff[6]);
//					Uart1_PutChar(recv_frame_buff[7]);
//					Uart1_PutChar(recv_frame_buff[8]);
				//	Uart1_PutChar(recv_frame_buff[9]);
					#endif
				
					if((recv_frame_buff[0] == recv_frame_buff[1])\
						&& (recv_frame_buff[1] == recv_frame_buff[2])\
						&& (recv_frame_buff[2] == recv_frame_buff[3])\
						&& (recv_frame_buff[3] == recv_frame_buff[4])\
						&& (recv_frame_buff[4] == recv_frame_buff[5])\
						&& (recv_frame_buff[5] == recv_frame_buff[6])\
						&& (recv_frame_buff[6] == recv_frame_buff[7])\
						&& (recv_frame_buff[7] == recv_frame_buff[8])\
						&& (recv_frame_buff[8] == recv_frame_buff[9])\
						)
					{
					
						disp_delay_flag = 1;
						disp_led_data = recv_frame_buff[5];
					//	Uart1_PutChar(0xf0);
					}
				}
			}
			

			if(disp_delay_flag)
			{
				if(disp_led_data != last_disp_led_data)
				{
				
					(disp_led_data & 0x80) ? (P26 = 1) : (P26 = 0);//
					(disp_led_data & 0x40) ? (P27 = 1) : (P27 = 0); //
					(disp_led_data & 0x20) ? (P24 = 1) : (P24 = 0);
					(disp_led_data & 0x10) ? (P25 = 1) : (P25 = 0);
					
					(disp_led_data & 0x08) ? (P22 = 1) : (P22 = 0);//
					(disp_led_data & 0x04) ? (P21 = 1) : (P21 = 0);//
					(disp_led_data & 0x02) ? (P23 = 1) : (P23 = 0);//
					
					Uart1_PutChar(0x5a);
					Uart1_PutChar(0xa5);
					Uart1_PutChar(disp_led_data);
					last_disp_led_data = disp_led_data;
				}
				
				recv_cnt = 0;
				//recv_flag = 0;
				disp_delay_flag = 0;

			}

	}
		
	}
/***********************************************************************************/	
	
	
/***********************************************************************************/	
	//寄存器PnxC
	#define SMIT_EN(N)		(N<<6)		//N=0~1,为1输入的SMIT使能，为0输入是反相器使能
	#define SINK(N)			(N<<3)		//灌电流强度选择;P00C~P07C的SINK设置位为两位(SINK[1:0] = 0~3)，其它IO的SINK设置位为一位(SINK[0] = 0~1)
	#define SINK_EN(N)		(N<<2)		//N=0~1,大灌电流使能	
	#define DRV(N)			(N<<1)		//N=0~1,输出强度选择	
	#define SR(N)			(N<<0)		//N=0~1,输出斜率控制		0：最慢斜率控制   1：最快斜率控制
	
//P02设置为推挽输出，并且设置为大灌电流模式，程序如下：	
	P02F = OUTPUT;								//P00设置为推挽输出模式
	P02C = (P02C&0x40) | SINK(3) | SINK_EN(1);	//P00使能高灌电流模式,灌电流强度设置为最大
/***********************************************************************************/	
	
/***********************************************************************************/	
//P02设置为开漏输出，程序如下：	
	P02F = OP_EN | OUTPUT;
/***********************************************************************************/

/***********************************************************************************/	
//P02设置为开漏输出，并且打开上拉，程序如下：
	P02F = PU_EN | OP_EN | OUTPUT;
/***********************************************************************************/	

	while(1)
	{
		
	}
}
#endif
