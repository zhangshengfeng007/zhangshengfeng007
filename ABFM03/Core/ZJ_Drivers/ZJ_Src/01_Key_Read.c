#include "includes.h"

 _Sys_Info_TypeDef  SysInfo;

/**************************************************************************************
* FunctionName   : Key_Scan
* Description    :
* EntryParameter : None 123
* ReturnValue    : None
**************************************************************************************/
_Key_Input Key_Scan(void)
{
  _Key_Input  key_set = KEY_None;

  if(IS_POWER_KEY_PRESS())
  {
    key_set = POWER_KEY_Press;
  }
  else if(IS_LEVEL_ADD_KEY_PRESS())
  {
    key_set = LEVEL_ADD_KEY_Press;
  }
  else if(IS_LEVEL_DEC_KEY_PRESS())
  {
    key_set = LEVEL_DEC_KEY_Press;
  }
  else
  {
    key_set = KEY_None;
  }
  return key_set;
}


/**************************************************************************************/
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == POWER_KEY_PIN)
	{
		if(!HAL_GPIO_ReadPin(POWER_KEY_PORT,POWER_KEY_PIN))
		{
			SysInfo.Sleep_Counts =0;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

