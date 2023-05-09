#ifndef _EEPROM_H
#define _EEPROM_H
#include "includes.h"

#define  Robot_Num_Flash_Add	0x0800F800	 
#define  EEPROM_STRAT_ADDR	  0x0800F800		
#define  EEPROM_WORD_SIZE	128				//?бу128б┴?
#define  Batt_STRAT_ADDR 			0x0800F800+128
extern void Read_Current_Level_For_Eeprom(void);
extern void Write_Current_Level_To_Eeprom(uint32_t level);
extern void Write_Parameter_To_Eeprom(void);
void save_flash_flag(uint32_t address, uint64_t data1,uint64_t data2,uint64_t data3,uint64_t data4);
void Sys_IWDG_STOP_Mode(void);
#endif

