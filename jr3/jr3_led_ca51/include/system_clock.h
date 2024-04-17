#ifndef _SYS_CLOCK_H_
#define _SYS_CLOCK_H_

//CKCON寄存器定义
#define IHCKE			(1<<7)			//IRCH使能控制位
#define ILCKE			(1<<6)			//IRCL使能控制位
#define TKCKE			(1<<5)			//TK时钟使能控制位
#define XLCKE			(1<<4)			//XLOSC时钟使能控制位
#define TMCS(N)			(N<<1)			//N=0-1,TMC计数时钟选择
#define SCKS(N)			(N<<0)			//N=0-1,系统时钟选择位

//SCKS
#define CKSEL_IRCH		0	
#define CKSEL_IRCL		1	


void Sys_Clk_Set_IRCH(void);
void Sys_Clk_Set_IRCL(void);
unsigned char ReadIHCFG(void);
unsigned char ReadTKCCFG(void);

#endif
