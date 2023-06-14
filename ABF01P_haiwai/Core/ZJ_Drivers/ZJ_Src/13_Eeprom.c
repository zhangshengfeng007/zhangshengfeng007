#include "includes.h"
#include "eeprom.h"

uint32_t PageError = 0; // ����PageError,������ִ�����������ᱻ����Ϊ������FLASH��ַ

/**************************************************************************************
 * FunctionName   : GetPage()
 * Description    :
 * EntryParameter :
 * Description    : --
 * ReturnValue    :
 **************************************************************************************/
static uint32_t GetPage(uint32_t Addr)
{
	return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
	;
}

/**************************************************************************************
 * FunctionName   : FLASH_Unlock()
 * Description    :
 * EntryParameter :
 * Description    : --
 * ReturnValue    :
 **************************************************************************************/
static void FLASH_Unlock(void)
{
	HAL_FLASH_Unlock();
}

/**************************************************************************************
 * FunctionName   : FLASH_Lock()
 * Description    :
 * EntryParameter :
 * Description    : --
 * ReturnValue    :
 **************************************************************************************/
static void FLASH_Lock(void)
{
	HAL_FLASH_Lock();
}

/**************************************************************************************
 * FunctionName   : FLASH_ProgramHalfWord()
 * Description    :
 * EntryParameter :
 * Description    : --
 * ReturnValue    :
 **************************************************************************************/
static void FLASH_ProgramHalfWord(uint32_t Address, uint64_t Data)
{
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Data) != HAL_OK)
	{
		Error_Handler();
	}
}

/**************************************************************************************
 * FunctionName   : FLASH_ErasePage()
 * Description    :
 * EntryParameter :
 * Description    : --
 * ReturnValue    :
 **************************************************************************************/
static void FLASH_ErasePage(uint32_t Page_Address)
{
	FLASH_EraseInitTypeDef EraseInitStruct = {0};

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page = GetPage(Page_Address);
	EraseInitStruct.NbPages = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		Error_Handler();
	}
}

/**************************************************************************************
 * FunctionName   : save_flash_flag()
 * Description    :
 * EntryParameter :
 * Description    : --
 * ReturnValue    :
 **************************************************************************************/
