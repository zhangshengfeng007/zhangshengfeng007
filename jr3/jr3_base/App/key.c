#include "key.h"

uint8_t gulKeyValue;
xSysStatus_t sx_SysStatus;

void Perpheral_En(void);
void Perpheral_Dis(void);
void OSTickScan(void);
void PowerOnOff_Set(uint8_t value)
{
	//b_FanRotateErrorFlag_Set(0);
	sx_SysStatus.b_OnOffFlag=value;
	b_FightLighTouchFanBusy_Set(0);
	sx_FightLightStatus.b_FightDisplayBusy = true;
	b_KeyPressFlag_Set(0);
	//if(b_SleepMode_Get()!=SLEEP_MODE_SLEEP)
	ul_SleepModeCnt_Set(0);
	if(value){		//开机
		LED_SensetypeFlag_Set(LED_SENSE_ON);
		b_SleepMode_Set(SLEEP_MODE_WAKE);
		b_DetInitFlag_Set(DETECTIVE_INIT_ON);
		//ErrorType_Reset(ERROR_TYPE_ALL);
		Perpheral_En();
		uc_Level_Set(sx_SysStatus.uc_Level);
		uc_LevelSum_Set(uc_Mode_Get()+6*(sx_SysStatus.uc_Level-1));
		//set_led_event(LED_EVENT_SENCE);
	}else{			//关机
		if(b_SleepMode_Get()==SLEEP_MODE_SLEEP)
			LED_SensetypeFlag_Set(LED_SENSE_SLEEP);
		else
			LED_SensetypeFlag_Set(LED_SENSE_ON);
		sx_SysStatus.b_AutoSwitchFlag=0;
		b_DetInitFlag_Set(DETECTIVE_INIT_BOOT);
		Perpheral_Dis();
		if(!b_BurninFlag_Get())set_led_event(LED_EVENT_OFF);
	}
	
}

void Key_Handler(void)
{
	uint8_t i=0;
	
	OSTickScan();
	if(ErrorType_Get()/*&(ERROR_TYPE_VOLTAGE|ERROR_TYPE_SUPPLY)*/){
		gulKeyValue = CKEY_NULL;
		return;
	}
	if(BurninEntering()){
		gulKeyValue = CKEY_NULL;
		return;
	}
	if(b_BurninFlag_Get()){
		if(gulKeyValue==CKEY_RLS_S1&&!b_BurninStart_Get()){
			PowerOnOff_Set(1);
			b_BurninCntStart_Set(1);
			LED_SensetypeFlag_Set(LED_SENSE_TOUCH);
		}
		gulKeyValue = CKEY_NULL;
		return;
	}
	if(b_SleepMode_Get()==SLEEP_MODE_SLEEP){
		if(gulKeyValue==CKEY_RLS_S1){
			PowerOnOff_Set(1);
			set_led_event(LED_EVENT_MODE);
		}
		gulKeyValue = CKEY_NULL;
		return;
	}
	switch(gulKeyValue)
	{
		default:break;
		case CKEY_RLS_S1:{
//			i=!i;
//			if(i) SMG_Write_Byte(0x20);
//			else SMG_Write_Byte(0x00);
			if(sx_SysStatus.b_OnOffFlag)
			{
				if(!sx_SysStatus.b_AutoSwitchFlag)
					b_KeyPressFlag_Set(1);
			}
			ul_SleepModeCnt_Set(0);
		}break;
		case CKEY_HLD_S1:{
			if(sx_SysStatus.b_OnOffFlag)
			{
				if(!sx_SysStatus.b_AutoSwitchFlag){
					sx_SysStatus.b_AutoSwitchFlag = 1;
					set_led_event(LED_EVENT_AUTO);
				}
			}
			ul_SleepModeCnt_Set(0);
		}break;
	}
	gulKeyValue = CKEY_NULL;
}




void Perpheral_En(void)
{
	uc_FanKeepFlagSet(FAN_ON);
	//FAN_FULLSPEED();
	FAN_ON();
	Pwm_Set(CHARGE_FRE,0);
	_nop_();
	Pwm_Open_Charge();
	RELEASE_400V_DIS();
	sx_ChargeStatus.uc_curpwmduty = 0;
}

void Perpheral_Dis(void)
{
	Pwm_Close_Charge();
	RELEASE_400V_EN();
	if((ErrorType_Get()&~(ERROR_TYPE_FAN|ERROR_TYPE_NTC)) || b_BurninFlag_Get()){
		FAN_OFF();
		uc_FanKeepFlagSet(FAN_BOOT);
	}else if(!ErrorType_Get()){
		uc_FanKeepFlagSet(FAN_OFF);
		FAN_HALFSPEED();
	}
}

type_sKSCtrl strKSCtrl;

