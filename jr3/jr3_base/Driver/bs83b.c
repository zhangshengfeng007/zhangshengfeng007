#include "bs83b.h"

#define		SDA_H()			(SDA=1)
#define		SDA_L()			(SDA=0)
#define		SCL_H()			(SCL=1)
#define		SCL_L()			(SCL=0)
#define		SDA_IN()		(P0M&=~0x04)
#define		SDA_OUT()		(P0M|=0x04)

//void Delay_us(uint8_t us);
//void Delay500ms()
//{
//	unsigned char i, j, k;

//	_nop_();
//	_nop_();
//	i = 63;
//	j = 179;
//	k = 82;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}

void Delay_us(uint8_t us)
{
	switch(us)
	{
		case 1:us=14;break;
		case 2:us=14;break;
		case 3:us=250;break;	//22
		default:break;
	}
	while(us--);
}

void IIC_Wave(void)
{
	//SCL_L();
	Delay_us(1);
	SCL_H();
	Delay_us(1);
}


uint8_t IIC_Test(void)
{
	//uint8_t i, j=0;
	uint16_t dat = 0x0000;
	
//	for(i=0;i<16;i++){
//		SCL_H();
//		Delay_us(1);
//		SCL_L();
//		if(i<8){
//			if(!SDA) dat|=0x01;
//			dat<<=1;
//		}
//		Delay_us(1);
//	}
	#if 1
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	dat<<=1;
	IIC_Wave();
	SCL_L();_nop_();
	if(SDA) dat|=0x01;
	//dat<<=1;
	IIC_Wave();
	#else
	#endif
	SCL_H();
	
	if((dat&0xff)==0xc2){
		dat >>= 8;
		dat = ~dat;
//		for(i=0x01;i!=0;i<<=1)
//		{
//			if(i&dat)
//				j++;
//		}
//		if(j==1)
			return dat;
//		else
//			return 0xff;
	}else
		return 0xff;
	
//	switch(dat)
//	{
//		case 0x05ff: dat = 0x0000;break;
//		case 0x70bf: dat = 0x0001;break;
//		case 0x785f: dat = 0x0002;break;
//		case 0x7f0b: dat = 0x0004;break;
//		case 0x617f: dat = 0x0008;break;
//		case 0x7f85: dat = 0x0010;break;
//		case 0x7e17: dat = 0x0020;break;
//		default: dat = 0x0000;break;
//	}
	
	//dat &= 0xfd;
	
}


void IIC_Init(void)
{
	SCL_H();
	Delay_us(2);
	SDA_H();
	Delay_us(2);
	SDA_IN();
}









