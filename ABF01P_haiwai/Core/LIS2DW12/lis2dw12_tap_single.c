/*
 ******************************************************************************
 * @file    single_tap.c
 * @author  Sensors Software Solution Team
 * @brief   This file show the simplest way to detect single tap from sensor.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/*
 * This example was developed using the following STMicroelectronics
 * evaluation boards:
 *
 * - STEVAL_MKI109V3 + STEVAL-MKI179V1
 * - NUCLEO_F411RE + X_NUCLEO_IKS01A3
 * - DISCOVERY_SPC584B + X_NUCLEO_IKS01A3
 *
 * and STM32CubeMX tool with STM32CubeF4 MCU Package
 *
 * Used interfaces:
 *
 * STEVAL_MKI109V3    - Host side:   USB (Virtual COM)
 *                    - Sensor side: SPI(Default) / I2C(supported)
 *
 * NUCLEO_STM32F411RE - Host side: UART(COM) to USB bridge
 *                    - I2C(Default) / SPI(supported)
 *
 * DISCOVERY_SPC584B  - Host side: UART(COM) to USB bridge
 *                    - Sensor side: I2C(Default) / SPI(supported)
 *
 * If you need to run this example on a different hardware platform a
 * modification of the functions: `platform_write`, `platform_read`,
 * `tx_com` and 'platform_init' is required.
 *
 */

/* STMicroelectronics evaluation boards definition
 *
 * Please uncomment ONLY the evaluation boards in use.
 * If a different hardware is used please comment all
 * following target board and redefine yours.
 */

// #define STEVAL_MKI109V3  /* little endian */
// #define NUCLEO_F411RE    /* little endian */
// #define SPC584B_DIS      /* big endian */

/* ATTENTION: By default the driver is little endian. If you need switch
 *            to big endian please see "Endianness definitions" in the
 *            header file of the driver (_reg.h).
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "lis2dw12_reg.h"
#include "i2c.h"
#include "lis2dw12_tap.h"
#include "includes.h"

#if (G_SENSOR_SELECT == USE_G_SENSOR)
#define SENSOR_BUS hi2c2
/* Private macro -------------------------------------------------------------*/
#define BOOT_TIME 20 // ms

/* Private variables ---------------------------------------------------------*/
static uint8_t whoamI, rst;
// static uint8_t tx_buffer[1000];

/* Extern variables ----------------------------------------------------------*/
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
							  uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
							 uint16_t len);
static void platform_delay(uint32_t ms);
/* Private functions ---------------------------------------------------------*/
/*
 *   WARNING:
 *   Functions declare in this section are defined at the end of this file
 *   and are strictly related to the hardware platform used.
 *
 */

void Lis2dh_Init(void)
{
	/* Initialize mems driver interface. */
	stmdev_ctx_t dev_ctx;
//	lis2dw12_reg_t int_route;
	dev_ctx.write_reg = platform_write;
	dev_ctx.read_reg = platform_read;
	dev_ctx.handle = &SENSOR_BUS;
//	uint8_t counts;
	/* Initialize platform specific hardware */

	/* Wait sensor boot time */
	platform_delay(BOOT_TIME);
	/* Check device ID */
	lis2dw12_device_id_get(&dev_ctx, &whoamI);
	if (whoamI != LIS2DW12_ID)
	{
		lis2dw12_device_id_get(&dev_ctx, &whoamI);
	}

	/* Restore default configuration */
	lis2dw12_reset_set(&dev_ctx, PROPERTY_ENABLE);

	//  do {
	lis2dw12_reset_get(&dev_ctx, &rst);
	//  } while (rst);
	/* Enable Block Data Update */
	lis2dw12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
	/* Set full scale */
	lis2dw12_full_scale_set(&dev_ctx, LIS2DW12_2g);
	/* Configure filtering chain
	 * Accelerometer - filter path / bandwidth
	 */
	lis2dw12_filter_path_set(&dev_ctx, LIS2DW12_LPF_ON_OUT);
	lis2dw12_filter_bandwidth_set(&dev_ctx, LIS2DW12_ODR_DIV_4);
	/* Configure power mode */
	lis2dw12_power_mode_set(&dev_ctx, LIS2DW12_HIGH_PERFORMANCE);
	// lis2dw12_power_mode_set(&dev_ctx, LIS2DW12_CONT_LOW_PWR_LOW_NOISE_12bit);
	/* Set Output Data Rate */
	lis2dw12_data_rate_set(&dev_ctx, LIS2DW12_XL_ODR_25Hz);
}

