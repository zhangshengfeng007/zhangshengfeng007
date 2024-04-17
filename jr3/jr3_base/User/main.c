#include "system.h"


extern void Delay500ms(void);
extern idata uint8_t SupplyTestFinishedFlag;
int main()
{
	SYSCLK_Init();
	GPIO_Init();
	System_Init();
//	if(!BurninEntering())
//		Delay500ms();
//	else
//		SupplyTestFinishedFlag = true;
//	FAN_ON();
//	Pwm_Open_Charge();
	//IIC_Init();
	//System_Init();
	//Pwm_Set(CHARGE_FRE,1);
//	UART_Init();
	
	//System_Run();
	while(1)
	{
		//testadc = (uint16_t)(Get_ADC_Value(ADCChannel12V) * (ADCREFRATIO/4096.0));
//		if(testbuf==100){
			
//			IPL = 1;
//			testflag = 1;
//			TR0 = 1;
//			while(testflag);
//			IPL = 0;
			//SMG_Write_Byte(Rotary_Read());
//			temp_adc = Get_ADC_Value(ADCChannel400V);
//			i=!i;
//			temptestduty+=10;
//			Pwm_Set(CHARGE_FRE,temptestduty);
//			if(temptestduty==50) temptestduty=0;
//			
//			//FAN_HALFSPEED();
//			if(i)	P06=0;
//			else 	P06=1;
//			if(i==5) {testbuf=5000;i=0;}
//			Charge_Handler();
//			UART_WriteByte(test_chargeadc/1000+48);
//			UART_WriteByte(test_chargeadc%1000/100+48);
//			UART_WriteByte(test_chargeadc%100/10+48);
//			UART_WriteByte(test_chargeadc%10+48);
//			UART_WriteByte('\r');
//			UART_WriteByte('\n');
//			testbuf=0;
			
//		}
//		if(testbuf==5000){
//			FAN_HALFSPEED();
//			Pwm_Close_Charge();
//		}
//		if(testbuf==10000){
//			FAN_OFF();
//		}
//		if(testbuf==15000){
//			Pwm_Open_Charge();
//			FAN_ON();
//			testbuf=0;
//		}
		System_Run();
		WDTR = 0x5A;
//		if(KEY_LIGHT)SMG_Write_Byte(0x20);
//		else SMG_Write_Byte(0x00);
	}
	
	return 0;
}