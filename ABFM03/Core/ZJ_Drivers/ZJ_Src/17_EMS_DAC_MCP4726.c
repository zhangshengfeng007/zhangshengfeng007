#include "includes.h"
#include "ems.h"

extern I2C_HandleTypeDef hi2c2;

// MCP4726 不同型号，对应的IIC地址
MCP4726_ADDR_e mcp4726_array[] = {MCP4726_A0_ADDR, MCP4726_A1_ADDR, MCP4726_A2_ADDR, MCP4726_A3_ADDR,
								   MCP4726_A4_ADDR, MCP4726_A5_ADDR, MCP4726_A6_ADDR, MCP4726_A7_ADDR};
/**************************************************************************************
* FunctionName   : DAC_Device_ReadData(uint8_t add)
* Description    : DAC������
* EntryParameter : None 123
* ReturnValue    : None
**************************************************************************************/
uint8_t DAC_Device_ReadData(uint8_t add)
{
	uint8_t Read_Data;

	HAL_I2C_Mem_Read(&hi2c2, EMS_dac_dev.iic_addr + 1, add, 1, &Read_Data, 1, 100);
	return Read_Data;
}
/**************************************************************************************
* FunctionName   : DAC_MCP4726_OUT(uint16_t Voltage)
* Description    : 12bit ----max digital val = 4096
* EntryParameter :  Vref :接VCC的时候，输出放大2倍,
*                         接地或者悬空的时候，输出不放大
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void DAC_MCP4726_OUT(uint16_t Voltage)
{
	uint8_t send_buff[3];

	Voltage = Voltage * 4096 / 3300; //
	send_buff[0] = Voltage >> 8;
	send_buff[1] = Voltage;
	HAL_I2C_Master_Transmit(&hi2c2, EMS_dac_dev.iic_addr, send_buff, 2, 100);
}
/**************************************************************************************
* FunctionName   : DAC_MCP4726_Init(void)
* IIC_ADDR : 7 bit:---1100 A2 A1 A0
*	‘1100000’   MCP47x6A0-E/xx  MCP47x6A0T-E/xx Tape and Reel
*	‘1100001’   MCP47x6A1-E/xx  MCP47x6A1T-E/xx Tape and Reel
*   ‘1100010’   MCP47x6A2-E/xx  MCP47x6A2T-E/xx Tape and Reel
	‘1100011’   MCP47x6A3-E/xx  MCP47x6A3T-E/xx Tape and Reel
    ‘1100100’   MCP47x6A4-E/xx  MCP47x6A4T-E/xx Tape and Reel
	‘1100101’   MCP47x6A5-E/xx  MCP47x6A5T-E/xx Tape and Reel
	‘1100110’   MCP47x6A6-E/xx  MCP47x6A6T-E/xx Tape and Reel
	‘1100111’   MCP47x6A7-E/xx  MCP47x6A7T-E/xx Tape and Reel
**************************************************************************************/
void DAC_MCP4726_Init(void)
{
	uint8_t cmd;

	cmd = 0x80;
	HAL_I2C_Master_Transmit(&hi2c2, EMS_dac_dev.iic_addr, &cmd, 1, 100);
}
/**************************************************************************************
* FunctionName   : DAC_MCP4726_probe(void)
* Description    : DAC��ʼ��
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
HAL_StatusTypeDef DAC_MCP4726_probe(void)
{
	uint8_t cmd;
	uint8_t cur_addr, i;
	HAL_StatusTypeDef ret;

	cmd = 0x80;

	for(i = 0; i < sizeof(mcp4726_array)/sizeof(mcp4726_array[0]); i++)
	{
		cur_addr = mcp4726_array[i];
		ret = HAL_I2C_Master_Transmit(&hi2c2, cur_addr, &cmd, 1, 100);
		if(ret == HAL_OK)
		{
			EMS_dac_dev.iic_addr = cur_addr;
			EMS_dac_dev.dac_probe.p_read = DAC_Device_ReadData;
			EMS_dac_dev.dac_probe.p_write = DAC_MCP4726_OUT;
			EMS_dac_dev.dac_probe.p_init = DAC_MCP4726_Init;
			printf ("\n\r mcp4726 probe succ:0x%0x", cur_addr);
			return HAL_OK;
		}
		else
		{
			printf ("\n\r mcp4726 probe fail cnt:%d", i);
		}
	}
	return HAL_ERROR;
}
