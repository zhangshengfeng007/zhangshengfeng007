#include "includes.h"
#include "mbi5020.h"

/**************************************************************************************
* FunctionName   : Set_Mbi5020_Out(uint16_t output_on_bit)
* Description    : mbi5020д����  16bit
* EntryParameter :
* ReturnValue    : None  123
**************************************************************************************/

void Set_Mbi5020_Out(uint16_t output_on_bit)
{
	if((output_on_bit & RF_CH1_ON_BIT)||(output_on_bit & RF_CH2_ON_BIT)||(output_on_bit & RF_CH3_ON_BIT))  //rf
	{
		EMS_CHANNEL_OFF();
		RF_CHANNEL_ON();
//		EMS_CHANNEL_ON();
//		RF_CHANNEL_OFF();
	}
	else if((output_on_bit & EMS_CH1_ON_BIT)||(output_on_bit & EMS_CH2_ON_BIT)||(output_on_bit & EMS_CH3_ON_BIT)) //ems
	{
		EMS_CHANNEL_ON();
		RF_CHANNEL_OFF();
//		EMS_CHANNEL_OFF();
//		RF_CHANNEL_ON();
	}
	else
	{
		EMS_CHANNEL_OFF();
		RF_CHANNEL_OFF();
	}

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

