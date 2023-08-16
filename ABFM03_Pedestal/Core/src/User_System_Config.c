#include "User_System_Config.h"

_SysInfo_TypeDef SysInfo;
uint16_t Sys_Time, LED_Time, Time;
uint8_t Sys_Task = 0;
uint16_t RX_Time = 0;
uint8_t Reset_Cnt = 0;
uint16_t Auto_30S_Lock_Cnt;
uint16_t Time_1S, IDLE_120S_Cnt;
uint8_t Interrupt_Cnt = 0;
uint16_t OP_ADC;
uint16_t Lock_Blink_Time = 0;
uint16_t CHARGE_IO = 0, Charge_Time = 0;


DISP_DATA_t data_disp;
DISP_DATA_t last_data_disp;
