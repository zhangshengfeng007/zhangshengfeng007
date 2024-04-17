#include "touch_key.h"

xTKStatus_t sx_TKStatus = {0};

void Touch_Key_Handler(void)
{
	uint8_t i=0,j=0xff,k=0;
	uint8_t iic_data;
	
	if(ErrorType_Get()/*&(ERROR_TYPE_VOLTAGE|ERROR_TYPE_SUPPLY)*/)
		return;
	
	if(BurninEntering() || b_BurninFlag_Get())
		return;
	

	if(vrefunstableflag)return;
	
	iic_data = IIC_Test();

	#if 0
	if(b_OnOffFlag_Get()){
		for(i=0x01,j=0;i!=0x40;i<<1,j++)		//模式键
		{
			if(sx_TKStatus.ucTKValue&i){
				uc_Mode_Set(j);
				uc_LevelSum_Set(uc_Mode_Get()+6*(uc_Level_Get()-1));
				set_led_event(LED_EVENT_MODE);
				break;
			}
		}
		if(sx_TKStatus.ucTKValue&0x40){			//自动打光键
			b_AutoSwitchFlag_Set(!b_AutoSwitchFlag_Get());
			set_led_event(LED_EVENT_AUTO);
		}
	}
	#else
	switch(iic_data)
	{
		case 0x04:j=3;break;
		case 0x01:j=5;break;
		case 0x20:j=1;break;
		case 0x10:j=0;break;
		case 0x40:j=2;break;
		case 0x02:		/* 自动打光键 */
		{
			if(!sx_TKStatus.bTKPressFlag)
			{
				if(++sx_TKStatus.bTKAutoScanCnt==5)
				{
					sx_TKStatus.bTKAutoScanCnt = 0;
					sx_TKStatus.bTKPressFlag = true;
					if(b_OnOffFlag_Get()){
						b_AutoSwitchFlag_Set(!b_AutoSwitchFlag_Get());
						set_led_event(LED_EVENT_AUTO);
						ul_SleepModeCnt_Set(0);
					}
		//				else if(b_SleepMode_Get()==SLEEP_MODE_SLEEP){
		//					PowerOnOff_Set(1);
		//					set_led_event(LED_EVENT_MODE);
		//				}
				}
			}
		}
		return;
		case 0x08:j=4;break;
		case 0xff:
		{
			sx_TKStatus.bTKPressFlag = false;
			sx_TKStatus.bTKAutoScanCnt = 0;
		}return;
		default:j=0xff;break;
	}

	if(j<6 && sx_TKStatus.ucTKCode!=iic_data){		//模式键
		if(++sx_TKStatus.bTKModeScanCnt==2){
			sx_TKStatus.bTKModeScanCnt = 0;
			if(b_OnOffFlag_Get()/* || b_SleepMode_Get()==SLEEP_MODE_SLEEP*/){
				uc_Mode_Set(j);
				uc_LevelSum_Set(uc_Mode_Get()+6*(uc_Level_Get()-1));
				set_led_event(LED_EVENT_MODE);
				sx_TKStatus.ucTKCode = iic_data;
				ul_SleepModeCnt_Set(0);
			}
//			if(b_SleepMode_Get()==SLEEP_MODE_SLEEP)
//				PowerOnOff_Set(1);
		}
	}else
		sx_TKStatus.bTKModeScanCnt = 0;
//	if(j==7)
//		sx_TKStatus.bTKPressFlag = false;
	sx_TKStatus.bTKAutoScanCnt = 0;
//	if(j==6)		//自动打光键
//	{
//		if(!sx_TKStatus.bTKPressFlag)
//		{
//			if(++sx_TKStatus.bTKAutoScanCnt==5)
//			{
//				sx_TKStatus.bTKAutoScanCnt = 0;
//				sx_TKStatus.bTKPressFlag = true;
//				if(b_OnOffFlag_Get()){
//					b_AutoSwitchFlag_Set(!b_AutoSwitchFlag_Get());
//					set_led_event(LED_EVENT_AUTO);
//					ul_SleepModeCnt_Set(0);
//				}
//	//				else if(b_SleepMode_Get()==SLEEP_MODE_SLEEP){
//	//					PowerOnOff_Set(1);
//	//					set_led_event(LED_EVENT_MODE);
//	//				}
//			}
//		}
//	}else
//		sx_TKStatus.bTKAutoScanCnt = 0;
	
	
	
	#endif
	
}




void Touch_Key_Init(void)
{
	IIC_Init();
	IIC_Test();
}



