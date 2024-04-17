#include "rotary.h"

xRotaryStatus_t xRotaryStatus = {0};

uint8_t Rotary_Read(void)
{
	uint8_t a,b,c,d;
	
	a=ROTARY_0;
	b=ROTARY_1;
	c=ROTARY_2;
	d=((c<<2)+(b<<1)+(a<<0))&0xff;
	switch(d)
	{
		case 0x04:return ROTARY_LEVEL_OFF;
		case 0x05:return ROTARY_LEVEL_1;
		case 0x06:return ROTARY_LEVEL_2;
		case 0x07:return ROTARY_LEVEL_3;
		case 0x01:return ROTARY_LEVEL_4;
		case 0x02:return ROTARY_LEVEL_5;
		case 0x03:return ROTARY_LEVEL_6;
		default: return 0;
	}

}

void Rotary_Handler(void)
{
	if(ErrorType_Get()/*&(ERROR_TYPE_VOLTAGE|ERROR_TYPE_SUPPLY)*/)
		return;
	
	if(BurninEntering())
		return;
	
		
//		for(k=0;k<2;k++)
//			xRotaryStatus.bRKbuf = xRotaryStatus.bRKbuf[k+1];
	xRotaryStatus.bRKbuf = Rotary_Read();
//		for(k=0;k<2;k++){
//			if(xRotaryStatus.bRKbuf != xRotaryStatus.bRKbuf[k+1])
//				break;
//		}
	if(!b_BurninFlag_Get())
	{
		#if 0
		switch(xRotaryStatus.bRKbuf)
		{
			case 1:SMG_Write_Byte(LED_MODE_1);break;
			case 2:SMG_Write_Byte(LED_MODE_2);break;
			case 3:SMG_Write_Byte(LED_MODE_3);break;
			case 4:SMG_Write_Byte(LED_MODE_4);break;
			case 5:SMG_Write_Byte(LED_MODE_5);break;
			case 6:SMG_Write_Byte(LED_MODE_6);break;
			case 7:SMG_Write_Byte(LED_AUTO);break;
		}
		
		#else
		if(uc_Level_Get()){
			if(uc_Level_Get()!=xRotaryStatus.bRKbuf){
				if(xRotaryStatus.bRKbuf>0&&xRotaryStatus.bRKbuf<7){		//换挡
//						LED_SenseCnt_Reset();
//						LED_SensetypeFlag_Set(LED_SENSE_FLASH);
					if(b_SleepMode_Get()==SLEEP_MODE_SLEEP){
						//唤醒
						PowerOnOff_Set(1);
						set_led_event(LED_EVENT_MODE);
					}
					LED_SensetypeFlag_Set(LED_SENSE_ROTARY);
					uc_Level_Set(xRotaryStatus.bRKbuf);
					uc_LevelSum_Set(uc_Mode_Get()+6*(uc_Level_Get()-1));
					ul_SleepModeCnt_Set(0);
				}else if(xRotaryStatus.bRKbuf==7){		//关机
					b_SleepMode_Set(SLEEP_MODE_WAKE);
					PowerOnOff_Set(0);
					LED_SensetypeFlag_Set(LED_SENSE_ON);
					uc_Level_Set(0);
					uc_LevelSum_Set(uc_Mode_Get());
				}
			}
		}else{
			if(xRotaryStatus.bRKbuf>0&&xRotaryStatus.bRKbuf<7){		//开机
				uc_Level_Set(xRotaryStatus.bRKbuf);
				uc_LevelSum_Set(uc_Mode_Get()+6*(uc_Level_Get()-1));
				PowerOnOff_Set(1);
				if(b_IsPowerOnFlag_Get())
					LED_SensetypeFlag_Set(LED_SENSE_ROTARY);
				set_led_event(LED_EVENT_MODE);
			}
		}
		#endif
		b_IsPowerOnFlag_Set(1);
	}
	else
	{
		if((xRotaryStatus.bRKbuf!=0) && (b_BurninLastMode_Get()!=xRotaryStatus.bRKbuf))
		{
			if(b_BurninStart_Get())
			{
				LED_SensetypeFlag_Set(LED_SENSE_ON);
				PowerOnOff_Set(0);
			}
			if(xRotaryStatus.bRKbuf==7){
				//if(b_BurninMode_Get()){
					b_BurninStart_Set(0);
					b_BurninMode_Set(0);
				//}
			}else{
				//if(!b_BurninMode_Get()){
					b_BurninStart_Set(0);
					b_BurninMode_Set(1);
				//}
			}
			set_led_event(LED_EVENT_BURNIN);
			b_BurninLastMode_Set(xRotaryStatus.bRKbuf);
		}
	}
	
}



