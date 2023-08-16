/* Includes ------------------------------------------------------------------*/
#include "includes.h"

extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;
extern uint32_t UserMemoryMask;
uint8_t tab_1024[1024] ={0};
uint8_t RecvFile_Name[FILE_NAME_LENGTH] = {0};

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);

void System_Flash_Unlock(void)
{
  HAL_FLASH_Unlock();
}

void System_Flash_Lock(void)
{
  HAL_FLASH_Lock();
}
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Download a file via serial port
* @param  None
* @retval None
*/
void SerialDownload(void)
{
  uint8_t Number[10] = {0};
  int32_t Size = 0;

  System_Flash_Unlock();
  SerialPutString("Waiting to be sent ... (press 'a' to abort)\r\n");
  Size = Ymodem_Receive(tab_1024);
  System_Flash_Lock();

  if (Size > 0)
  {
//    LL_GPIO_ResetOutputPin(LV1_LED_OUT_GPIO_Port , LV1_LED_OUT_Pin);
//    LL_GPIO_SetOutputPin  (LV2_LED_OUT_GPIO_Port , LV2_LED_OUT_Pin);
//	  Set_Color_Handle(255,255,255,2);

    SerialPutString("Updata Successfully!\r\nName: ");
    SerialPutString(RecvFile_Name);
    Int2Str(Number, Size);
    SerialPutString("\n\r Size: ");
    SerialPutString(Number);
    SerialPutString(" Bytes\r\n");
  }
  else if (Size == -1)//SIZE_ERR
  {
    SerialPutString("\n\n\rThe image size is higher!\r\n");
  }
  else if (Size == -2)//VER_ERR
  {
    SerialPutString("\n\n\rVerification failed!\r\n");
  }
  else if (Size == -3)//Abort_ERR
  {
    SerialPutString("\r\n\nAborted by user!\r\n");
  }
  else if (Size == -4)//FILE_ERR
  {
    SerialPutString("\r\n\file err!\r\n");
  }
  else               //RECV_ERR
  {
    SerialPutString("\n\rFailed to receive the file!\r\n");
  }
}

/**
* @brief  Display the Main Menu on HyperTerminal
* @param  None
* @retval None
*/
void Main_Menu(void)
{
  while (1)
  {

    SerialDownload();


  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
