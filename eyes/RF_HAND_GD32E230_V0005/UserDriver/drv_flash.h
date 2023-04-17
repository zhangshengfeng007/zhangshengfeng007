#ifndef __DRV_FLASH_H
#define __DRV_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/
#define FMC_PAGE_SIZE ((uint16_t)0x400U)
#define FMC_START_ADDR ((uint32_t)0x08000000U)
#define FMC_END_ADDR ((uint32_t)0x08010000U)

#define LEVEL_ADDR ((uint32_t)0x0800F800U)
#define NTC_OFFSET_ADDR ((uint32_t)0x0800F804U)
#define AGING_FINISH_FLAG_ADDR ((uint32_t)0x0800F808U)
#define UNLOAD_ADC_VAL_ADDR ((uint32_t)0x0800F80CU)
/* Exported functions ------------------------------------------------------- */

void Flash_Erase_Page(uint32_t addr);
void Flash_Write_Word(uint32_t addr, uint32_t *data, u16 Len);

#endif
