#include "burnin.h"


#define		BURNINLIFEWORKTIME		(300000/20)
#define		BURNINLIFESTAYTIME		(120000/20)



xBurninStatus_t sx_BurninStatus_t = {0};



void BurninInit_Cb(void)
{
	//上电按键检测
	if(sx_BurninStatus_t.OnTestCnt<60)
		sx_BurninStatus_t.OnTestCnt++;
}

void Burnin_init(void)
{
	uint8_t ONpressTimes = 0, ONunpressTimes = 0;
	sx_BurninStatus_t.OnTestCnt = 0;
	while(sx_BurninStatus_t.OnTestCnt<60)
	{
		if(!KEY_LIGHT)
			ONpressTimes++;
		else
			ONunpressTimes++;
	}
	if(ONpressTimes>ONunpressTimes)
		sx_BurninStatus_t.b_BurninReadyFlag = true;
	sx_BurninStatus_t.OnTestCnt = 0;
}

void Burnin_Handler(void)
{
	uint8_t iic_data = 0, iic_data1 = 0, ul;
	
	#if 1
	if(!sx_BurninStatus_t.b_BurninReadyFlag && ErrorType_Get()/*&(ERROR_TYPE_VOLTAGE|ERROR_TYPE_SUPPLY)*/)
		return;
	
	if(!sx_BurninStatus_t.b_BurninReadyFlag && !sx_BurninStatus_t.b_BurninFlag)
		return;
	
	/* 老化模式进入检测 */
	if(sx_BurninStatus_t.b_BurninReadyFlag && !sx_BurninStatus_t.b_BurninFlag){
		iic_data = IIC_Test();
		for(ul=0; ul<20; ul++);
		iic_data1 = IIC_Test();
		if(iic_data==iic_data1 && iic_data==0x02)
			sx_BurninStatus_t.OnTouchTimes++;
		else if(sx_BurninStatus_t.OnTouchTimes)
			sx_BurninStatus_t.OnTouchTimes--;
		
		if(sx_BurninStatus_t.OnTouchTimes==80){
			sx_BurninStatus_t.b_BurninReadyFlag = false;
			sx_BurninStatus_t.b_BurninFlag = true;
			//LED_SensetypeFlag_Set(LED_SENSE_TOUCH);
			//PowerOnOff_Set(1);
			uc_Mode_Set(0);
			uc_Level_Set(6);
			uc_LevelSum_Set(30);
			set_led_event(LED_EVENT_BURNIN);
		}
	}
	
	/* 寿命计数 */
	if(sx_BurninStatus_t.b_BurninMode && sx_BurninStatus_t.b_BurninStart)
		sx_BurninStatus_t.uc_LifeTesttimeCnt++;
	else
		sx_BurninStatus_t.uc_LifeTesttimeCnt = 0;
	if(!sx_BurninStatus_t.b_LifeTestMode){		//打光时间
		if(sx_BurninStatus_t.uc_LifeTesttimeCnt > BURNINLIFEWORKTIME){
			sx_BurninStatus_t.uc_LifeTesttimeCnt = 0;
			sx_BurninStatus_t.b_LifeTestMode = 1;
		}
	}else{		//关闭时间
		if(sx_BurninStatus_t.uc_LifeTesttimeCnt > BURNINLIFESTAYTIME){
			sx_BurninStatus_t.uc_LifeTesttimeCnt = 0;
			sx_BurninStatus_t.b_LifeTestMode = 0;
		}
	}
	
	/* 启动计时 */
	if(sx_BurninStatus_t.b_BurninCntStart)
		sx_BurninStatus_t.BurninCntStartCnt++;
	else return;
	if(sx_BurninStatus_t.BurninCntStartCnt==400){	/* (8000/20) */
		sx_BurninStatus_t.b_BurninStart = true;
		sx_BurninStatus_t.b_BurninCntStart = false;
		sx_BurninStatus_t.BurninCntStartCnt = 0;
		//LED_SensetypeFlag_Set(LED_SENSE_TOUCH);
		//老化
		sx_BurninStatus_t.ul_BurninTestCnt = 0;
		//寿命
		sx_BurninStatus_t.uc_LifeTesttimeCnt = 0;
		sx_BurninStatus_t.b_LifeTestMode = 0;
		
		uc_Mode_Set(0);uc_Level_Set(6);uc_LevelSum_Set(30);
	}
	
	/* 老化模式 */
	if(sx_BurninStatus_t.b_BurninFlag){
		if(!sx_BurninStatus_t.b_BurninStart){
			if(!sx_BurninStatus_t.b_BurninMode){
				sx_BurninStatus_t.ul_BurninTestCnt = 0;
			}else{
				sx_BurninStatus_t.uc_LifeTesttimeCnt = 0;
			}
			uc_Mode_Set(0);uc_Level_Set(6);uc_LevelSum_Set(30);
		}
	}
	#endif
}






