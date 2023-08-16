#ifndef _MCP4726_H
#define _MCP4726_H
#include "includes.h"

//*********************************************************
// 因为批次不同或型号不同，addr地址不同
// Device                 7bit_addr        8bit_write(0)     8bit_read(1)
// MCP47x6A0              1100000            0xc0               0xc1
// MCP47x6A1              1100001            0xc2               0xc3
// MCP47x6A2              1100010            0xc4               0xc5
// MCP47x6A3              1100011            0xc6               0xc7
// MCP47x6A4              1100100            0xc8               0xc9
// MCP47x6A5              1100101            0xca               0xcb
// MCP47x6A6              1100110            0xcc               0xcd
// MCP47x6A7              1100111            0xce               0xcf //123
//*********************************************************
#define MCP4726_ADDRESS     0xc0

#define MCP4726_READ        0x01
#define MCP4726_DAC         0x40
#define MCP4726_DAC_EEPROM  0x60

typedef enum
{
  MCP4726_A0_ADDR = 0xc0,
  MCP4726_A1_ADDR = 0xc2,
  MCP4726_A2_ADDR = 0xc4,
  MCP4726_A3_ADDR = 0xc6,
  MCP4726_A4_ADDR = 0xc8,
  MCP4726_A5_ADDR = 0xca,
  MCP4726_A6_ADDR = 0xcc,
  MCP4726_A7_ADDR = 0xce,
}MCP4726_ADDR_e;


//*********************************************************
//   cmd_code(3bit / 2bit)        意义
//   C2   C1   C0
//   0    0    X                 Write Volatile DAC Register
//   0    1    0                 Write Volatile Memory
//   0    1    1                 Write All Memory Command
//   1    0    0                 Write Volatile Configuration Bits Command
//*********************************************************


//************************** STATUS BITS OPERATION *****************************************
//   RDY/-BSY       意义 This bit indicates the state of the EEPROM program memory
//   0              EEPROM is not in a programming cycle
//   1              EEPROM is in a programming cycle
//****************************************************************************************

//*********************************************************
//  POR            意义Power-on Reset Status Indicator (flag)
//   0             Device is powered on with VDD > VPOR.
//                 Ensure that VDD is above VDD(MIN) to ensure proper operation
//
//   1             Device is in powered off state. If this value is read, VDD < VDD(MIN) < VPOR
//                 Unreliable device operation should be expected.
//*********************************************************


//**************************** CONFIGURATION BITS  *************************************
//  Vref1：Vref0       意义Resistor Ladder Voltage Reference (VRL) Selection bit
//    0    x        = VDD (Unbuffered)
//    1    0        = Vref pin（Unbuffered）
//    1    1        = Vref pin（Buffered）
//*****************************************************************************************


//*********************************************************
//   PD1 :PD0 (2bit)        意义 输出端Vout，内部接地的电阻
//   0    0                Normal operation
//   0    1                1 kΩ resistor to ground
//   1    0                125 kΩ resistor to ground
//   1    1                640 kΩ resistor to ground
//*********************************************************

//*********************************************************
//   G (1bit)        意义 放大倍数选择
//   0               1x (gain of 1)
//   1               2x (gain of 2). Not applicable when VDD is used as VR
//*********************************************************

//CMD 指令格式：
//*******************************************
//  Write Volatile DAC Register: 向DAC寄存器 写入值
//  Device_addr(8bit) + C2:C1(2bit) + PD1:PD0(2bit) + data(12bit)[D[11]~~~D[0]]
//*******************************************

//*******************************************
//  Write Volatile Memory: 配置DAC寄存器 + config bit位
//  Device_addr(8bit) + C2:C0(3bit) + Vref1：Vref0（2bit） + PD1:PD0(2bit) + G(1bit) + data(16bit)[D[11]~~~D[0](12bit)+xxxx(4bit)]
//                      [0 1 0]
//*******************************************

//*******************************************
//  Write All Memory: 配置 EEPROM中DAC寄存器 + config bit位
//  Device_addr(8bit) + C2:C0(3bit) + Vref1：Vref0（2bit） + PD1:PD0(2bit) + G(1bit) + data(16bit)[D[11]~~~D[0](12bit)+xxxx(4bit)]
//                      [0 1 1]
//*******************************************

//*******************************************
// Write Volatile Configuration Bits:  config bit位
//  Device_addr(8bit) + C2:C0(3bit) + Vref1：Vref0（2bit） + PD1:PD0(2bit) + G(1bit)
//                      [1  0  0]
//*******************************************

//read 指令:
//*******************************************
// READ COMMAND:  This command reads all the device memory
//  Device_addr(8bit) + RDY(1bit) + POR(1bit) + 0(1bit) + Vref1：Vref0(2bit) + PD1:PD0(2bit) + G(1bit) + Vol_data[16bit](D[11]~D[0](12bit)+ 0 0 0 0 (4bit))
//                                                       |                vol Config bits             |
//
//                    + PDY(1bit) + POR(1bit) + 1(1bit) + Vref1: Vref0(2bit) + PD1:PD0(2bit) + G(1bit) + NV_data(16bit)(D[11]~D[0](12bit)+ 0 0 0 0 (4bit))
//                                                       |                NV Config bits              ｜
//
//*******************************************

/**************************************************************************************
* FunctionName   : DAC_Device_ReadData(uint8_t add)
* Description    : DAC������
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
uint8_t DAC_Device_ReadData(uint8_t add);

/**************************************************************************************
* FunctionName   : DAC_MCP4726_OUT(uint16_t Voltage)
* Description    : DAC���������ѹ
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void DAC_MCP4726_OUT(uint16_t Voltage);
void DAC_MCP4726_Init(void);
HAL_StatusTypeDef DAC_MCP4726_probe(void);

#endif

