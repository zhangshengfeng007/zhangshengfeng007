#include "sleep.h"

#define		SLEEP3MINCHACK		(180000/20)
#define		SLEEP30MINCHACK		(1800000/20)


xdata xSleepStatus_t sx_SleepStatus = {0};


void Sleep_Handler(void)
{
	if(ErrorType_Get()/*&(ERROR_TYPE_VOLTAGE|ERROR_TYPE_SUPPLY)*/)
		return;
	
	if(BurninEntering() || b_BurninFlag_Get())
		return;
	
	if(b_OnOffFlag_Get())
	{
		if(sx_SleepStatus.l_NoninductivitCnt++>SLEEP3MINCHACK)
		{
			sx_SleepStatus.l_NoninductivitCnt = 0;
			sx_SleepStatus.b_SleepMode = SLEEP_MODE_SLEEP;
			PowerOnOff_Set(0);
			FightLightReset();
		}
		if(sx_SleepStatus.l_MaximumWorkCnt++>SLEEP30MINCHACK)
		{
			sx_SleepStatus.l_MaximumWorkCnt = 0;
			sx_SleepStatus.b_SleepMode = SLEEP_MODE_SLEEP;
			PowerOnOff_Set(0);
			FightLightReset();
		}
	}
	else
	{
		sx_SleepStatus.l_NoninductivitCnt=0;
		sx_SleepStatus.l_MaximumWorkCnt=0;
	}
}





