/******************************************************************************
 * @file     spi.h
 * @brief    ES32F0654 spi function
 *           
 *
 * @version  1.0
 * @date     2021-5-27
 *
 * @author   JACK
 *
 * @note
 * @Copyright (C) 2021 Shanghai Eastsoft Microelectronics C0., Ltd.
 ******************************************************************************/
#ifndef __SPI_HW3000_H
#define __SPI_HW3000_H

#include <stdint.h>
#include "User_System_Config.h"


void spi_init(void);

void hw3000_write_reg(uint8_t addr, uint16_t data);

uint16_t hw3000_read_reg(uint8_t addr);

void hw3000_write_fifo(uint8_t addr, uint8_t *data, uint16_t length);

void hw3000_read_fifo(uint8_t addr, uint8_t *data, uint16_t length);


#endif