uint32_t Key_Scan()
{
	uint8_t ulKIO,ulTemp;
	uint8_t Key_Data;
	uint8_t ul;
	uint8_t bFlag,bRetFlag;

	bRetFlag = false;	//假定无有效键输出

	//scan board and get physical key code
    bFlag = false;
	while(1)
	{
		Key_Data = 0xff;
		if(!KEY_LIGHT)
			Key_Data &= ~(0x01<<0);		//打光键
			
		Key_Data = ~Key_Data;
			
		ulKIO = Key_Data&IO_KEY;

		for(ul=0; ul<20; ul++);
		ulTemp = Key_Data&IO_KEY;
		if(ulTemp != ulKIO)
			break;
		
		//连续数次后认为一次稳定物理输入
		bFlag = true;
		break;
	}
	//得到原始物理键值
	if(!bFlag)
		return CKEY_NULL;	//键盘没有一个稳定状态，忽虑

	
	//得到一次稳定状态，开始逻辑判断
	//strKSCtrl.ulKIOState = ulKIO;
	

	strKSCtrl.ulKeyCur = ulKIO;

	switch(strKSCtrl.ucScanStep)
	{
	case CKSTEP_NULL:
		if(strKSCtrl.ulKeyCur == 0x00)
			break;	//没有任何键压下
		
		strKSCtrl.ulCodeTemp = strKSCtrl.ulKeyCur;
		strKSCtrl.ucScanCnt = 1;
		strKSCtrl.ucScanStep = CKSTEP_PRESS;
		
		break;

	case CKSTEP_PRESS:
		if(strKSCtrl.ulKeyCur != strKSCtrl.ulCodeTemp)
		{
				strKSCtrl.ulCodeTemp = CKEY_NULL;
				strKSCtrl.ucScanCnt = 0;
				strKSCtrl.ucScanStep = CKSTEP_NULL;

		}
		else
		{
			if(CKCONFIRM_TIMES == strKSCtrl.ucScanCnt ++)
			{
				strKSCtrl.ulKeyCode = strKSCtrl.ulCodeTemp;
				bRetFlag = true;
				strKSCtrl.ucScanStep = CKSTEP_HOLD;
			}
		}

		break;

	case CKSTEP_HOLD:
		if(strKSCtrl.ulKeyCur != strKSCtrl.ulCodeTemp)
		{	
			strKSCtrl.ucScanStep = CKSTEP_RLS;
			//strKSCtrl.ucScanCnt = CKCONFIRM_TIMES;
			
		}
		else
		{
			strKSCtrl.ucScanCnt++;
			if(CHOLDKEY_TIME == strKSCtrl.ucScanCnt - CKCONFIRM_TIMES)
			{
				strKSCtrl.ulKeyCode = strKSCtrl.ulCodeTemp<<1;
				bRetFlag = true;
				strKSCtrl.ucScanStep = CKSTEP_HOLDRPT;
				//strKSCtrl.ucScanCnt = CKCONFIRM_TIMES;
			}
		}

		break;

	case CKSTEP_HOLDRPT:
		if(strKSCtrl.ulKeyCur != strKSCtrl.ulCodeTemp)
			strKSCtrl.ucScanStep = CKSTEP_RLS;
			//strKSCtrl.ucScanCnt = CKCONFIRM_TIMES;
		else
			strKSCtrl.ucScanStep = CKSTEP_RLS;
		break;

	case CKSTEP_RLS:
		if(strKSCtrl.ulKeyCur == 0x00)
		{	//无任何键按下
			
			if(strKSCtrl.ucScanCnt++ >= CKCONFIRM_TIMES+CKCONFIRM_TIMES)
			{
//				test=!test;
//			if(test) SMG_Write_Byte(0x08);
//			else SMG_Write_Byte(0x00);
					if(strKSCtrl.ucScanCnt<CPRESSKEY_TIME)
						//strKSCtrl.ulKeyCode = strKSCtrl.ulCodeTemp + (strKSCtrl.ulCodeTemp<<3);
						strKSCtrl.ulKeyCode = strKSCtrl.ulCodeTemp<<2;
					else if(strKSCtrl.ucScanCnt>CHOLDKEY_TIME)
						//strKSCtrl.ulKeyCode = strKSCtrl.ulCodeTemp + (strKSCtrl.ulCodeTemp<<1) + (strKSCtrl.ulCodeTemp<<3);
						strKSCtrl.ulKeyCode = strKSCtrl.ulCodeTemp<<3;
					if(BurninEntering())
						set_led_event(LED_EVENT_SENCE);
					b_BurninReadyFlag_Set(0);
					bRetFlag = true;
					strKSCtrl.ulCodeTemp = CKEY_NULL;
					strKSCtrl.ucScanCnt = 0;
					strKSCtrl.ucScanStep = CKSTEP_NULL;
			}
		}
		else
		{	//仍有键被压下
//			if(strKSCtrl.ulKeyCur != strKSCtrl.ulCodeTemp)
//			{	//未放手又有新的键被压下
//				strKSCtrl.ulCodeTemp = CKEY_NULL;
//				strKSCtrl.ucScanCnt = 0;
//				strKSCtrl.ucScanStep = CKSTEP_NULL;
//			}
			//else 仍然是上次按键未放开，无动作；直到放开或新的按下
		}

		break;
	}
	
	if(bRetFlag == true)
		return strKSCtrl.ulKeyCode;
	else
		return CKEY_NULL;
}

void OSTickScan(void)
{
	uint8_t ulT;
	

	ulT = Key_Scan();
	if(CKEY_NULL != ulT)
	{
		strKSCtrl.ulKeyCode = CKEY_NULL;
		gulKeyValue = ulT;
	}
	
}

void Key_init(void)
{
	strKSCtrl.ulKeyCode = CKEY_NULL;
	strKSCtrl.ulCodeTemp = CKEY_NULL;
	strKSCtrl.ulKeyCur = CKEY_NULL;
	strKSCtrl.ucScanCnt = 0;
	strKSCtrl.ucScanStep = CKSTEP_NULL;
	//strKSCtrl.usHoldTime= CHOLDKEY_TIME/CSYSTICK_BASE;
	//strKSCtrl.usHoldRPTTime = CHOLDRPTKEY_TIME/CSYSTICK_BASE;
	gulKeyValue = CKEY_NULL;
	
	sx_SysStatus.uc_Mode = 0;
	sx_SysStatus.uc_Level = 0;
	sx_SysStatus.uc_LevelSum = 0;
}





