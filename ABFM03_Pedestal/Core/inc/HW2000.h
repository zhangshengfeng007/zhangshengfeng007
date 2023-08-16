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
#ifndef __HW2000_H
#define __HW2000_H

#include <stdint.h>
#include "User_System_Config.h"

/*
	HW2000B.RX_Buffer[1]//起始字节
	HW2000B.RX_Buffer[2]//长度字节
	HW2000B.RX_Buffer[3]//地址1
	HW2000B.RX_Buffer[4]//地址2
	HW2000B.RX_Buffer[5]//地址3
	HW2000B.RX_Buffer[6]//地址4
	HW2000B.RX_Buffer[7]//挡位字节
	HW2000B.RX_Buffer[8]//错误状态&解锁状态 高四位存储错误状态 低四位存储解锁状态
	HW2000B.RX_Buffer[9]//结束字节
	HW2000B.RX_Buffer[10]//和校验
*/
#define HW_BUFFER_LEN  10
#define INDEX_START		0
#define INDEX_LEN		1
#define INDEX_DEVICE	2
#define INDEX_ID1		3
#define INDEX_ID2		4
#define INDEX_ID3		5
#define INDEX_LEVEL		6
#define INDEX_LOCK		7
#define INDEX_END		8
#define INDEX_AND		9


#define MAX_INVESER_CNT		5

typedef enum
{
    NORMAL_MODE,
    AUTO_MATCH_MODE,
}HW_2_4G_mode_e;

typedef struct
{
	uint8_t send_id_flag;
	uint8_t recv_new_id_cnt;
	uint8_t save_new_id_flag;
    HW_2_4G_mode_e mode;
	uint32_t tmp_device_id;
    /* data */
}HW_2_4G_t;


extern HW_2_4G_t hw_2_4G_data;



void HW_Spi_Init(void);
void HW_RF_RX(uint8_t *RX_Buf,uint8_t Length);
void HW2000_Init_250k(void);
void HW_RF_TX(uint8_t *TX_Buf, uint16_t Length);
void HW_TX_Data_Process(void);
void HW_RX_Data_Process(void);
void HW_RF_RX_EN(void);

void set_normal_freq(void);
void set_auto_math_freq_2470M(void);

#endif
