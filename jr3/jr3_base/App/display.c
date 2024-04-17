#include "display.h"

xdata xDisplayStatus_t sx_DisplayStatus_t={0};

extern uint8_t errordisplay_buf;
void Display_Handler(void)
{
	uint8_t displayflag = false;
	
//	if(sx_DisplayStatus_t.b_Event){
//		if(vrefunstableflag)return;
//	}
	if(b_IPLWorking_Get())return;
	
	if(!ErrorType_Get()/*sx_DisplayStatus_t.b_ScanCnt==10*/)
	{
		if(sx_DisplayStatus_t.b_Event & LED_EVENT_CLEAR){
			sx_DisplayStatus_t.b_Event &= ~LED_EVENT_CLEAR;
			LED_send_buf&=~(LED_EVENT_MODE|LED_AUTO);
		}
		#if 1
		//if(b_DetInitFlag_Get()==DETECTIVE_INIT_FINISH){
			if(sx_DisplayStatus_t.b_Event & LED_EVENT_MODE)		//模式灯
			{
				sx_DisplayStatus_t.b_Event &= ~LED_EVENT_MODE;
				LED_send_buf&=~LED_MODE;
				switch(uc_Mode_Get())
				{
					case 0:LED_send_buf|=LED_MODE_1;break;
					case 1:LED_send_buf|=LED_MODE_2;break;
					case 2:LED_send_buf|=LED_MODE_3;break;
					case 3:LED_send_buf|=LED_MODE_4;break;
					case 4:LED_send_buf|=LED_MODE_5;break;
					case 5:LED_send_buf|=LED_MODE_6;break;
				}
				displayflag = true;
//				SMG_Write_Byte(LED_send_buf);
//				return;
			}
			if(sx_DisplayStatus_t.b_Event & LED_EVENT_AUTO)		//自动打光灯
			{
				sx_DisplayStatus_t.b_Event &= ~LED_EVENT_AUTO;
				LED_send_buf&=~LED_AUTO;
				if(b_AutoSwitchFlag_Get())
					LED_send_buf|=LED_AUTO;
				displayflag = true;
//				SMG_Write_Byte(LED_send_buf);
//				return;
			}
		//}
		if(sx_DisplayStatus_t.b_Event & LED_EVENT_BURNIN)		//老化模式
		{
			sx_DisplayStatus_t.b_Event &= ~LED_EVENT_BURNIN;
			LED_send_buf&=~LED_ALL;
			LED_send_buf|=LED_ALL;
			if(!b_BurninMode_Get())
				LED_send_buf&=~LED_MODE_6;
			else
				LED_send_buf&=~LED_AUTO;
			displayflag = true;
//			SMG_Write_Byte(LED_send_buf);
//			return;
		}
		if(sx_DisplayStatus_t.b_Event & LED_EVENT_OFF)		//关闭所有灯
		{
			sx_DisplayStatus_t.b_Event &= ~LED_EVENT_OFF;
			LED_send_buf&=(LED_OFF);
			LEDB = 1;
			displayflag = true;
//			SMG_Write_Byte(LED_send_buf);
//			return;
		}
		if(sx_DisplayStatus_t.b_sensetype==LED_SENSE_SLEEP)		//呼吸灯
		{
			sx_DisplayStatus_t.b_Event &= ~LED_EVENT_SENCE;
//			if(/*sx_DisplayStatus_t.b_senseonoffflag&&*/sx_DisplayStatus_t.b_sensebreathduty!=0&&sx_DisplayStatus_t.b_sensebreathduty!=20){
//			//if(sx_DisplayStatus_t.uc_senseonoffcnt<100/*&&!sx_DisplayStatus_t.b_senseonoffflag*/)
//				//sx_DisplayStatus_t.uc_senseonoffcnt++;
//				if(sx_DisplayStatus_t.uc_senseonoffcnt==sx_DisplayStatus_t.b_sensebreathduty){
//					LED_send_buf|=LED_SENCE;
//					displayflag = true;
//				}else if(sx_DisplayStatus_t.uc_senseonoffcnt==20){
//					LED_send_buf&=~LED_SENCE;
//					displayflag = true;
//					sx_DisplayStatus_t.uc_senseonoffcnt = 0;
//				}
//			}
//			else{
//				if(sx_DisplayStatus_t.b_sensebreathduty==0)
//					LED_send_buf|=LED_SENCE;
//				else if(sx_DisplayStatus_t.b_sensebreathduty==20)
//					LED_send_buf&=~LED_SENCE;
//				sx_DisplayStatus_t.b_senseonoffflag = 0;
//				displayflag = true;
//			}
		}
		if(sx_DisplayStatus_t.b_Event & LED_EVENT_SENCE)		//感应灯
		{
			sx_DisplayStatus_t.b_Event &= ~LED_EVENT_SENCE;
			switch(sx_DisplayStatus_t.b_sensetype)
			{
				case LED_SENSE_ON:LEDB = 0;break;
				case LED_SENSE_ROTARY:
				case LED_SENSE_TOUCH:
					if(!sx_DisplayStatus_t.b_senseonoffflag)
						LEDB = 0;
					else
						LEDB = 1;
					break;
			}
		//	displayflag = true;
//			SMG_Write_Byte(LED_send_buf);
//			return;
		}
		#endif
//		if(displayflag)
//			SMG_Write_Byte(LED_send_buf);
		if((!vrefunstableflag && !sx_FightLightStatus.b_FightDisplayBusy) || displayflag)
		{SMG_Write_Byte(LED_send_buf);SMG_Write_Byte(LED_send_buf);}
	}
	else{
//		if(sx_DisplayStatus_t.b_Event & LED_EVENT_ERROR){
//			sx_DisplayStatus_t.b_Event &= ~LED_EVENT_ERROR;
//		}
		SMG_Write_Byte(errordisplay_buf);
	}
	
}

