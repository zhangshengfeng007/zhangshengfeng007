#include "includes.h"

//uint32_t ram_buf[EEPROM_WORD_SIZE];
uint32_t PageError = 0;                        //设置PageError,如果出现错误这个变量会被设置为出错的FLASH地址
void Flash_Data_Save_RGB_FACTOR(float R_data,float G_data,float B_data,float Lux_data);
uint8_t read_rgb_sensor_data_from_eeprom(void);
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
uint32_t write_gear_for_eeprom(uint8_t gear1,uint8_t gear2,uint8_t mode)
{
  FLASH_EraseInitTypeDef My_Flash;             //声明 FLASH_EraseInitTypeDef 结构体为 My_Flash
  uint32_t save_data = 0;
  
  save_data = (gear1 << 16) |(gear2 << 8) | mode;
  
  HAL_FLASH_Unlock();                          //解锁Flash
  My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;  //标明Flash执行页面只做擦除操作
  My_Flash.Page = FLASH_PAGE_NB - 1;
  My_Flash.Banks = FLASH_BANK_1;
  My_Flash.NbPages = 1;                        //说明要擦除的页数，此参数必须是Min_Data = 1和Max_Data =(最大页数-初始页的值)之间的值
  
  HAL_FLASHEx_Erase(&My_Flash, &PageError);    //调用擦除函数擦除
  FLASH_WaitForLastOperation(FLASH_WAITETIME);
  
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, EEPROM_STRAT_ADDR, save_data);
  FLASH_WaitForLastOperation(FLASH_WAITETIME);
  
  HAL_FLASH_Lock(); //锁住Flash
  
  return 0;
}
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
uint32_t  ID_Num;
uint32_t read_gear_for_eeprom_gearseal(void)
{
  uint32_t Robot_Num_Flash_Add = 0x800FC00; 
  
  //*(__IO uint16_t *)是读取该地址的参数值,其值为16位数据,一次读取两个字节，*(__IO uint32_t *)就一次读4个字节
  ID_Num = *(__IO uint32_t*)( Robot_Num_Flash_Add );//*(__IO uint16_t*)( Robot_Num_Flash_Add ); 
  //	dfdfdfd = *(__IO uint16_t*)( Robot_Num_Flash_Add + 8); 
  //Seal_gear = (unsigned char)(ID_Num>>16);
  
  ID_Num = (ID_Num >> 16)&0x00ff;//(ID_Num >> 8)&0x00ff;

  return ID_Num;
}

uint32_t read_gear_for_eeprom_gear(void)
{
  uint32_t Robot_Num_Flash_Add = 0x800FC00; 
  
  //*(__IO uint16_t *)是读取该地址的参数值,其值为16位数据,一次读取两个字节，*(__IO uint32_t *)就一次读4个字节
  ID_Num = *(__IO uint16_t*)( Robot_Num_Flash_Add );//*(__IO uint16_t*)( Robot_Num_Flash_Add ); 
  //	dfdfdfd = *(__IO uint16_t*)( Robot_Num_Flash_Add + 8); 
  //Seal_gear = (unsigned char)(ID_Num>>16);
  
  ID_Num = (ID_Num >> 8)&0x00ff;//(ID_Num >> 8)&0x00ff;
  
  return ID_Num;
}

/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
uint32_t read_gear_for_eeprom_mode(void)
{
  uint32_t Robot_Num_Flash_Add = 0x800FC00; 
  
  //*(__IO uint16_t *)是读取该地址的参数值,其值为16位数据,一次读取两个字节，*(__IO uint32_t *)就一次读4个字节
  ID_Num = *(__IO uint16_t*)( Robot_Num_Flash_Add ); 
  //	dfdfdfd = *(__IO uint16_t*)( Robot_Num_Flash_Add + 8); 
  ID_Num = ID_Num&0x00ff;
  
  return ID_Num;
}
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
void Flash_Data_Save_RGB_FACTOR(float R_data,float G_data,float B_data,float Lux_data)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t PageError = 0;
  uint32_t temp;
  
  HAL_FLASH_Unlock();
  
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks  = FLASH_BANK_1;
  EraseInitStruct.Page   = FLASH_PAGE_NB - 2;
  EraseInitStruct.NbPages = 1;
  
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    Error_Handler();
  }
  FLASH_WaitForLastOperation(FLASH_WAITETIME);
  
  temp = *(uint32_t *)&G_data;
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, RGB_FACTOR_DATA_SAVE_ADDESS, temp);
  FLASH_WaitForLastOperation(FLASH_WAITETIME);
  
  temp = *(uint32_t *)&R_data;
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, RGB_FACTOR_DATA_SAVE_ADDESS+8, temp);
  FLASH_WaitForLastOperation(FLASH_WAITETIME);
  
  temp = *(uint32_t *)&B_data;
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, RGB_FACTOR_DATA_SAVE_ADDESS+16, temp);
  FLASH_WaitForLastOperation(FLASH_WAITETIME);
  
  temp = *(uint32_t *)&Lux_data;
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, RGB_FACTOR_DATA_SAVE_ADDESS+24, temp);
  FLASH_WaitForLastOperation(FLASH_WAITETIME);
  
  HAL_FLASH_Lock();
}
/**************************************************************************************
* FunctionName   :
* Description    : 
* EntryParameter : 
* Description    : --
* ReturnValue    :
**************************************************************************************/
uint8_t read_rgb_sensor_data_from_eeprom(void)
{
  if((*(float *)RGB_FACTOR_DATA_SAVE_ADDESS) == 1)
  {
    White_Balance.Calibration_R   = *(float *)(RGB_FACTOR_DATA_SAVE_ADDESS+8);
    White_Balance.Calibration_G   = *(float *)(RGB_FACTOR_DATA_SAVE_ADDESS);
    White_Balance.Calibration_B   = *(float *)(RGB_FACTOR_DATA_SAVE_ADDESS+16);
    White_Balance.Calibration_Lux = *(float *)(RGB_FACTOR_DATA_SAVE_ADDESS+24);
  }
  else
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

