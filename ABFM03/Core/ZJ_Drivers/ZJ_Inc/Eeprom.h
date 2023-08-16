#ifndef _EEPROM_H
#define _EEPROM_H
#include "includes.h"

//123
#define PEERCODE_START_ADDR	    0x0800F000

// #define  Robot_Num_Flash_Add	0x0800F800
#define  EEPROM_STRAT_ADDR	    0x0800F800
#define  EEPROM_WORD_SIZE	    128				//?��128��?

#define  FLASH_REPAIR_LVL_ADDR 	        (EEPROM_STRAT_ADDR)
#define  FLASH_UPKEEP_LVL_ADDR 	        (EEPROM_STRAT_ADDR + 64)

#define  FLASH_BAT_STATE_ADDR 	        (EEPROM_STRAT_ADDR + 128)
#define  FLASH_WORK_STATE_ADDR 	        (EEPROM_STRAT_ADDR + 192)
#define  FLASH_NTC_OFFSET_ADDR  	    (EEPROM_STRAT_ADDR + 256)
#define  FLASH_AGING_FINISH_FLAG_ADDR 	(EEPROM_STRAT_ADDR + 320)


extern void Read_Current_Level_For_Eeprom(void);
extern void Write_Current_Level_To_Eeprom(uint32_t level);
extern void Write_Parameter_To_Eeprom(void);
// void save_msg_to_flash(void);
void Sys_IWDG_STOP_Mode(void);
uint32_t ReadPeerCode(void);
volatile void SavePeerCode(uint64_t PeerCode);
void flashInit(void);
void ReadDevice_ID(void);

#endif

