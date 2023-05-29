
#include "includes.h"

// uint8_t ir_on_flag;
_IRLED_VALUE_TypeDef IRled_Value;

void LedIR_Init(void)
{
//#if ARF001
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LED_IR_ON_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LED_IR_ON_GPIO_Port, &GPIO_InitStruct);

	LED_IR_OFF();
//#else
	//MX_TIM3_Init();
//#endif
}
/**************************************************************************************
 * FunctionName   : IRled_stop
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void IRled_stop(void)
{
	LED_IR_OFF();
}

/**************************************************************************************
 * FunctionName   : IRled_start
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void IRled_start(void)
{
	VBAT_OUT_ON();
	LED_IR_ON();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
