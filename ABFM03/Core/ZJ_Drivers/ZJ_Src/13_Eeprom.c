#include "includes.h"
#include "eeprom.h"
#include "stm32g0xx_ll_utils.h"

uint32_t PageError = 0;                    //����PageError,������ִ�����������ᱻ����Ϊ������FLASH��ַ

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
  if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Data) != HAL_OK)
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
/*static */void FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_EraseInitTypeDef EraseInitStruct = {0};

  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Page        = GetPage(Page_Address);
  EraseInitStruct.NbPages     = 1;
	EraseInitStruct.Banks	  = FLASH_BANK_1;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    Error_Handler();
  }
}


/**************************************************************************************
* FunctionName   : Write_Current_Level_To_Eeprom(uint32_t level)
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Write_Current_Level_To_Eeprom(uint32_t level)
{
	FLASH_EraseInitTypeDef My_Flash;  //���� FLASH_EraseInitTypeDef �ṹ��Ϊ My_Flash

	HAL_FLASH_Unlock();               //����Flash

	My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;  //����Flashִ��ҳ��ֻ����������
	My_Flash.Page = FLASH_PAGE_NB - 1;
	My_Flash.Banks = FLASH_BANK_1;
	My_Flash.NbPages = 1;                        //˵��Ҫ������ҳ�����˲���������Min_Data = 1��Max_Data =(���ҳ��-��ʼҳ��ֵ)֮���ֵ

	HAL_FLASHEx_Erase(&My_Flash, &PageError);  //���ò�����������
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, EEPROM_STRAT_ADDR, level);

	HAL_FLASH_Lock(); //��סFlash
}



/**************************************************************************************
* FunctionName   : Read_Current_Level_For_Eeprom(void)
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Read_Current_Level_For_Eeprom(void)
{
	uint32_t EMS_Level;
	//*(__IO uint16_t *)�Ƕ�ȡ�õ�ַ�Ĳ���ֵ,��ֵΪ16λ����,һ�ζ�ȡ�����ֽڣ�*(__IO uint32_t *)��һ�ζ�4���ֽ�
	SysInfo.repair_level  = *(__IO uint32_t*)( FLASH_REPAIR_LVL_ADDR);
	EMS_Level  = *(__IO uint32_t*)(FLASH_UPKEEP_LVL_ADDR);
	SysInfo.Batt_Value.State  = *(__IO uint32_t*)(FLASH_BAT_STATE_ADDR);
	SysInfo.WorkState = *(__IO uint32_t*)(FLASH_WORK_STATE_ADDR);

	//SysInfo.Test_Mode.Ageing_Mode = *(__IO uint32_t*)( EEPROM_STRAT_ADDR+256 );
    SysInfo.NTC_offset = *(__IO uint32_t*)(FLASH_NTC_OFFSET_ADDR);
	SysInfo.Test_Mode.Ageing_finish_Flag= *(__IO uint32_t*)(FLASH_AGING_FINISH_FLAG_ADDR);

	SysInfo.upkeep_level = (EMS_Level >> 8) & 0xff;
	EMS_Handle.vol_value = (uint16_t)EMS_Level & 0xff;

// ------医疗版最高档位 3档
	if(SysInfo.repair_level > Level_3 || SysInfo.repair_level == Level_None)
	{
		SysInfo.repair_level= Level_1;
	}
	if(SysInfo.upkeep_level > Level_3 || SysInfo.upkeep_level == Level_None)
	{
		SysInfo.upkeep_level= Level_1;
	}

	if(SysInfo.Batt_Value.State > 0x0A)
	{
		SysInfo.Batt_Value.State =0x04 ;
	}
	if(SysInfo.WorkState > upkeep_mode)
	{
		SysInfo.WorkState = repair_mode;
	}

    if(EMS_Handle.vol_value < Level_1 || EMS_Handle.vol_value > Level_30)
	{
		EMS_Handle.vol_value = Level_20;
	}

	SysInfo.Test_Mode.STATUS = 0;
	SysInfo.Test_Mode.Ageing_Flag = 0;

	SysInfo.Test_Mode.Test_Mode_Flag = OFF;
	SysInfo.Test_Mode.Is_Enter_Ageing_Mode = 0;

	upkeep_RF_Level(&EMS_Handle);
	SysInfo.Save_Data.repair_level = SysInfo.repair_level;
	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
	SysInfo.Save_Data.WorkState = SysInfo.WorkState;
	SysInfo.Save_Data.BattState	= SysInfo.Batt_Value.State;

//    SysInfo.EMS_level = SysInfo.upkeep_level ;
//	SysInfo.Save_Data.BattState=BAT_00_00_STATUS;  //����
}



/**************************************************************************************
* FunctionName   : save_msg_to_flash()
* Description    :
* EntryParameter :
* Description    : --
* ReturnValue    :
**************************************************************************************/
static void save_msg_to_flash(void)
{
	//	__set_PRIMASK(1);
	uint64_t EMS_level;

	SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;
	EMS_level = (SysInfo.Save_Data.upkeep_level);
	EMS_level <<= 8;  //              大档位：bit 15 ~ bit 8
	EMS_level += EMS_Handle.vol_value;// 电压值档位：bit7~bit0

	__disable_irq();
	
	__disable_irq();
	__ISB();
	__DSB();
	__DMB();   // 2023 07 28 汇编代码 FAE建议新增，（确保 之前的代码均已执行完毕）
	
	FLASH_Unlock();

	__HAL_FLASH_INSTRUCTION_CACHE_DISABLE(); // 2023 07 28 FAE建议，操作flash之前，先将缓存 和 buffer关闭，
	__HAL_FLASH_PREFETCH_BUFFER_DISABLE();
	
	FLASH_ErasePage(EEPROM_STRAT_ADDR);

	FLASH_ProgramHalfWord(FLASH_REPAIR_LVL_ADDR, (uint64_t)SysInfo.Save_Data.repair_level);
	FLASH_ProgramHalfWord(FLASH_UPKEEP_LVL_ADDR, EMS_level);
	FLASH_ProgramHalfWord(FLASH_BAT_STATE_ADDR, (uint64_t)SysInfo.Save_Data.BattState);
	FLASH_ProgramHalfWord(FLASH_WORK_STATE_ADDR, (uint64_t)SysInfo.Save_Data.WorkState);

	FLASH_ProgramHalfWord(FLASH_NTC_OFFSET_ADDR, (uint64_t)SysInfo.NTC_offset);  // 后面改为，老化结束标记位
	FLASH_ProgramHalfWord(FLASH_AGING_FINISH_FLAG_ADDR, (uint64_t)SysInfo.Test_Mode.Ageing_finish_Flag);
	
	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE(); // 2023 07 28 FAE建议，操作flash之后，还原缓存和 buffer
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	FLASH_Lock();
	__enable_irq();

	HAL_NVIC_SetPriority(TIM14_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);
}


