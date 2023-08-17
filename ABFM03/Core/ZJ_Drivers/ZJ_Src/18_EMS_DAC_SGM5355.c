#include "includes.h"

extern I2C_HandleTypeDef hi2c2;

// 向 寄存器写入数据*****************************
//example, writing to point register 0x00 sets the SGM5355-16 to normal mode, we need the following order:
// 1. First byte, 0b0001100 (first 7-bit is I2C address), the 8th bit is read/write bit which is low writing now
// 2. Second byte, 0b00000000 (points to register 0x00)
// 3. Third byte, 0b00000000 (value 0x00 is wrote, which means PD[1:0] = 00)  123
//****************************************************
static HAL_StatusTypeDef DAC_SGM5355_write_reg(uint8_t reg, uint8_t* data, uint8_t len)
{
	HAL_StatusTypeDef ret;
	uint8_t send_data[5] = {0};
	send_data[0] = reg;

	for(int i = 0; i < len; i++)
	{
		send_data[i + 1] = data[i];
	}
	ret = HAL_I2C_Master_Transmit(&hi2c2, SGM5355_ADDRESS, send_data, len + 1, 100);
	return ret;
}


// 向 寄存器写入数据*****************************
// -------reg 0 -----------------------
// To read from the point register 0x00, please refer to the following order:
// 1. First byte, 0b0001100 (first 7-bit is I2C address), the 8th bit is read/write bit which is low writing now
// 2. Second byte, 0b00000000 (points to register 0x00)
// 3. Third byte, 0b0001101 (first 7-bit is I2C address), the 8th bit is read/write bit which is high reading now
// 4. Fourth byte, SGM5355-16 answers with 0b000000XX (data of register 0x00)

//--------reg 1 -------------------------
//To read back the output value 0xABCD from point register 0x01, please refer to the following order:
// 1. First byte, 0b0001100 (first 7-bit is I2C address), the 8th bit is read/write bit which is low writing now
// 2. Second byte, 0b00000001 (points to register 0x01)
// 3. Third byte, 0b0001101 (first 7-bit is I2C address), the 8th bit is read/write bit which is high reading now
// 4. Fourth byte, SGM5355-16 answers with high byte 0xAB (data of register 0x01 high byte)
// 5. Fifth byte, SGM5355-16 answers with low byte 0xCD (data of register 0x01 low byte)
//****************************************************
static uint16_t DAC_SGM5355_read_reg(uint8_t reg)
{
	uint16_t recv_data;
	uint8_t send_buff[5];
	uint8_t recv_buff[5];

	send_buff[0] = reg;

	switch(reg)
	{
		case SGM5355_MODE_REG:
		{
			HAL_I2C_Master_Transmit(&hi2c2, SGM5355_ADDRESS, &reg, 1, 100);
			HAL_I2C_Mem_Read(&hi2c2, SGM5355_ADDRESS, reg, 1, recv_buff, 1, 100);
			recv_data = recv_buff[0];
			break;
		}
		case SGM5355_OUTPUT_REG:
		{
			HAL_I2C_Master_Transmit(&hi2c2, SGM5355_ADDRESS, &reg, 1, 100);
			HAL_I2C_Mem_Read(&hi2c2, SGM5355_ADDRESS, reg, 1, recv_buff, 2, 100);

			 recv_data = recv_buff[0];
			 recv_data <<= 8;
			 recv_data += recv_buff[1];
			break;
		}
	}
	return recv_data;
}
/**************************************************************************************
* FunctionName   : DAC_5355_OUT(uint16_t Voltage)
* Description    :  Vout = D_IN / 65536 *Vref
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
static void DAC_SGM5355_OUT(uint16_t Voltage)
{
	uint16_t tmp;
	uint8_t data[3];

	tmp = Voltage * 65536 / 3300; //
	data[0] = tmp >> 8;
	data[1] = tmp;

	DAC_SGM5355_write_reg(SGM5355_OUTPUT_REG, data, 2);
}
/**************************************************************************************
* FunctionName   : DAC_SGM5355_Init(void)
* Description    : DAC��ʼ��
* EntryParameter : None  // 设置为normal 模式
* ReturnValue    : None
**************************************************************************************/
static void DAC_SGM5355_Init(void)
{
	uint8_t data[3];
	data[0] = 0;
	DAC_SGM5355_write_reg(SGM5355_MODE_REG, data, 1);
}

/**************************************************************************************
* FunctionName   : DAC_SGM5355_Probe(void)
* Description    : DAC��ʼ��
* EntryParameter : None  // 设置为normal 模式
* ReturnValue    : None
**************************************************************************************/
HAL_StatusTypeDef DAC_SGM5355_Probe(void)
{
	uint8_t i;
	uint8_t data[3];
	HAL_StatusTypeDef ret;

	data[0] = 0;
	for(i = 0; i < 3; i++)
	{
		if(HAL_OK == DAC_SGM5355_write_reg(SGM5355_MODE_REG, data, 1))
		{
			EMS_dac_dev.iic_addr = SGM5355_ADDRESS;
			EMS_dac_dev.dac_probe.p_init = DAC_SGM5355_Init;
			EMS_dac_dev.dac_probe.p_read = DAC_SGM5355_read_reg;
			EMS_dac_dev.dac_probe.p_write = DAC_SGM5355_OUT;
			printf ("\n\r sgm5355_probe succ:0x%0x \n\r", EMS_dac_dev.iic_addr);
			return HAL_OK;
		}
		else
		{
			printf ("\n\r sgm5355_probe fail, cnt :%d", i);
		}
	}

	return HAL_ERROR;
}