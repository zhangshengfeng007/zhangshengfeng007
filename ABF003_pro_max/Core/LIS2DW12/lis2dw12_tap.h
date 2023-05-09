#include <string.h>
#include <stdio.h>
#include "lis2dw12_reg.h"


//#include "stm32l0xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "i2c.h"
#include "includes.h"
//void lis2dw12_single_tap(void);
//static void tx_com( uint8_t *tx_buffer, uint16_t len );

void LIS2DH_Data_Deal(void);
void Lis2dh_Init(void);
void Lis2dh_Sleep_on(void);


