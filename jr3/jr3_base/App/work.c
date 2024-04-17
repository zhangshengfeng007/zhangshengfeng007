#include "work.h"

#define		AUTOLIGHTTIME		(30000/10)

#define		IPL_ON()					IPL=1
#define		IPL_OFF()					IPL=0

xdata uint16_t FightLightBuffer[]={	770, 770, 1180, 770, 770, 770,
									1010, 1010, 1330, 1010, 1010, 910,
									1220, 1220, 1600, 1220, 1220, 920,
									1440, 1490, 1680, 1460, 1490, 1100,
									1710, 1750, 1750, 1710, 1740, 1200,
									1880, 1880, 1900, 1880, 1870, 1200};

sx_FightLightStatus_t sx_FightLightStatus={0,0,0,0};

uint16_t ChargeDelayCount = 0;

void Fight_Light_Times(void);
void Trriger_Light(void);

uint8_t IsChargeDelayOK(void)
{
	return (!!ChargeDelayCount);
}

void Work_Handler(void)
{
	if(ErrorType_Get()/*&(ERROR_TYPE_VOLTAGE|ERROR_TYPE_SUPPLY)*/)
		return;
	if(b_SleepMode_Get()==SLEEP_MODE_SLEEP)
		return;
	Fight_Light_Times();
	if(b_OnOffFlag_Get() && sx_FightLightStatus.b_ChargeOkFlag && sx_FightLightStatus.b_FightLightFlag)
	{
		if(b_BurninFlag_Get())		
		{
			if(!b_BurninMode_Get()){		//老化模式
				if(b_BurninStart_Get()){
					if(++sx_BurninStatus_t.ul_BurninTestCnt==400)//40
					{	
						uc_Mode_Set(0);uc_Level_Set(1);uc_LevelSum_Set(0);
					}
					else if(sx_BurninStatus_t.ul_BurninTestCnt==500)//50
					{	
						b_BurninStart_Set(0);
					}
					Trriger_Light();
				}
			}else{		//寿命模式
				if(!b_LifeTestMode_Get() && b_BurninStart_Get())
					Trriger_Light();
			}
		}
		else
		{
			if(b_TouchFlag_Get())		// 非老化必须触摸
			{
				if(b_AutoSwitchFlag_Get()){
					//ul_SleepModeCnt_Set(0);
					//sx_FightLightStatus.sl_AutolightTime=0;		//改到触摸
					Trriger_Light();
				}else{
					if(sx_FightLightStatus.b_KeyPressFlag)		// 按键按下
					{
						Trriger_Light();
						sx_FightLightStatus.b_KeyPressFlag=0;
					}
				}
			}else// if(sx_FightLightStatus.b_KeyPressFlag)
				sx_FightLightStatus.b_KeyPressFlag=0;
		}
		
	}
}

void Trriger_Light(void)
{
	Pwm_Set(CHARGE_FRE,0);
	sx_ChargeStatus.uc_curpwmduty = 0;
	IPL_ON();
	
	Delay_us(3);
	Display_Handler();
	Delay_us(3);
	Display_Handler();
	
	sx_FightLightStatus.b_IPLWorking = true;
	sx_FightLightStatus.b_FightLightTouchFanBusy = true;
	sx_FightLightStatus.b_FightDisplayBusy = true;
	
	//重置打光
	sx_FightLightStatus.b_FightLightFlag=false;
	sx_FightLightStatus.ul_FightLightCnt=0;
	//开启定时器
	TR0 = 1;
	sx_FightLightStatus.sl_AutolightTime=0;
}

void Fight_Light_Times(void)
{
	//充电时间
	sx_FightLightStatus.ul_FightLightCnt++;
	if(sx_FightLightStatus.ul_FightLightCnt>=sx_FightLightStatus.uc_FightLightTime)
		sx_FightLightStatus.ul_FightLightCnt=sx_FightLightStatus.uc_FightLightTime;
	//异常检测初始化时间
	if(sx_FightLightStatus.b_DetInitFlag==DETECTIVE_INIT_ON)
		sx_FightLightStatus.uc_DetInitCnt++;
	else
		sx_FightLightStatus.uc_DetInitCnt = 0;
//	if(sx_FightLightStatus.uc_DetInitCnt>100)
//		sx_FightLightStatus.b_DetInitFlag=DETECTIVE_INIT_SUPPLY;
//	if(sx_FightLightStatus.uc_DetInitCnt>310)
//		sx_FightLightStatus.b_DetInitFlag=DETECTIVE_INIT_DISPLAY;
	if(sx_FightLightStatus.uc_DetInitCnt>500){
		sx_FightLightStatus.uc_DetInitCnt=0;
		sx_FightLightStatus.b_DetInitFlag=DETECTIVE_INIT_FINISH;
	}
	//没有感应退出自动打光
	if(b_AutoSwitchFlag_Get())
		sx_FightLightStatus.sl_AutolightTime++;
	else
		sx_FightLightStatus.sl_AutolightTime=0;
	if(sx_FightLightStatus.sl_AutolightTime>AUTOLIGHTTIME){
		sx_FightLightStatus.sl_AutolightTime = 0;
		b_AutoSwitchFlag_Set(0);
		set_led_event(LED_EVENT_AUTO);
		//sx_FightLightStatus.sl_FightNum = 0;
	}
	//if(ChargeDelayCount < 10)
	ChargeDelayCount = 1;
	
	if(sx_FightLightStatus.b_FightLightFlag==false){
		if(sx_FightLightStatus.ul_FightLightCnt==2){
			sx_FightLightStatus.b_FightLightTouchFanBusy = false;
		}
		if(sx_FightLightStatus.ul_FightLightCnt==5){
			sx_FightLightStatus.b_FightDisplayBusy = false;
		}
		if(sx_FightLightStatus.ul_FightLightCnt>=sx_FightLightStatus.uc_FightLightTime)
		{
			sx_FightLightStatus.b_FightLightFlag=true;
			sx_FightLightStatus.ul_FightLightCnt=0;
		}
	}
}


void Work_Cb(void)
{
	IPL_OFF();
	sx_FightLightStatus.b_IPLWorking = false;
	ChargeDelayCount = 0;

//	sx_DisplayStatus_t.b_ScanCnt = 5;
}

void Work_Init(void)
{
	sx_FightLightStatus.uc_FightLightTime = FightLightBuffer[0]/10;
	ChargeDelayCount = 1;
}

void Fight_Light_Time_Set(uint8_t num)
{
	sx_FightLightStatus.uc_FightLightTime = FightLightBuffer[num]/10;
}



