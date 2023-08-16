#include "includes.h"

///ʱ�Ӽ���  ÿ1msִ��һ�Σ�����������ʱ��  123
extern 	uint32_t hw2000_log_time;
// void Base_Time_Const(void)
// {
// 	static uint8_t Cnt_5ms;
// 	static uint8_t Cnt_10ms;
// 	static uint8_t Cnt_100ms;

// 	Cnt_5ms ++;
// 	Cnt_10ms ++;
// 	Cnt_100ms++;
// 	SysInfo.Timer_Value.Get_1ms_Flag = 1;

// 	if(Cnt_5ms == 5)
// 	{
// 		Cnt_5ms = 0;
// 		SysInfo.Timer_Value.Get_5ms_Flag = 1;
// 	}
// 	if(Cnt_10ms == 10)
// 	{
// 		Cnt_10ms = 0;
// 		SysInfo.Timer_Value.Get_10ms_Flag = 1;
// 	}
// 	if(Cnt_100ms == 100)
// 	{
// 		Cnt_100ms = 0;
// 		SysInfo.Timer_Value.Get_100ms_Flag = 1;
// 	}
// }

void HAL_IncTick(void)
{
  uwTick += (uint32_t)uwTickFreq;

//   Base_Time_Const();
	hw2000_log_time++;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



