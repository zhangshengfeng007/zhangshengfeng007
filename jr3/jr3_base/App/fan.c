#include "fan.h"

#define		ROTATESLEEPKEEPTIME		(60000/20)

xdata xFanStatus_t_ xFanStatus = {0};

#define		FANMAXCURRENT		400
#define		FANMINCURRENT		50

void Fan_Handler(void)
{
	//uint8_t i;
	
	//风扇异常测试
//		if(b_FightLighTouchFanBusy_Get()){
//			xFanStatus.ul_fanTestTimeCnt = 0;
//			xFanStatus.ul_RotateCntBuff = 0;
//		}
	
	if(ErrorType_Get()&(~ERROR_TYPE_FAN))
		return;
	if(BurninEntering()||b_BurninFlag_Get())
		return;
	
	#if 1
	if(!b_ChargeBusy_Get())
	{
		if(vrefunstableflag || b_FightLighTouchFanBusy_Get())return;
		//for(i=0;i<3;i++)
//			xFanStatus.ul_fanadcbuff = Get_ADC_Value(ADCChannelFAN);
//		xFanStatus.ul_fanadcvalue = (uint16_t)((xFanStatus.ul_fanadcbuff) * ADCREFRATIO/4096.0);
//		xFanStatus.ul_fanadcbuff = 0;
		xFanStatus.ul_fanadcbuff = Get_ADC_Value(ADCChannelFAN);
		xFanStatus.ul_fanadcvalue = (uint16_t)((xFanStatus.ul_fanadcbuff) * 125.0/128.0);
		xFanStatus.ul_fanadcbuff = 0;
		#if 1
		if(((b_DetInitFlag_Get()==DETECTIVE_INIT_FINISH) || (ErrorType_Get()&ERROR_TYPE_FAN)) )
		{
			if(xFanStatus.ul_fanadcvalue<FANMINCURRENT || xFanStatus.ul_fanadcvalue>FANMAXCURRENT)
			{
				xFanStatus.uc_RotateAbnormalTimes++;
				xFanStatus.uc_RotateNormalTimes = 0;
				if(xFanStatus.uc_RotateAbnormalTimes>=50){
					xFanStatus.uc_RotateAbnormalTimes = 0;
					xFanStatus.b_RotateErrorFlag = true;
					ErrorType_Set(ERROR_TYPE_FAN);
					//关机
					PowerOnOff_Set(0);
					b_SleepMode_Set(SLEEP_MODE_WAKE);
					FightLightReset();
				}
				//PA1 = 0;
			}
			else{		//5s自动退出(测试)
				xFanStatus.uc_RotateNormalTimes++;
				xFanStatus.uc_RotateAbnormalTimes = 0;
				if(xFanStatus.uc_RotateNormalTimes>=50){
					xFanStatus.uc_RotateNormalTimes = 0;
					if(xFanStatus.b_RotateErrorFlag){
						ErrorType_Reset(ERROR_TYPE_FAN);
						if(uc_Level_Get()){
							PowerOnOff_Set(1);
							b_IsPowerOnFlag_Set(1);
							reset_led_event(LED_EVENT_OFF);
							set_led_event(LED_EVENT_MODE);
						}
					}
					xFanStatus.b_RotateErrorFlag = false;
					//PA1 = 1;
				}
			}
		}
		#endif
	}
	#endif
	//关机运行时间
	if(xFanStatus.b_RotateKeepworkFlag==FAN_OFF)
		xFanStatus.ul_RotateKeepworkCnt++;
	else
		xFanStatus.ul_RotateKeepworkCnt=0;
	//休眠判断
	if(xFanStatus.ul_RotateKeepworkCnt > ROTATESLEEPKEEPTIME){
		xFanStatus.b_RotateKeepworkFlag = FAN_BOOT;
		FAN_OFF();
	}
}



void FanWork_Cb(void)
{
	if(xFanStatus.b_halfspeedflag)
	{
		if(xFanStatus.uc_half_worktime_count++ == 3)FAN = 1;
		else if(xFanStatus.uc_half_worktime_count == 10){FAN = 0;xFanStatus.uc_half_worktime_count = 0;}
	}
}


void Fan_Init(void)
{
	
}

