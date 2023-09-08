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
*  ADDR + CMD +DATA1 +DATA2
* CMD:  RDY(1bit) + POR(1bit) + 0(1bit) + VREF(2BIT) + PD(2BIT) + G(1BIT)
*
* RDY(1bit) : 0 EEMPRO is in programming cycle
*             1 eeprom is not in a programming cycle
*
* PDR   : 	  0  Device is in power off state
*              1  device is power on
*
*  cmd = 0xc0
**************************************************************************************/
uint8_t DAC_Device_ReadData(uint8_t add)
{
	uint8_t Read_Data;
	uint8_t send_buff[3];
	uint8_t recv_buff[5];

	//HAL_I2C_Mem_Read(&hi2c2, EMS_dac_dev.iic_addr + 1, add, 1, &Read_Data, 1, 100);
	//gpio_iic_read_bytes(MCP4726_ADDRESS, add, recv_buff, 1);
	//Read_Data = recv_buff[0];


	//gpio_iic_read_bytes(MCP4726_ADDRESS, );

	return Read_Data;
}
/**************************************************************************************
* FunctionName   : DAC_MCP4726_OUT(uint16_t Voltage)
* Description    : 12bit ----max digital val = 4096
* EntryParameter :  Vref :接VCC的时候，输出放大2倍,
*                         接地或者悬空的时候，输出不放大
* EntryParameter : None
* ReturnValue    : None
* reg寄存器 组成  ：cmd (3bit) + vref(2bit) + pd(2bit) + G(1bit)
*	cmd = 0b   011    --写入eeprom 和  volatile寄存器
*   vref = 0b   00    --硬件电路没有接VREF,所以为0
*   pd   = 0b   00    ---normal 输出
*   G    = 0b   0     --- 放大倍数，0为不放大
*   所以寄存器 为 ：0b  0110 0000
**************************************************************************************/
void DAC_MCP4726_OUT(uint16_t Voltage)
{
	uint8_t send_buff[3];
	uint8_t reg;

	Voltage = Voltage * 4096 / 3300; //
	//send_buff[0] = Voltage >> 8;
	//send_buff[1] = Voltage;
	//HAL_I2C_Master_Transmit(&hi2c2, EMS_dac_dev.iic_addr, send_buff, 2, 100);
	//reg = 0x60;
	reg = Voltage>>8;
	send_buff[0] = Voltage;
	gpio_iic_send_bytes(MCP4726_ADDRESS, reg, send_buff, 1);

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
	uint8_t send_buff[3];
	cmd = 0x80;
	// HAL_I2C_Master_Transmit(&hi2c2, EMS_dac_dev.iic_addr, &cmd, 1, 100);
	gpio_iic_send_bytes(MCP4726_ADDRESS, cmd, send_buff, 0);
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
	uint8_t cur_addr, i, probe_ok_flag = 0;
	HAL_StatusTypeDef ret;

	cmd = 0x80;


 	 taskENTER_CRITICAL();  // 进入临界区， 通讯时码值尽量保持完整

     gpio_iic_start();
	for(i = 0; i < 3; i++)
	{
		only_send_one_byte(MCP4726_ADDRESS);
	       // wait ack************
		I2C_SCL_L();
		sda_cfg_input();   // 等待从机将 sda 拉低
		my_delya_us(5);

		I2C_SCL_H();
		my_delya_us(10);
		if(IS_SDA_L())
		{
			probe_ok_flag = 1;
			break;
		}
	}
   taskEXIT_CRITICAL();		// 退出临界区

	if(probe_ok_flag)
	{
		EMS_dac_dev.iic_addr = MCP4726_ADDRESS;
		EMS_dac_dev.dac_probe.p_read = DAC_Device_ReadData;
		EMS_dac_dev.dac_probe.p_write = DAC_MCP4726_OUT;
		EMS_dac_dev.dac_probe.p_init = DAC_MCP4726_Init;
		printf ("\n\r mcp4726 probe succ:0x%0x", MCP4726_ADDRESS);
		return HAL_OK;
	}

	// for(i = 0; i < sizeof(mcp4726_array)/sizeof(mcp4726_array[0]); i++)
	// {
		// cur_addr = mcp4726_array[i];
		// ret = HAL_I2C_Master_Transmit(&hi2c2, MCP4726_ADDRESS, &cmd, 1, 100);
		// if(ret == HAL_OK)
		// {
		// 	EMS_dac_dev.iic_addr = MCP4726_ADDRESS;
		// 	EMS_dac_dev.dac_probe.p_read = DAC_Device_ReadData;
		// 	EMS_dac_dev.dac_probe.p_write = DAC_MCP4726_OUT;
		// 	EMS_dac_dev.dac_probe.p_init = DAC_MCP4726_Init;
		// 	printf ("\n\r mcp4726 probe succ:0x%0x", cur_addr);
		// 	return HAL_OK;
		// }
		// else
		// {
		// 	printf ("\n\r mcp4726 probe fail cnt:%d", i);
		// }
	// }
	return HAL_ERROR;
}