void Lis2dh_Sleep_on(void)
{
//	stmdev_ctx_t dev_ctx;
	uint8_t Lis2d_Sleep = 0x00;
	//  lis2dw12_data_rate_set(&dev_ctx, LIS2DW12_XL_ODR_OFF);
	platform_write(&SENSOR_BUS, LIS2DW12_CTRL1, &Lis2d_Sleep, 1);
//	platform_write(&SENSOR_BUS, LIS2DW12_WAKE_UP_THS, &Lis2d_Sleep, 1);

}
static int16_t data_raw_acceleration[3];
void LIS2DH_Data_Deal(void)
{
	static uint8_t Lis2d_Counts, ERROR_ID_CNT;
	static uint32_t Lis2d_Result;
	static uint8_t OUT_X_L, OUT_X_H, OUT_Y_L, OUT_Y_H, OUT_Z_L, OUT_Z_H;

//	stmdev_ctx_t dev_ctx;
//  uint8_t reg;

//  lis2dw12_flag_data_ready_get(&dev_ctx, &reg);
//  lis2dw12_acceleration_raw_get(&dev_ctx, data_raw_acceleration);

	platform_read(&SENSOR_BUS, LIS2DW12_OUT_X_L, &OUT_X_L, 1);
	platform_read(&SENSOR_BUS, LIS2DW12_OUT_X_H, &OUT_X_H, 1);
	platform_read(&SENSOR_BUS, LIS2DW12_OUT_Y_L, &OUT_Y_L, 1);
	platform_read(&SENSOR_BUS, LIS2DW12_OUT_Y_H, &OUT_Y_H, 1);
	platform_read(&SENSOR_BUS, LIS2DW12_OUT_Z_L, &OUT_Z_L, 1);
	platform_read(&SENSOR_BUS, LIS2DW12_OUT_Z_H, &OUT_Z_H, 1);

	gdata.AXIS_X = (int16_t)OUT_X_L + (int16_t)(OUT_X_H * 256);
	gdata.AXIS_Y = (int16_t)OUT_Y_L + (int16_t)(OUT_Y_H * 256);
	gdata.AXIS_Z = (int16_t)OUT_Z_L + (int16_t)(OUT_Z_H * 256);

	gdata.AXIS_X = lis2dw12_from_fs2_to_mg(gdata.AXIS_X);
	gdata.AXIS_Y = lis2dw12_from_fs2_to_mg(gdata.AXIS_Y);
	gdata.AXIS_Z = lis2dw12_from_fs2_to_mg(gdata.AXIS_Z);

	gdata.AXIS_D = sqrt(gdata.AXIS_X * gdata.AXIS_X + gdata.AXIS_Y * gdata.AXIS_Y + gdata.AXIS_Z * gdata.AXIS_Z);
	Lis2d_Result += abs(gdata.AXIS_D - gdata.LAST_AXIS_D);
	gdata.LAST_AXIS_D = gdata.AXIS_D;

	if (++Lis2d_Counts >= 10)
	{
		//		printf ("\n\r%d\n\r",Lis2d_Result);	 //����
		if (Lis2d_Result > 40) //
		{
			SysInfo.MotionStateFlage = MEMS_SUCCESS;
		}
		else
		{
			SysInfo.MotionStateFlage = MEMS_ERROR;
		}
		Lis2d_Counts = 0;
		Lis2d_Result = 0;

		//		HAL_I2C_Mem_Read(&SENSOR_BUS, LIS2DW12_I2C_ADD_L,LIS2DW12_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &whoamI,1,100);
		platform_read(&SENSOR_BUS, LIS2DW12_WHO_AM_I, &whoamI, 1);
//		gdata.AXIS_ID = whoamI;
		if (whoamI != LIS2DW12_ID)
		{
			if (++ERROR_ID_CNT > 20)
			{
        if(ERROR_ID_CNT < 25) //�رմ�������Դ��ʱ50ms
				{
					VCC3V3_OUT_OFF();
				}
				else if(ERROR_ID_CNT < 35) //������������Դ��ʱ100ms�󣬵ȵ�Դ�ȶ����ٳ�ʼ��������
				{
					VCC3V3_OUT_ON();
				}
				else
				{
          ERROR_ID_CNT = 0;
					Lis2dh_Init();
				}
			}
		}
		else
		{
			ERROR_ID_CNT = 0;
		}
	}
}
/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
							  uint16_t len)
{

	HAL_I2C_Mem_Write(handle, LIS2DW12_I2C_ADD_L, reg,
					  I2C_MEMADD_SIZE_8BIT, (uint8_t *)bufp, len, 100);

	return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
							 uint16_t len)
{

	HAL_I2C_Mem_Read(handle, LIS2DW12_I2C_ADD_L, reg,
					 I2C_MEMADD_SIZE_8BIT, bufp, len, 100);

	return 0;
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  tx_buffer     buffer to transmit
 * @param  len           number of byte to send
 *
 */
// static void tx_com(uint8_t *tx_buffer, uint16_t len)
//{

//  HAL_UART_Transmit(&huart2, tx_buffer, len, 1000);

//}

/*
 * @brief  platform specific delay (platform dependent)
 *
 * @param  ms        delay in ms
 *
 */
static void platform_delay(uint32_t ms)
{

	HAL_Delay(ms);
}


#endif
/*
 * @brief  platform specific initialization (platform dependent)
 */