/**************************************************************************************
* FunctionName   : Write_Parameter_To_Eeprom(void)
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Write_Parameter_To_Eeprom(void)
{
	save_msg_to_flash();
	// uint32_t EMS_level;

	// BOOST_5V_ON();
	// SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;

	// EMS_level = (SysInfo.Save_Data.upkeep_level);
	// EMS_level <<= 8;  //              大档位：bit 15 ~ bit 8
	// EMS_level += EMS_Handle.vol_value;// 电压值档位：bit7~bit0

	// save_msg_to_flash(EEPROM_STRAT_ADDR, (uint64_t)SysInfo.Save_Data.repair_level,\
	// 									(uint64_t)EMS_level,\
	// 									(uint64_t)SysInfo.Save_Data.BattState,\
	// 									(uint64_t)SysInfo.Save_Data.WorkState);
//	printf ("\n\r Save BattState=%d\n\r",SysInfo.Save_Data.BattState);//����
//	BOOST_5V_OFF();

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

	flag_IWDG = READ_BIT(FLASH->OPTR, FLASH_OPTR_IWDG_STOP);
	if(flag_IWDG != 0)
	{
		HAL_FLASH_Unlock();		// 解锁flash
		HAL_FLASH_OB_Unlock();  // 解锁 “选项字节”

		CLEAR_BIT(FLASH->OPTR, FLASH_OPTR_IWDG_STOP); // 配置“stop 模式关闭看门狗”
    	/* Set OPTSTRT Bit */
   		 SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT);   // 开始更新 选项字节
    	/* Wait for last operation to be completed */
   		 FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE); // 等待flash 操作结束
   		 /* If the option byte program operation is completed, disable the OPTSTRT Bit */
    	CLEAR_BIT(FLASH->CR, FLASH_CR_OPTSTRT);
		HAL_FLASH_OB_Launch();                 // 加载flash，会导致重启 opt生效

		HAL_FLASH_OB_Lock();  // 选项字节上锁
		HAL_FLASH_Lock();    // flash上锁
	}
	__enable_irq();
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
uint32_t ReadPeerCode(void)
{
	uint32_t temp=*(__IO uint32_t*)(PEERCODE_START_ADDR);
	return 	temp;
}

volatile void SavePeerCode(uint64_t PeerCode)
{
	__disable_irq();

	__ISB();
	__DSB();
	__DMB();   // 2023 07 28 汇编代码 FAE建议新增，（确保 之前的代码均已执行完毕）

	FLASH_Unlock();
	__HAL_FLASH_INSTRUCTION_CACHE_DISABLE(); // 2023 07 28 FAE建议，操作flash之前，先将缓存 和 buffer关闭，
	__HAL_FLASH_PREFETCH_BUFFER_DISABLE();

  FLASH_ErasePage(PEERCODE_START_ADDR);
  FLASH_ProgramHalfWord(PEERCODE_START_ADDR, PeerCode);
	
	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE(); // 2023 07 28 FAE建议，操作flash之后，还原缓存和 buffer
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();

	FLASH_Lock();
	__enable_irq();
}


/************************
 * 读取芯片的 id
 ************************/
void ReadDevice_ID(void)
{
	SysInfo.Device_id = LL_GetUID_Word0();
}

void flashInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	FLASH_Unlock();

	HAL_NVIC_SetPriority(FLASH_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(FLASH_IRQn);

	FLASH_Lock();
}



