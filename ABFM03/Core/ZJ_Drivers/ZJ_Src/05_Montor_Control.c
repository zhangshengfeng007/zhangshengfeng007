#include "includes.h"

/**************************************************************************************
* FunctionName   : Montor_Drive(_Sys_Info_TypeDef* SysInfo ,uint16_t StayTime)
* Description    : ������������
* EntryParameter : None
* ReturnValue    : None 10ms ����һ��
**************************************************************************************/
void Montor_Drive(_Sys_Info_TypeDef* SysInfo )
{
	static uint16_t TimerCnt;
	static uint8_t  Montor_Lock_Flag;

	if(Montor_Lock_Flag)
	{
//		printf ("\n\r TimerCnt=%d\n\r",TimerCnt);
		if(++TimerCnt>(SysInfo->StayTime))
		{
			SysInfo->Montor_Flag = 0;
			Montor_Lock_Flag =0;
//			printf ("\n\r MOTOR_OFF\n\r");
		}
	}
	else
	{
		TimerCnt =0;
	}

	if(SysInfo->Montor_Flag)
	{
		MOTOR_ON();
		// BOOST_5V_ON();
		Montor_Lock_Flag = 1;
//		printf ("\n\r MOTOR_ON\n\r");
	}
	else
	{
//		BOOST_5V_OFF();
		MOTOR_OFF();
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


