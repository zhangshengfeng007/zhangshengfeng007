#include "ntc.h"

xdata xNTCStatus_t_ sxNTCStatus = {0};

#define		NTCOVERVALUE		500
#define		NTCNORMALVALUE		2000

static uint8_t errortimes = 0;
static uint8_t normaltimes = 0;

void NTC_Handler(void)
{
	uint16_t tempbuf;
	
	if(ErrorType_Get()&(~ERROR_TYPE_NTC))return;
	if(BurninEntering()||b_BurninFlag_Get())return;
	if(vrefunstableflag)return;
	
	if((b_DetInitFlag_Get()==DETECTIVE_INIT_FINISH || ErrorType_Get()&ERROR_TYPE_NTC) && !b_ChargeBusy_Get())
	{
		tempbuf = (uint16_t)(Get_ADC_Value(ADCChannelNTC) * (ADCREFRATIO/4096.0));
//		for(i=0;i<2;i++)
//			sxNTCStatus.ntcbuff[i] = sxNTCStatus.ntcbuff[i+1];
//		sxNTCStatus.ntcbuff[i] = tempbuf;
//		for(i=0;i<3;i++)
//			tempbuf += sxNTCStatus.ntcbuff[i];
		sxNTCStatus.ntcvalue = tempbuf;
		if(sxNTCStatus.ntcvalue<NTCOVERVALUE){
			normaltimes = 0;
			if(errortimes++ >= 50)
			{
				errortimes = 0;
				ErrorType_Set(ERROR_TYPE_NTC);
				PowerOnOff_Set(0);
			}
		}else if(sxNTCStatus.ntcvalue>NTCNORMALVALUE){
			errortimes = 0;
			if(ErrorType_Get()&ERROR_TYPE_NTC){
				if(normaltimes++ >= 50)
				{
					normaltimes = 0;
					ErrorType_Reset(ERROR_TYPE_NTC);
					if(uc_Level_Get()){
						PowerOnOff_Set(1);
						b_IsPowerOnFlag_Set(1);
						set_led_event(LED_EVENT_CLEAR);
						reset_led_event(LED_EVENT_OFF);
						//set_led_event(LED_EVENT_MODE);
					}
				}
				
			}
		}
	}
}


//void NTC_Init(void)
//{
//	
//}


