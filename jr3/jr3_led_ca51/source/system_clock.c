#ifndef _SYSTEM_CLOCK_C_
#define _SYSTEM_CLOCK_C_
/*********************************************************************************************************************/
#include "include/ca51f1s6_config.h"		
#include "include/ca51f1s6sfr.h"
#include "include/ca51f1s6xsfr.h"
#include "include/gpiodef_f1s6.h"

#include "include/system_clock.h"
#include "include/delay.h"
/*********************************************************************************************************************/


/***********************************************************************************
������  ��		Sys_Clk_Set_IRCH										 	
����������		����ϵͳʱ��ΪIRCH
���������		��
����ֵ  ��		��
***********************************************************************************/
void Sys_Clk_Set_IRCH(void)
{
	CKCON |= IHCKE;											//IRCHʱ��ʹ��
	CKCON = (CKCON&0xFE) | CKSEL_IRCH;						//ϵͳʱ���л���IRCH
}
/***********************************************************************************/


/***********************************************************************************
������  ��		Sys_Clk_Set_IRCL										 	
����������		����ϵͳʱ��ΪIRCL
���������		��
����ֵ  ��		��
***********************************************************************************/
void Sys_Clk_Set_IRCL(void)
{
	CKCON |= ILCKE;											//IRCLʱ��ʹ��
	Delay_ms(1);											//ʹ��IRCL����ʱ1ms���ȴ�IRCL�ȶ�
	CKCON = (CKCON&0xFE) | CKSEL_IRCL;						//ϵͳʱ���л���IRCL	
}
/***********************************************************************************/


/***********************************************************************************
������  ��		ReadIHCFG										 	
����������		
���������		��
����ֵ  ��		��
***********************************************************************************/
unsigned char ReadIHCFG(void)    
{
	unsigned char D1,D2;
	FSCMD = 0x80; 
	PTSH = 0x00;    
	PTSL = 0x08;           
	FSCMD = 0x81;      
	D1 = FSDAT;
	D2 = FSDAT;
	FSCMD = 0;
	if(D1 == 'H')
	{
		return D2;
	}
	return IHCFG;
}


/***********************************************************************************
������  ��		ReadTKCCFG										 	
����������		
���������		��
����ֵ  ��		��
***********************************************************************************/
unsigned char ReadTKCCFG(void)    
{
	unsigned char D1,D2;
	FSCMD = 0x80; 
	PTSH = 0x00;    
	PTSL = 0x06;           
	FSCMD = 0x81;      
	D1 = FSDAT;
	D2 = FSDAT;
	FSCMD = 0;
	if(D1 == 'F')
	{
		return D2;
	}
	return TKCCFG;
}
/*********************************************************************************************************************/
#endif