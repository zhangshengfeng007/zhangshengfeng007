/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define  USER_FLASH_LAST_PAGE_ADDRESS  0x0800E800 // 4K_DATA_ROM+2K_BOOT_FALG
#define  USER_FLASH_END_ADDRESS        0x08010000//0x0800EFFF // 64 KBytes
//#define  FLASH_PAGE_SIZE              0x800     // 2 Kbytes

/* define the address from where user application will be loaded,
   the application address should be a start sector address */
#define  APPLICATION_ADDRESS     (uint32_t)0x08002000  //APP_START_ADDRESS

/* Get the number of Sector from where the user program will be loaded */
#define  FLASH_PAGE_NUMBER      (uint32_t)((APPLICATION_ADDRESS - 0x08000000) >> 12)

/* Compute the mask to test if the Flash memory, where the user program will be
   loaded, is write protected */
#define  FLASH_PROTECTED_PAGES   ((uint32_t)~((1 << FLASH_PAGE_NUMBER) - 1))

/* define the user application size */
#define  USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define FLASH_WAITETIME 100

#define BOOT_FLAG_ADDRESS  0x0800E800
#define BOOT_FLAG_DATA 	   0x00005aa5

uint8_t FLASH_If_Erase(uint32_t StartSector);
uint8_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data, uint16_t DataLength);

#endif  /* __FLASH_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
