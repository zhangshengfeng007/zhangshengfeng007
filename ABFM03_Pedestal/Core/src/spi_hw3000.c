/******************************************************************************
 * @file     spi0.c
 * @brief    hr8p296 spi function
 *
 *
 * @version  1.0
 * @date     2018-12-06
 *
 * @author   fudl
 *
 * @note
 * @Copyright (C) 2018 Shanghai Eastsoft Microelectronics C0., Ltd.
 ******************************************************************************/
#include "spi_hw3000.h"
#include "Delay.h"




/******************************************************************************
 * @brief    hr8p296_spi0_init
 *
 * @note	 spi0 init function
 *
 * @param	 None
 * @retval	 None
 *
 * @version  1.0
 * @date
 * @author
 ******************************************************************************/

void spi_init(void)
{
    CSN_1;
    PDN_0;
    Delay_MS(20);
    PDN_1;
}

/******************************************************************************
 * @brief    hw3000_write_reg
 *
 * @note	 write register function
 *
 * @param	 addr reg address
			 data
 * @retval	 None
 *
 * @version  1.0
 * @date
 * @author
 ******************************************************************************/
void hw3000_write_reg(uint8_t addr, uint16_t data)
{
    uint8_t i;
    addr |= 0x80;

    CSN_0;

    for(i = 0; i < 8; i++)
    {
        if(addr & 0x80)
        {
            MOSI_1;
        }
        else
        {
            MOSI_0;
        }

        SCK_1;
        addr <<= 1;
        SCK_0;
    }

    for(i = 0; i < 16; i++)
    {
        if(data & 0x8000)
        {
            MOSI_1;
        }
        else
        {
            MOSI_0;
        }

        SCK_1;
        data <<= 1;
        SCK_0;
    }

    CSN_1;
    MOSI_0;

}

/******************************************************************************
 * @brief    hw3000_read_reg
 *
 * @note	 read register function
 *
 * @param	 addr reg address
 * @retval	 data
 *
 * @version  1.0
 * @date
 * @author
 ******************************************************************************/

uint16_t hw3000_read_reg(uint8_t addr)
{
    uint8_t i;
    uint16_t value;
    CSN_0;

    for(i = 0; i < 8; i++)
    {
        if(addr & 0x80)
        {
            MOSI_1;
        }
        else
        {
            MOSI_0;
        }

        SCK_1;
        addr <<= 1;
        SCK_0;
    }

    for(i = 0; i < 16; i++)
    {
        SCK_1;
        value <<= 1;
        SCK_0;

        if(MISO)
        {
            value |= 0x0001;
        }
    }

    CSN_1;
    MOSI_0;
    return value;
}

void hw3000_write_fifo(uint8_t addr, uint8_t *data, uint16_t length)
{
    uint8_t i, j;
    uint8_t value;
    addr |= 0x80;
    CSN_0;

    for(i = 0; i < 8; i++)
    {
        if(addr & 0x80)
        {
            MOSI_1;
        }
        else
        {
            MOSI_0;
        }

        SCK_1;
        addr <<= 1;
        SCK_0;
    }

    for(j = 0; j < length; j++)
    {
        value = data[j];

        for(i = 0; i < 8; i++)
        {
            if(value & 0x80)
            {
                MOSI_1;
            }
            else
            {
                MOSI_0;
            }

            SCK_1;
            value <<= 1;
            SCK_0;
        }
    }

    CSN_1;
    MOSI_0;
}

/******************************************************************************
 * @brief    hw3000_read_fifo
 *
 * @note	 read fifo function
 *
 * @param	 addr reg address
 *		     data
 *           length
 * @retval	 None
 *
 * @version  1.0
 * @date
 * @author
 ******************************************************************************/

void hw3000_read_fifo(uint8_t addr, uint8_t *data, uint16_t length)
{
    uint8_t i, j;
    uint8_t value;
    CSN_0;

    for(i = 0; i < 8; i++)
    {
        if(addr & 0x80)
        {
            MOSI_1;
        }
        else
        {
            MOSI_0;
        }

        SCK_1;
        addr <<= 1;
        SCK_0;
    }

    for(j = 0; j < length; j++)
    {
        for(i = 0; i < 8; i++)
        {
            SCK_1;
            value <<= 1;
            SCK_0;

            if(MISO)
            {
                value |= 0x0001;
            }
        }

        data[j] = value;
    }

    CSN_1;
    MOSI_0;
}

