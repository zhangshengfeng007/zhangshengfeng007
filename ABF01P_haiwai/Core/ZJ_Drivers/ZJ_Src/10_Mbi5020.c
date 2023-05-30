#include "includes.h"
#include "mbi5020.h"

extern SPI_HandleTypeDef hspi1;
/**************************************************************************************
 * FunctionName   : Set_Mbi5020_Out(uint16_t output_on_bit)
 * Description    : mbi5020д����  16bit
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void Mbi5020_SPI_Transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	HAL_SPI_Transmit(&hspi1, pData, Size, Timeout);
}

/**************************************************************************************
 * FunctionName   : Set_Mbi5020_Out(uint16_t output_on_bit)
 * Description    : mbi5020д����  16bit
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/

void Set_Mbi5020_Out(uint16_t output_on_bit)
{
	//	uint16_t iii = 0,jjj = 0;
	uint8_t TxData[2] = {0, 0};

#if (ARF001 == DEVICE_R1_RPO)																									   // �»Դｵ��
	if ((output_on_bit & RF_CH1_ON_BIT) || (output_on_bit & RF_CH2_ON_BIT) || (output_on_bit & RF_CH3_ON_BIT)) // rf
	{
		EMS_CHANNEL_OFF();
		RF_CHANNEL_ON();
		//		EMS_CHANNEL_ON();
		//		RF_CHANNEL_OFF();
	}
	else if ((output_on_bit & EMS_CH1_ON_BIT) || (output_on_bit & EMS_CH2_ON_BIT) || (output_on_bit & EMS_CH3_ON_BIT)) // ems
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
#elif ((ARF001 == DEVICE_R1_RPO_MAX)||(ARF001 == DEVICE_R1_HAIWAI))
	SPI_VBAT_OUT_OFF();
	TxData[0] = (uint8_t)((output_on_bit >> 8) & 0x00ff);
	TxData[1] = (uint8_t)(output_on_bit & 0x00ff);
	Mbi5020_SPI_Transmit(TxData, 1, 0xffff);

	SPI_VBAT_OUT_ON();
	for (uint16_t iii = 0; iii < 40; iii++)
	{
		for (uint16_t jjj = 0; jjj < 1; jjj++)
			;
	}

	SPI_VBAT_OUT_OFF();
#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
