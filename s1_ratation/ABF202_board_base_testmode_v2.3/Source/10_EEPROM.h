#ifndef __EEPROM_H
#define __EEPROM_H

#include "main.h"

#define EEPROM_STRAT_ADDR               0x800FC00       //Flash×îºó1K
#define EEPROM_WORD_SIZE        128     //?¡ã128¡Á?
#define RGB_FACTOR_DATA_SAVE_ADDESS     0x800F000
#define FLASH_WAITETIME         100

extern uint32_t read_gear_for_eeprom_gear(void);
extern uint32_t read_gear_for_eeprom_gearseal(void);
extern uint32_t read_gear_for_eeprom_mode(void);	
extern uint32_t write_gear_for_eeprom(uint8_t gear1,uint8_t gear2,uint8_t mode);
extern void Flash_Data_Save_RGB_FACTOR(float R_data,float G_data,float B_data,float Lux_data);
extern uint8_t read_rgb_sensor_data_from_eeprom(void);

#endif

