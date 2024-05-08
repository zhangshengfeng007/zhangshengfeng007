#include "timer.h"

#define		T1Fre			(65535-1000*32/12)
#define		T1GATE			(0<<7)
#define		T1CT			(0<<6)			//Ftimer = Fcpu / 12
#define		T1M				(0x01<<4)		//16 位向上计数定时器

#define		T0Fre			(62868)		//2ms
#define		T0GATE			(0<<3)
#define		T0CT			(0<<2)
#define		T0M				(0x01<<0)

#define		T2Fre			(65268)		//2ms 65535-50*32/12

void Timer2ISR(void) interrupt ISRTimer2
{// TF0 clear by hardware
	if(TF2){
		TF2 = 0;
		TH2 = T2Fre/256;
		TL2 = T2Fre%256;
		Display_Breath_Cb();
	}
}
void Timer0ISR(void) interrupt ISRTimer0
{// TF0 clear by hardware
	TR0 = 0;
	TH0 = T0Fre/256;
	TL0 = T0Fre%256;
	Work_Cb();
}

extern idata uint8_t SupplyTestFinishedFlag;
void Timer1ISR(void) interrupt ISRTimer1
{// TF1 clear by hardware
	TH1 = T1Fre/256;
	TL1 = T1Fre%256;
	
	ChargeInit_Cb();
	BurninInit_Cb();
	
	//P05^=1;
	
	if(SupplyTestFinishedFlag)Task_Roll();
	else return;
	
	if(++TIM_Period.systick > 20)TIM_Period.systick = 0;
	
	Display_Cb();
	FanWork_Cb();
}

void TIMER2_Configuration(void)
{
	// T1_Initial
	TH2 = T2Fre/256;
	TL2 = T2Fre%256;
	
	T2CON = 0x05;
	
	//IEN1 |= 0x80;
	
	TF2 = 0;
	
	// Enable total/Timer2 interrupt
	ET2 = 1;
	
}

void TIMER1_Configuration(void)
{
	// T1_Initial
	TH1 = T1Fre/256;
	TL1 = T1Fre%256;
	// T0 mode0 with gating clock by INT0, clock source from Fosc or FRTC
	TMOD |= T1GATE|T1CT|T1M;
	// Timer 1 enable. Clear TF1
	TR1 = 1;
	TF1 = 0;
	// Enable T0/T1 interrupt
	ET1 = 1;
	//Enable total interrupt
	EAL = 1;
}

void TIMER0_Configuration(void)
{
	// T1_Initial
	TH0 = T0Fre/256;
	TL0 = T0Fre%256;
	// T0 mode0 with gating clock by INT0, clock source from Fosc or FRTC
	TMOD |= T0GATE|T0CT|T0M;
	// Timer 1 enable. Clear TF1
	TR0 = 0;
	TF0 = 0;
	// Enable T0/T1 interrupt
	ET0 = 1;
	//Enable total interrupt
	EAL = 1;
}


