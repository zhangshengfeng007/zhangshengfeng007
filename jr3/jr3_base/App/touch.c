#include "touch.h"


#define		LIGHTTOUCH		(20/10)

xTouchStatus_t sx_TouchStatus = {0};

void Touch_Handler(void)
{
	if(ErrorType_Get()/*&(ERROR_TYPE_VOLTAGE|ERROR_TYPE_SUPPLY)*/)
		return;
	
	if(BurninEntering() || b_BurninFlag_Get())
		return;
	
	if(b_SleepMode_Get()==SLEEP_MODE_SLEEP)
		return;
	
//	if(!b_FightLighTouchFanBusy_Get())
//	{
		//if(1/*TOUCH==0*/)
		if(TOUCH==0)
		{
			if(sx_TouchStatus.uc_TouchTimeCnt++>=LIGHTTOUCH)
			{
				sx_TouchStatus.uc_TouchTimeCnt = 0;
				if(LED_SensetypeFlag_Get()==LED_SENSE_ON && b_OnOffFlag_Get())
					LED_SensetypeFlag_Set(LED_SENSE_TOUCH);
//				if(b_OnOffFlag_Get()&&!sx_TouchStatus.b_TouchFlag)
//					set_led_event(LED_SENSE_TOUCH);
				//sl_AutolightTime_Set(0);
				ul_SleepModeCnt_Set(0);
				sx_TouchStatus.b_TouchFlag=true;
			}
//			else if(sx_TouchStatus.uc_TouchTimeCnt>=LIGHTTOUCH*2)
//				sx_TouchStatus.uc_TouchTimeCnt = 0;
		}
		else
		{
			if(sx_TouchStatus.uc_TouchTimeCnt++>=LIGHTTOUCH){
				sx_TouchStatus.uc_TouchTimeCnt= 0;
				if(LED_SensetypeFlag_Get()==LED_SENSE_TOUCH && b_OnOffFlag_Get())
					LED_SensetypeFlag_Set(LED_SENSE_ON);
//				if(b_OnOffFlag_Get()&&sx_TouchStatus.b_TouchFlag)
//					set_led_event(LED_EVENT_SENCE);
				sx_TouchStatus.b_TouchFlag=false;
			}
			
		}
//	}
}








