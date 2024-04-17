#include "pwm_driver.h"

#define		PW1En		(1<<7)
#define		PWN1RATE	(0x07<<4)		//clock = Fosc/32
#define		PWN1RATE1M	(0x02<<4)		//clock = Fosc/32
#define		PWNV2		(0<<3)			//PWM20/21/22 conver
#define		PWNV1		(0<<2)			//PWM10/11/12 conver

void Pwm_Init(void)
{
	//PWM1_Initial
	PW1YH = 0x00;		//fpwm1 = 1M/20=50k
	PW1YL = 0x06;		//Fre = 0
	PW1DH = 0x00;
	PW1DL = 0x00;		//Duty = 1
	
	PW1BH = PW1DH;
	PW1BL = PW1DL;		//死区B
	PW1A = 0x00;		//死区A
	
	//PW1CH =  0x77;
	PW1CH = 0;
	PW1M = PW1En | PWN1RATE | PWNV2 | PWNV1;
	
	Pwm_Open_Charge();
}


void Pwm_Set(uint16_t Frq_value,uint16_t duty_value)
{
	uint16_t temp_cnt;
	
//	if(duty_value>42) b_ChargeBusy_Set(1);
//	else b_ChargeBusy_Set(0);
	
	temp_cnt = Frq_value;
	PW1YH = temp_cnt>>8;
	PW1YL = temp_cnt&0xff;
	//temp_cnt = temp_cnt*duty_value/Frq_value;
	temp_cnt = duty_value;
	PW1DH = temp_cnt>>8;
	PW1DL = temp_cnt&0xff;
	PW1BH = PW1DH;
	PW1BL = PW1DL;		//死区B
	PW1A = 0x00;		//死区A
	
	PW1M = PW1En | PWN1RATE | PWNV2 | PWNV1;
}








