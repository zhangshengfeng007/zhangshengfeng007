#ifndef _SYS_CLOCK_H_
#define _SYS_CLOCK_H_

//CKCON�Ĵ�������
#define IHCKE			(1<<7)			//IRCHʹ�ܿ���λ
#define ILCKE			(1<<6)			//IRCLʹ�ܿ���λ
#define TKCKE			(1<<5)			//TKʱ��ʹ�ܿ���λ
#define XLCKE			(1<<4)			//XLOSCʱ��ʹ�ܿ���λ
#define TMCS(N)			(N<<1)			//N=0-1,TMC����ʱ��ѡ��
#define SCKS(N)			(N<<0)			//N=0-1,ϵͳʱ��ѡ��λ

//SCKS
#define CKSEL_IRCH		0	
#define CKSEL_IRCL		1	


void Sys_Clk_Set_IRCH(void);
void Sys_Clk_Set_IRCL(void);
unsigned char ReadIHCFG(void);
unsigned char ReadTKCCFG(void);

#endif
