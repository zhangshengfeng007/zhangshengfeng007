#ifndef _INTERRUPT_C_
#define _INTERRUPT_C_

/*********************************************************************************************************************/
#include "include/ca51f1s6_config.h"		
#include "include/ca51f1s6sfr.h"
#include "include/ca51f1s6xsfr.h"
#include "include/gpiodef_f1s6.h"

#include "include/uart.h"
/*********************************************************************************************************************/


extern char recv_led_data;
//extern char disp_led_data = 0xff;
extern char recv_flag;
extern char recv_cnt;
extern unsigned int time_out_cnt;
extern char recv_frame_buff[10];
extern char recv_index;



void INT2_ISR (void) interrupt 4 
{
#ifdef UART1_EN
	UART1_ISR();
#endif
}

void INT4_ISR(void) interrupt 6 
{
#ifdef UART2_EN
	UART2_ISR();
#endif
	if(EPIF & 0x04)					//判断外部中断4中断标志
	{
		EPIF = 0x04;				//中断标志写1清0
		
		//int4_flag = 1;

			time_out_cnt = 0;
		
			if(recv_cnt < 8)
			{
				recv_led_data >>= 1;
				if(P17)	// data
				{
					recv_led_data |= 0x80;
				}
				else
				{
					recv_led_data &= ~0x80;
				}	
//				if(recv_cnt % 8 == 0)
//				{
//					recv_frame_buff[recv_index % 10] = recv_led_data;
//					recv_index ++;
//				}
				
				
//				recv_frame_buff[recv_cnt/8] >>= 1;
//			  if(P17)	// data
//				{
//					recv_frame_buff[recv_cnt/8] |= 0x80;
//				}
//				else
//				{
//					recv_frame_buff[recv_cnt/8] &= ~0x80;
//				}		
//				
				
				recv_cnt ++;
			}
	}
	
	
}

/*********************************************************************************************************************/
#endif
