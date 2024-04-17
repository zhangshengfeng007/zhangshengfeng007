#include "74HC595.h"
#include "bs83b.h"

#define		SER_H()						(SER=1)
#define		SER_L()						(SER=0)
#define		SCK_H()						(SCK=1)
#define		SCK_L()						(SCK=0)
#define		RCK_H()						(RCK=1)
#define		RCK_L()						(RCK=0)

uint8_t LED_send_buf = 0;


void SMG_Write_Byte(uint8_t dat)
{
	uint8_t i;
	
//	PW1M &= 0x7f;
	/*
	EAL = 0;
	for(i=0x01;i!=0;i<<=1)
	{
		SCK_L();
		(dat&i) ? SER_H():SER_L();
		SCK_H();
		SCK_L();
		RCK_L();
		RCK_H();
		RCK_L();
	}
	EAL = 1;
	*/
//	EAL = 0;
//	for(i=0x01;i!=0;i<<=1)
//	{
//		SCK_L();
//		RCK_L();
//		(dat&i) ? SER_H():SER_L();
//		RCK_H();
//		SCK_H();
//	}
//	EAL = 1;

	EAL = 0;
	for(i=0x01;i!=0;i<<=1)
	{
		SCK_L();
		RCK_L();
		(dat&i) ? SER_H():SER_L();
		RCK_H();
		SCK_H();
	}
	EAL = 1;
//	RCK_L();
//	RCK_H();
	//SCK_H();
//	for(i=0x01;i!=0;i<<=1)
//	{
//		RCK_L();
//		SCK_L();
//		(dat&i) ? SER_H():SER_L();
//		SCK_H();
//		RCK_H();
////		RCK_H();
////		RCK_L();
//	}
	
//	for(i=0x01;i!=0x80;i<<=1)
//	{
//		(dat&i) ? SER_H():SER_L();
//		SCK_H();
//		SCK_L();
//		RCK_H();
//		RCK_L();
//	}
//	(dat&i) ? SER_H():SER_L();
//	for(i=0x01;i!=0;i<<=1)
//	{
//		(dat&i) ? SER_H():SER_L();
//		SCK_H();
//		SCK_L();
//		RCK_H();
//		RCK_L();
//	}
//	SCK_L();
//	RCK_L();
//	PW1M |= 0x80;
}







