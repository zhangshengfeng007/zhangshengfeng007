
#include "includes.h"

uint8_t ir_on_flag;
_IRLED_VALUE_TypeDef IRled_Value;

void LedIR_Init(void)
{
#if ARF001
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LED_IR_ON_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_IR_ON_GPIO_Port, &GPIO_InitStruct);

	LED_IR_OFF();
#else
	MX_TIM3_Init();
#endif
}
/**************************************************************************************
 * FunctionName   : IRled_stop
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void IRled_stop(void)
{
	ir_on_flag = 0;
#if ARF001
	LED_IR_OFF();
#else
	LED_IR_OFF();
	IR_LED_RED_SET(0);
	IR_LED_RED_STOP();
#endif

	//	IR_LED_RED_SET(0);
	//	IR_LED_RED_STOP();
	//	VBAT_OUT_OFF();
	//	IR_LED_RED_SET(0);
	//	VBAT_OUT_SET(0);
	//	HAL_TIMEx_PWMN_Stop(&IR_LED_TIM,IR_LED_RED);
}

/**************************************************************************************
 * FunctionName   : IRled_start
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void IRled_start(void)
{
	ir_on_flag = 1;

#if ARF001
	LED_IR_ON();
	VBAT_OUT_ON();
#else
	VBAT_OUT_ON();
	IR_LED_RED_START();
	IR_LED_RED_SET(90);
	//		LED_IR_ON();
	//		VBAT_OUT_ON() ;
#endif
	//	IR_LED_RED_START();
	//	IR_LED_RED_SET(100);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
