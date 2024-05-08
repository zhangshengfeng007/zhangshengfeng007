#include "charge.h"
#include <stdlib.h>

//xdata uint16_t const Level_Duty[] = {			//电容目标电压
//										240,240,293,240,240,245,
//										272,272,310,272,272,259,
//										298,298,336,300,300,271,
//										320,320,340,320,320,273,
//										343,344,343,341,343,278,
//										362,364,364,362,362,298
//									};
xdata uint16_t const Level_Duty[] = {			//电容目标电压
										225,225,270,225,225,225,
										245,245,280,245,245,235,
										270,270,290,270,270,245,
										290,290,300,290,290,250,
										305,305,305,305,305,260,
										320,320,320,320,320,270
									};


xChargeStatus_t sx_ChargeStatus = {0};
uint8_t vrefunstableflag = false;
static xdata uint8_t lastvaluetestcount = 0;	/* 電壓異常計數 */
static xdata uint8_t supplytestcnt = 0;			/* 上電供電測試周期 */
static uint8_t vreftestcnt = 0;

void Charge_Duty_Set(uint8_t num)
{
	sx_ChargeStatus.ul_current_duty=Level_Duty[num];
}

void Charge_Handler(void)
{	
	uint8_t i;
	uint16_t tempbuf,tempmax=0,tempmin=4096;
	uint32_t supplybuf = 0;
	
	if(ErrorType_Get()/*&(ERROR_TYPE_VOLTAGE|ERROR_TYPE_SUPPLY)*/)
	{
		Pwm_Close_Charge();
		return;
	}
	if(b_SleepMode_Get()==SLEEP_MODE_SLEEP)
		return;
	
	#if 1		/* 電容電壓 */
	for(i=0;i<5;i++){
		tempbuf = Get_ADC_Value(ADCChannel400V);
		sx_ChargeStatus.ul_chargeBuff += tempbuf;
		if(tempmax<tempbuf)
			tempmax = tempbuf;
		if(tempmin>tempbuf)
			tempmin = tempbuf;
	}
	//test_chargeadc = (sx_ChargeStatus.ul_chargeBuff-tempmax-tempmin)/3;
	sx_ChargeStatus.ul_chargevalue = (uint16_t)((sx_ChargeStatus.ul_chargeBuff-tempmax-tempmin)/3 * ADCREFRATIO*(452.2/2.2)/4096.0 /1000);
	//if(sx_ChargeStatus.ul_chargevalue>=5)sx_ChargeStatus.ul_chargevalue -= 5;
	//sx_ChargeStatus.ul_chargevalue = (uint16_t)(test_chargeadc * ADCREFRATIO/4096.0);
	sx_ChargeStatus.ul_chargeBuff = 0;
	#endif
	
	#if 1		/* 供電 */
	tempmax = 0;
	tempmin = 0xffff;
	for(i=0;i<3;i++){
		tempbuf = Get_ADC_Value(ADCChannel12V);
		supplybuf += tempbuf;
		if(tempmax<tempbuf)
			tempmax = tempbuf;
		if(tempmin>tempbuf)
			tempmin = tempbuf;
	}
	supplybuf = (supplybuf-tempmax-tempmin);
	supplybuf = (uint16_t)(supplybuf * (ADCREFRATIO/4096.0 * 10.91));
	supplybuf += 30;
	#if 1		/* pwm */
	if(b_OnOffFlag_Get())
	{
		if(IsChargeDelayOK())
		{
			if(sx_ChargeStatus.ul_chargevalue < sx_ChargeStatus.ul_current_duty)
			{
//				if(supplybuf < 11400)//11000	//10400//8400
//				{
//					if(sx_ChargeStatus.uc_curpwmduty>=28)sx_ChargeStatus.uc_curpwmduty-=28;
//				}
//				else if(supplybuf > 11500)//11200	//10600//8600
//				{
//					if(supplybuf > 11800)i = 15;	//11800//11100//9100	//32
//					else if(supplybuf > 11700)i = 12;	//11500//11000//9000	//28
//					else if(supplybuf > 11600)i = 10;	//11400//10900//8900	//24
//					else i = 8;		//20
//					if(sx_ChargeStatus.uc_curpwmduty<401-i)sx_ChargeStatus.uc_curpwmduty+=i;//401		/* duty+1 */
//				}
				if(supplybuf < 11500)//11000	//10400//8400
				{
					if(sx_ChargeStatus.uc_curpwmduty>=28)sx_ChargeStatus.uc_curpwmduty-=28;
				}
				else if(supplybuf > 11600)//11200	//10600//8600
				{
					if(supplybuf > 11900)i = 10;	//11800//11100//9100	//32
					else if(supplybuf > 11800)i = 8;	//11500//11000//9000	//28
					else if(supplybuf > 11700)i = 6;	//11400//10900//8900	//24
					else i = 4;		//20
					if(sx_ChargeStatus.uc_curpwmduty<401-i)sx_ChargeStatus.uc_curpwmduty+=i;//401		/* duty+1 */
				}
			}
			else if(sx_ChargeStatus.ul_chargevalue > sx_ChargeStatus.ul_current_duty+5)
			{
				sx_ChargeStatus.uc_curpwmduty = 0;
			}
			else
			{
				sx_ChargeStatus.uc_curpwmduty = 8;
			}
			if(sx_ChargeStatus.ul_chargevalue >= sx_ChargeStatus.ul_current_duty-2)b_ChargeOkFlag_Set(1);
			else if(sx_ChargeStatus.ul_chargevalue < sx_ChargeStatus.ul_current_duty-5)b_ChargeOkFlag_Set(0);
			Pwm_Set(CHARGE_FRE,sx_ChargeStatus.uc_curpwmduty);
		}
		else
			sx_ChargeStatus.uc_curpwmduty = 100;//160	//CHARGE_FRE/2-CHARGE_FRE/100
	}
	#elif 0
	UartSendByte((uint8_t)(supplybuf/10000+48));
	UartSendByte((uint8_t)(supplybuf%10000/1000+48));
	UartSendByte((uint8_t)(supplybuf%1000/100+48));
	UartSendByte((uint8_t)(supplybuf%100/10+48));
	UartSendByte((uint8_t)(supplybuf%10+48));
	UartSendByte('\r');
	UartSendByte('\n');
	#endif
	
	#if 1
	if(b_DetInitFlag_Get()==DETECTIVE_INIT_FINISH && (sx_ChargeStatus.ul_chargevalue==0 || sx_ChargeStatus.ul_chargevalue>400) && !BurninEntering())
	{
//				tempvrefdata = Get_ADC_Value(0x0c);
//				
//					if(!Supply_Test()&&tempvrefdata>4000)
//						sx_ChargeStatus.uc_supplytestandlastvaluetestcnt++;
		if(!vrefunstableflag)
			lastvaluetestcount++;
		//tempbuf = Get_ADC_Value(ADCChannel400V);
		//sx_ChargeStatus.ul_chargevalue = (uint16_t)((tempbuf-tempmax-tempmin)/3 * ADCREFRATIO*(452.2/2.2)/4096.0 /1000);
		//if(sx_ChargeStatus.ul_chargevalue==0 || sx_ChargeStatus.ul_chargevalue>440){
		if(lastvaluetestcount==100){
			b_ChargeOkFlag_Set(1);
			ErrorType_Set(ERROR_TYPE_VOLTAGE);
			PowerOnOff_Set(0);
		}
		//}
	}else// if(b_DetInitFlag_Get()==DETECTIVE_INIT_FINISH)
		lastvaluetestcount = 0;
	#endif
	
	
	
	#endif
	
}


