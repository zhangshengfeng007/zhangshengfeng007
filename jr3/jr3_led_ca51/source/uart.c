#ifndef _UART_C_
#define _UART_C_

#include "include/ca51f1s6_config.h"		
#include "include/ca51f1s6sfr.h"
#include "include/ca51f1s6xsfr.h"
#include "include/gpiodef_f1s6.h"

#include "include/uart.h"
#include <stdarg.h>
#include <stdio.h>
/*********************************************************************************************************************/
/*********************************************************************************************************************/
//注意： 以下波特率参数对应主时钟为16M，如选择另外的主时钟频率，参数须重新计算。
code unsigned long int BR_TAB[]=
{
	1200,
	2400,
	4800,
	9600,
	19200,
	38400,
	57600,
	115200,
};

code unsigned int BR_SET_TAB[][2]=
{
#ifdef SYSCLK_16MHZ
	{416,31},
	{208,31},
	{104,31},
	{52 ,31},
	{26 ,31},
	{13 ,31},
	{10 ,27},
	{5  ,27},	
#else 		
	{208,31},
	{104,31},
	{52 ,31},
	{26 ,31},
	{13 ,31},
	{8  ,25}, 
	{5  ,27},
	{3  ,22}, 
#endif
};

#ifdef UART1_EN
void Uart1_Initial(unsigned long int baudrate)
{
	unsigned int value_temp;
	unsigned char i;

	uart1_send.head=0;
	uart1_send.tail=0;
	uart1_rev.head=0;
	uart1_rev.tail=0;
	uart1_tx_flag=0;

	for(i=0;i<sizeof(BR_TAB)/4;i++)
	{
		if(baudrate == BR_TAB[i])
		{
			value_temp = 0x400 - BR_SET_TAB[i][0];
			UDCKS1 = 0x80 | BR_SET_TAB[i][1]; 
			break;
		}
	}
	if(i == sizeof(BR_TAB)/4) return;	//如果所设置的波特率不在表格中，可自行添加。

	GPIO_Init(P31F,P31_UART1_RX_SETTING);
	GPIO_Init(P30F,P30_UART1_TX_SETTING);

	S1RELH = (unsigned char)(value_temp>>8);
	S1RELL = (unsigned char)(value_temp);
	
	S1CON = 0xD0;
	INT2EN = 1;		
}
void Uart1_PutChar(unsigned char bdat)
{
	unsigned char free_space;
	unsigned char tail_tmp;
	while(1)
	{		
		tail_tmp = uart1_send.tail;
		if(uart1_send.head < tail_tmp)
		{
			free_space = tail_tmp - uart1_send.head;
		}
		else
		{
			free_space = UART1_TX_BUF_SIZE + tail_tmp - uart1_send.head;
		}		
		if(free_space > 1)
		{
			INT2EN = 0; 
			uart1_send.head++;
			uart1_send.head %= UART1_TX_BUF_SIZE;
			uart1_tx_buf[uart1_send.head] = bdat;
			if(!uart1_tx_flag)
			{
				INT2EN = 1;
				uart1_send.tail++;
				uart1_send.tail %= UART1_TX_BUF_SIZE;		
				S1BUF = uart1_tx_buf[uart1_send.tail];				
				uart1_tx_flag = 1;		
			}
			else
			{
				INT2EN = 1;	
			}			
			break;
		}
	}
}
void UART1_ISR(void)
{
	if(S1CON & 0x01)
	{
		S1CON = (S1CON&~(0x03))|0x01;			 
		uart1_rev.head++;
	   	uart1_rev.head %= UART1_RX_BUF_SIZE;
		uart1_rx_buf[uart1_rev.head]=S1BUF;
	}
	if(S1CON & 0x02)
	{
		S1CON = (S1CON&~(0x03))|0x02;	
		if(uart1_send.head!=uart1_send.tail)
		{
			uart1_send.tail++;
			uart1_send.tail %= UART1_TX_BUF_SIZE;
			S1BUF=uart1_tx_buf[uart1_send.tail];				
		}
		else
		{
			uart1_tx_flag=0;
		}		
	}
}
#endif
#ifdef UART2_EN
void Uart2_Initial(unsigned long int baudrate)
{
	unsigned int value_temp,i;

	uart2_send.head=0;
	uart2_send.tail=0;
	uart2_rev.head=0;
	uart2_rev.tail=0;
	uart2_tx_flag=0;

	GPIO_Init(P00F,P00_UART2_TX_SETTING);
	GPIO_Init(P01F,P01_UART2_RX_SETTING);

	for(i=0;i<sizeof(BR_TAB)/4;i++)
	{
		if(baudrate == BR_TAB[i])
		{
			value_temp = 0x400 - BR_SET_TAB[i][0];
			UDCKS2 = 0x80 | BR_SET_TAB[i][1]; 
			break;
		}
	}
	if(i == sizeof(BR_TAB)/4) return;	//如果所设置的波特率不在表格中，可自行添加。

	S2RELH = (unsigned char)(value_temp>>8);
	S2RELL = (unsigned char)(value_temp);
	
	S2CON = 0xD0;
	INT4EN = 1;	
}
void Uart2_PutChar(unsigned char bdat)
{
	unsigned char free_space;
	unsigned char tail_tmp;
	while(1)
	{		
		tail_tmp = uart2_send.tail;
		if(uart2_send.head < tail_tmp)
		{
			free_space = tail_tmp - uart2_send.head;
		}
		else
		{
			free_space = UART2_TX_BUF_SIZE + tail_tmp - uart2_send.head;
		}		
		if(free_space > 1)
		{
			INT4EN = 0; 
			uart2_send.head++;
			uart2_send.head %= UART2_TX_BUF_SIZE;
			uart2_tx_buf[uart2_send.head] = bdat;			
			if(!uart2_tx_flag)
			{
				INT4EN = 1;
				uart2_send.tail++;
				uart2_send.tail %= UART2_TX_BUF_SIZE;			
				S2BUF = uart2_tx_buf[uart2_send.tail];				
				uart2_tx_flag = 1;		
			}
			else
			{
				INT4EN = 1;	
			}			
			break;
		}
	}
}
void UART2_ISR(void) 	
{
	if(S2CON & 0x01)
	{
		S2CON = (S2CON&~(0x03))|0x01;			 
		uart2_rev.head++;
		uart2_rev.head %= UART2_RX_BUF_SIZE;
		uart2_rx_buf[uart2_rev.head]=S2BUF;
	}
	if(S2CON & 0x02)
	{
		S2CON = (S2CON&~(0x03))|0x02;	
		if(uart2_send.head!=uart2_send.tail)
		{
			uart2_send.tail++;
			uart2_send.tail %= UART2_TX_BUF_SIZE;
			S2BUF=uart2_tx_buf[uart2_send.tail];				
		}
		else
		{
			uart2_tx_flag=0;
		}		
	}
}
#endif

#ifdef PRINT_EN
	#ifdef UART1_PRINT
		#define Uart_PutChar	Uart1_PutChar
	#elif defined  UART2_PRINT
		#define Uart_PutChar	Uart2_PutChar
	#endif
void UartPutStr(char *str)
{
	while(*str)
	{	
 		Uart_PutChar(*str++);
	}
}
void uart_printf(char *fmt,...) 
{
    va_list ap;
    char xdata string[128];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    UartPutStr(string);
    va_end(ap);
}
#endif
/*********************************************************************************************************************/
#endif