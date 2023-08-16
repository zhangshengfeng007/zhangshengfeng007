#ifndef _SGM5355_H
#define _SGM5355_H

#define SGM5355_ADDRESS			0x1A  // 7bit addr = 000 1101 = 0x0D *2 = 0X1A

#define SGM5355_MODE_REG		0x00  // 模式寄存器，默认值为0 ，即 normal 模式
#define SGM5355_OUTPUT_REG		0x01

HAL_StatusTypeDef DAC_SGM5355_Probe(void);

#endif