void Display_Breath_Cb(void)
{
//	if(sx_DisplayStatus_t.b_sensetype==LED_SENSE_SLEEP){
//		if(sx_DisplayStatus_t.uc_senseonoffcnt<100&&!sx_DisplayStatus_t.b_senseonoffflag){
//			sx_DisplayStatus_t.uc_senseonoffcnt++;
//			sx_DisplayStatus_t.b_senseonoffflag = 1;
//		}
//		if(!sx_DisplayStatus_t.b_senseonoffflag&&sx_DisplayStatus_t.b_sensebreathduty!=0&&sx_DisplayStatus_t.b_sensebreathduty!=100){
//			if(sx_DisplayStatus_t.uc_senseonoffcnt==sx_DisplayStatus_t.b_sensebreathduty){
//				sx_DisplayStatus_t.b_senseonoffflag = 1;
//			}else if(sx_DisplayStatus_t.uc_senseonoffcnt==100){
//				sx_DisplayStatus_t.b_senseonoffflag = 2;
//				sx_DisplayStatus_t.uc_senseonoffcnt = 0;
//			}
//		}
//	}
	if(sx_DisplayStatus_t.b_sensetype==LED_SENSE_SLEEP){
		if(++sx_DisplayStatus_t.uc_senseonoffcnt==sx_DisplayStatus_t.b_sensebreathduty){
			LEDB = 1;
		}else if(sx_DisplayStatus_t.uc_senseonoffcnt==22){
			LEDB = 0;
			sx_DisplayStatus_t.uc_senseonoffcnt = 0;
		}
	}
}

void Display_Cb(void)
{
	//uint16_t sense_fre = 0;
	
	//感应灯
	if((sx_DisplayStatus_t.b_sensetype==LED_SENSE_ROTARY ||  sx_DisplayStatus_t.b_sensetype==LED_SENSE_TOUCH) \
				/*&& b_OnOffFlag_Get() && (!(sx_DisplayStatus_t.b_Event & LED_EVENT_SENCE))*/)
	{
		if(++sx_DisplayStatus_t.uc_senseonoffcnt==sx_DisplayStatus_t.sense_fre){
			sx_DisplayStatus_t.b_senseonoffflag = true;
			set_led_event(LED_EVENT_SENCE);
		}
		if(sx_DisplayStatus_t.uc_senseonoffcnt==sx_DisplayStatus_t.sense_fre*2){
			sx_DisplayStatus_t.uc_senseonoffcnt = 0;
			sx_DisplayStatus_t.b_senseonoffflag = false;
			if(sx_DisplayStatus_t.b_sensetype==LED_SENSE_ROTARY)
				sx_DisplayStatus_t.b_sensetype = LED_SENSE_ON;
			set_led_event(LED_EVENT_SENCE);
		}
	}
	else if(sx_DisplayStatus_t.b_sensetype==LED_SENSE_SLEEP){
//		if(sx_DisplayStatus_t.uc_senseonoffcnt<20)
//			sx_DisplayStatus_t.uc_senseonoffcnt++;
		if(++sx_DisplayStatus_t.b_sensebreathjustcnt%100==0)
		{
			if(!sx_DisplayStatus_t.b_sensebreathjustdir){
				if(++sx_DisplayStatus_t.b_sensebreathduty==21){
					sx_DisplayStatus_t.b_sensebreathjustdir = 1;
					sx_DisplayStatus_t.b_sensebreathjustcnt = 0;
					//LEDB = 0;
				}
				
			}else{
				if(--sx_DisplayStatus_t.b_sensebreathduty==1){
					sx_DisplayStatus_t.b_sensebreathjustdir = 0;
					sx_DisplayStatus_t.b_sensebreathjustcnt = 0;
					//LEDB = 1;
				}
			}
		}
	}
	else{
		sx_DisplayStatus_t.uc_senseonoffcnt = 0;
		sx_DisplayStatus_t.b_senseonoffflag = 0;
	}
}

void Display_init(void)
{
	SMG_Write_Byte(0);
	sx_DisplayStatus_t.b_Event = 0;
	sx_DisplayStatus_t.sense_fre = 10000;
	LED_send_buf = 0x00;
	if(!BurninEntering())
		set_led_event(LED_EVENT_SENCE);
}

void LED_SensetypeFlag_Set(uint8_t value)
{
	sx_DisplayStatus_t.b_sensetype=value;
	sx_DisplayStatus_t.uc_senseonoffcnt = 0;
	sx_DisplayStatus_t.b_senseonoffflag = false;
	if(value==LED_SENSE_ROTARY)
		sx_DisplayStatus_t.sense_fre = 300;
	else if(value==LED_SENSE_TOUCH)
		sx_DisplayStatus_t.sense_fre = 500;
	else if(value==LED_SENSE_SLEEP){
		sx_DisplayStatus_t.b_sensebreathjustcnt = 0;
		sx_DisplayStatus_t.b_sensebreathjustdir = 1;
		sx_DisplayStatus_t.b_sensebreathduty = 21;
	}else
		sx_DisplayStatus_t.sense_fre = 10000;
	set_led_event(LED_EVENT_SENCE);
	//RKSwitchFlag_Set(0);
}


