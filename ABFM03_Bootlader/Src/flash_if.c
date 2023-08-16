/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/**
* @brief  This function does an erase of all user flash area
* @param  StartSector: start of user flash area
* @retval 0: user flash area successfully erased
*         1: error occurred
*/
uint8_t FLASH_If_Erase(uint32_t StartSector)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t PageError = 0;
  
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;                                    //页擦除
  EraseInitStruct.Banks = FLASH_BANK_1;
  EraseInitStruct.Page   = StartSector/FLASH_PAGE_SIZE;                                 //声明要擦除的地址
  EraseInitStruct.NbPages = (USER_FLASH_END_ADDRESS-StartSector)/FLASH_PAGE_SIZE;       //说明要擦除的页数，
  
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    return (1);
  }
  FLASH_WaitForLastOperation(FLASH_WAITETIME);
  return (0);
}


uint8_t FLASH_Write_Data(uint32_t Address, uint64_t Data)
{
  if (FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE) == HAL_OK)
  {
    SET_BIT(FLASH->CR, FLASH_CR_PG);
    /* Program first word */
    *(uint32_t *)Address = (uint32_t)Data;
    __ISB();
    /* Program second word */
    *(uint32_t *)(Address + 4U) = (uint32_t)(Data >> 32U);
    /* Wait for last operation to be completed */
    if(FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE) != HAL_OK)
    {
      return 1;
    }
    /* If the program operation is completed, disable the PG or FSTPG Bit */
    CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
  }
  return 0;
}
/**
* @brief  This function writes a data buffer in flash (data are 32-bit aligned).
* @note   After writing data buffer, the flash content is checked.
* @param  FlashAddress: start address for writing data buffer
* @param  Data: pointer on data buffer
* @param  DataLength: length of data buffer (unit is 32-bit word)   
* @retval 0: Data successfully written to Flash memory
*         1: Error occurred while writing data in Flash memory
*         2: Written Data in flash memory is different from expected one
*/
//#pragma optimize=none
uint8_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint16_t DataLength)
{
  //HAL_StatusTypeDef status;
  uint32_t i = 0;
  
  for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-8)); )
  {
    //if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, *FlashAddress, *(uint64_t *)(*Data+i)) != HAL_OK)
    if(FLASH_Write_Data(*FlashAddress, *(uint64_t *)(*Data+i)) != 0)
    {
      return (1);
    }
    else
    {
      if (*(uint64_t*)*FlashAddress != *(uint64_t *)(*Data+i))   //逐字校验，校验错误返回
      {
        return(2);
      }
      *FlashAddress += 8;
      i+= 8;
    }
  }
  return (0);
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
