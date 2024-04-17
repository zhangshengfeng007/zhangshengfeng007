#ifndef _Delay_C_
#define _Delay_C_
#include "include/ca51f1s6_config.h"	
/*********************************************************************************************************************/

void Delay_10us(unsigned int n)	   
{
	unsigned char i;	
#ifdef SYSCLK_16MHZ
	n *= 2;
#endif
	while(n--)
	{
		for(i=0;i<6;i++);
	}
}


void Delay_50us(unsigned int n)	   
{
	unsigned char i;	
#ifdef SYSCLK_16MHZ
	n *= 2;
#endif
	while(n--)
	{
		for(i=0;i<33;i++);
	}
}
void Delay_ms(unsigned int n)
{
	while(n--)
	{
		Delay_50us(20);
	}
}
/*********************************************************************************************************************/
#endif