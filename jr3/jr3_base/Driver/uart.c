#include "uart.h"


#define				L_S0RELH_4800				0x03
#define				L_S0RELL_4800				0x98
#define				L_S0RELH_9600				0x03
#define				L_S0RELL_9600				0xcc
#define				L_S0RELH_19200				0x03
#define				L_S0RELL_19200				0xe6
#define				L_S0RELH_38400				0x03
#define				L_S0RELL_38400				0xf3
#define				L_S0RELH_56000				0x03
#define				L_S0RELL_56000				0xee
#define				L_S0RELH_57600				0x03
#define				L_S0RELL_57600				0xef
#define				L_S0RELH_115200				0x03
#define				L_S0RELL_115200				0xf7

void UartInit(void)
{
	P00 = 1;
	P0M |= 0x01; 																	//UTX初始化输出高	
	P0M &= 0xfd; 																	//URX初始化数模模式
	
	SM0 = 0;																			//模式1
	SM1 = 1;
	REN0 = 1;																			//使能接收功能
	
	PCON &= 0x7f;       													
	BD = 1;				
	S0RELH = L_S0RELH_38400;
	S0RELL = L_S0RELL_38400;
	
	//ES0 = 1;
}

void UartSendByte(uint8_t txbuf)
{
	S0BUF = txbuf;
	while(!TI0);
	TI0 = 0;
}

//void UartIsr(void) interrupt ISRUart
//{
//	if(RI0 == 1)
//	{
//		RI0 = 0;
//		f_rx = 1;
//		r_urat_rx_data[r_uart_rx_length++] = S0BUF;
//	}
//}