void save_flash_flag(uint32_t address, uint64_t data1, uint64_t data2, uint64_t data3, uint64_t data4)
{
	//	__set_PRIMASK(1);
	__disable_irq();
	FLASH_Unlock();
	FLASH_ErasePage(address);
	FLASH_ProgramHalfWord(address, data1);
	FLASH_ProgramHalfWord(address + 64, data2);
	FLASH_ProgramHalfWord(address + 128, data3);
	FLASH_ProgramHalfWord(address + 192, data4);
	FLASH_ProgramHalfWord(address + 256, SysInfo.Test_Mode.Ageing_Mode);
	FLASH_Lock();
	__enable_irq();

	HAL_NVIC_SetPriority(TIM14_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);
}
/**************************************************************************************
 * FunctionName   : Write_Current_Level_To_Eeprom(uint32_t level)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void Write_Current_Level_To_Eeprom(uint32_t level)
{
	FLASH_EraseInitTypeDef My_Flash; // ���� FLASH_EraseInitTypeDef �ṹ��Ϊ My_Flash

	HAL_FLASH_Unlock(); // ����Flash

	My_Flash.TypeErase = FLASH_TYPEERASE_PAGES; // ����Flashִ��ҳ��ֻ����������
	My_Flash.Page = FLASH_PAGE_NB - 1;
	My_Flash.Banks = FLASH_BANK_1;
	My_Flash.NbPages = 1; // ˵��Ҫ������ҳ�����˲���������Min_Data = 1��Max_Data =(���ҳ��-��ʼҳ��ֵ)֮���ֵ

	HAL_FLASHEx_Erase(&My_Flash, &PageError); // ���ò�����������
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, EEPROM_STRAT_ADDR, level);

	HAL_FLASH_Lock(); // ��סFlash
}

/**************************************************************************************
 * FunctionName   : Read_Current_Level_For_Eeprom(void)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void Read_Current_Level_For_Eeprom(void)
{
	//*(__IO uint16_t *)�Ƕ�ȡ�õ�ַ�Ĳ���ֵ,��ֵΪ16λ����,һ�ζ�ȡ�����ֽڣ�*(__IO uint32_t *)��һ�ζ�4���ֽ�
	SysInfo.repair_level = *(__IO uint32_t *)(EEPROM_STRAT_ADDR);
	SysInfo.upkeep_level = *(__IO uint32_t *)(EEPROM_STRAT_ADDR + 64);
	SysInfo.Batt_Value.State = *(__IO uint32_t *)(EEPROM_STRAT_ADDR + 128);
	SysInfo.WorkState = *(__IO uint32_t *)(EEPROM_STRAT_ADDR + 192);
	SysInfo.Test_Mode.Ageing_Mode = *(__IO uint32_t *)(EEPROM_STRAT_ADDR + 256);

	if (SysInfo.repair_level > 0x05 || SysInfo.repair_level == 0)
	{
		SysInfo.repair_level = 0x01;
	}
	if (SysInfo.upkeep_level > 0x05 || SysInfo.upkeep_level == 0)
	{
		SysInfo.upkeep_level = 0x01;
	}
	if (SysInfo.Batt_Value.State > 0x06)
	{
		SysInfo.Batt_Value.State = 0x03;
	}
	if (SysInfo.WorkState > 0x01)
	{
		SysInfo.WorkState = repair_mode;
	}

	if (SysInfo.Test_Mode.Ageing_Mode == 0x01 || SysInfo.Test_Mode.Ageing_Mode == 0x02)
	{
		SysInfo.Test_Mode.Ageing_Mode = 1;
		SysInfo.Test_Mode.Test_Mode = 0x0a;
		SysInfo.Test_Mode.Ageing_Flag = 1;
		SysInfo.Test_Mode.Test_Mode_Flag = ON;
		SysInfo.Sleep_Flag = 0;
		SysInfo.Test_Mode.Lock_Flag = 0x01;
	}
	else
	{
		SysInfo.Test_Mode.Ageing_Mode = 0;
		SysInfo.Test_Mode.Test_Mode = 0;
		SysInfo.Test_Mode.Ageing_Flag = 0;
		SysInfo.Test_Mode.Lock_Flag = 0x00;
		SysInfo.Test_Mode.Test_Mode_Flag = OFF;
	}

	SysInfo.Save_Data.repair_level = SysInfo.repair_level;
	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
	SysInfo.Save_Data.WorkState = SysInfo.WorkState;
	SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;
	//    SysInfo.EMS_level = SysInfo.upkeep_level ;
	//	SysInfo.Save_Data.BattState=BAT_00_00_STATUS;  //????
}

/**************************************************************************************
 * FunctionName   : Write_Parameter_To_Eeprom(void)
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void Write_Parameter_To_Eeprom(void)
{
	BOOST_5V_ON();
	SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;
	save_flash_flag(EEPROM_STRAT_ADDR, (uint64_t)SysInfo.Save_Data.repair_level,
					(uint64_t)SysInfo.Save_Data.upkeep_level,
					(uint64_t)SysInfo.Save_Data.BattState,
					(uint64_t)SysInfo.Save_Data.WorkState);
	// BOOST_5V_OFF();
}
/**************************************************************************************
 * FunctionName   : HAL_GPIO_EXTI_Falling_Callback()
 * Description    :
 * EntryParameter :
 * Description    : --
 * ReturnValue    :
 **************************************************************************************/
void Sys_IWDG_STOP_Mode(void)
{
	uint8_t flag_IWDG;

	__disable_irq();
	flag_IWDG = READ_BIT(FLASH->OPTR, FLASH_OPTR_IWDG_STOP) != 0; // ������꣬���ⷴ������flash��������
	if (flag_IWDG != 0)
	{
		HAL_FLASH_Unlock();
		HAL_FLASH_OB_Unlock(); // ����FLASH

		CLEAR_BIT(FLASH->OPTR, FLASH_OPTR_IWDG_STOP); // �޸Ŀ��Ź�����Ϊ������ʱ���Ź�ֹͣ����
		/* Set OPTSTRT Bit */
		SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT); // ����3�䲻��ȱ�٣������޷��޸�FLASH->OPTR
		/* Wait for last operation to be completed */
		FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
		/* If the option byte program operation is completed, disable the OPTSTRT Bit */
		CLEAR_BIT(FLASH->CR, FLASH_CR_OPTSTRT);
		HAL_FLASH_OB_Launch(); // ����flash���ᵼ������
	}
	HAL_FLASH_OB_Lock();
	HAL_FLASH_Lock(); // �޸����ر�flash���Ѿ�������ִ��һ�����ⲻ��
	__enable_irq();
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