void VDD_Test_Handler(void)
{
//	uint8_t i;
//	uint16_t tempbuf,tempmax=0,tempmin=4096;
	uint32_t supplybuf = 0;
	
	VREFH |= 0x04;		/* Vref = VDD  AIN14 = 2V */
	VREFH &= ~0x03;
	
//	for(i=0;i<3;i++){
//		tempbuf = Get_ADC_Value(ADCChannel12);
//		supplybuf += tempbuf;
//		if(tempmax<tempbuf)
//			tempmax = tempbuf;
//		if(tempmin>tempbuf)
//			tempmin = tempbuf;
//	}
//	supplybuf = (supplybuf-tempmax-tempmin);
	
	supplybuf = Get_ADC_Value(ADCChannel12);
	supplybuf = (uint16_t)(8192000/supplybuf);
	if(supplybuf < 3500 || supplybuf > 6000)
		vrefunstableflag = true;
	else if(supplybuf > 4000)
		vrefunstableflag = false;
	VREFH &= ~0x04;
	VREFH = VREFH &~0x01 |0x02;
}

/* 開機供電波動 */
static uint16_t vreftesttimeout = 0;
void Charge_Init(void)
{
	uint8_t vrefstabletimes = 0;
	uint16_t tempbuf=0, tempmax=0, tempmin=0xffff;
	uint32_t curvalue=0;
	
	sx_ChargeStatus.ul_current_duty=Level_Duty[0];
	sx_ChargeStatus.uc_curpwmduty = 0;
	
	/* 上電穩壓檢測 */
	#if 0
	VREFH |= 0x04;		/* Vref = VDD  AIN14 = 2V */
	VREFH &= ~0x03;
	while(1)
	{
		if(vreftestcnt)
		{
			vreftesttimeout++;
			curvalue = (uint16_t)(4096000.0/Get_ADC_Value(ADCChannel12)*2);		/* 计算公式：VDD = 4096/r_ad_value3 *(2V/3V/4V) */
			if(curvalue < 3500){if(vrefstabletimes)vrefstabletimes--;}
			else if(curvalue > 6000){if(vrefstabletimes)vrefstabletimes--;}
			else if(curvalue > 4000){vrefstabletimes++;vreftesttimeout = 0;}
			if(vrefstabletimes >= 100)break;
			vreftestcnt = 0;
			if(vreftesttimeout >= 5000)
			{
//				ErrorType_Set(ERROR_TYPE_SUPPLY);
//				Display_Event_Set(LED_EVENT_ERROREN, 0);
//				break;
				SRST = 0x01;SRST = 0x01;
			}
			
			break;
		}
	}
	VREFH &= ~0x04;
	VREFH = VREFH &~0x01 |0x02;
	#endif
	
	/* 供電檢測 */
	#if 0
	curvalue = 0;
	tempmax = 0;
	tempmin = 0xffff;
	for(i=0;i<10;i++){
		tempbuf = Get_ADC_Value(ADCChannel12V);
		curvalue += tempbuf;
		if(tempmax<tempbuf)
			tempmax = tempbuf;
		if(tempmin>tempbuf)
			tempmin = tempbuf;
	}
	curvalue = (curvalue-tempmax-tempmin)>>3;
	curvalue = (uint16_t)(curvalue * (ADCREFRATIO/4096.0));
	
	
	if(curvalue < 916)		/* 18V */
	{
		ErrorType_Set(ERROR_TYPE_SUPPLY);
		//Display_Event_Set(LED_EVENT_ERROREN, 0);
	}
	#endif
	
	//Pwm_Open_Charge();
}

void ChargeInit_Cb(void)
{
	if(vreftestcnt<100)vreftestcnt++;
}







