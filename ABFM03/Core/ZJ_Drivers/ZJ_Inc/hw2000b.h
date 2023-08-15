/******************************************************************************
 * @file     hw2000b.h
 * @brief    hw2000b function
 *
 *
 * @version  1.2
 * @date     2017-03-21
 *
 * @author   fudl
 *
 * @note
 * @Copyright (C) 2017 Shanghai Eastsoft Microelectronics C0., Ltd.
 ******************************************************************************/
#ifndef __HW2000B_H
#define __HW2000B_H
#include "includes.h"

#define HW_CSN_HIGH() HAL_GPIO_WritePin(CSN_2_4G_PORT, CSN_2_4G_Pin, GPIO_PIN_SET)
#define HW_CSN_LOW()  HAL_GPIO_WritePin(CSN_2_4G_PORT, CSN_2_4G_Pin, GPIO_PIN_RESET)

#define HW_CE_HIGH()  HAL_GPIO_WritePin(CE_2_4G_PORT, CE_2_4G_Pin, GPIO_PIN_SET)
#define HW_CE_LOW()   HAL_GPIO_WritePin(CE_2_4G_PORT, CE_2_4G_Pin, GPIO_PIN_RESET)

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

//=====reg======
#define  PKTCTRL_REG    0x20        // 数据包配置寄存器
#define  TRCTL_REG      0x21        // 发送接收使能寄存器
#define  CHANNR_REG     0x22
 // Channel 设置寄存器 默认值：0x1830
 //bit[13:9] 参考时钟的频率（MHz）
            //12M 晶振： 01100
            //16M 晶振： 10000
            //20M 晶振： 10100
// bit[6:0] RF channel number
    //The on-air frequency:
         //F = RF_FREQ_BASE + PLL_CH_NO

                                    //
#define  MISC0_REG      0x23        // 配置寄存器 0
#define  FOCCFG_REG     0x24        // 频率偏差补偿寄存器
#define  FREQBASE_REG   0x25        // 起始频点设置寄存器
//bit[11:0] 起始频点设置(MHz)
//
//

#define  DS_PE_REG      0x26        // 上拉和大电流驱动使能控制寄存器
#define  THRES_REG      0x28        // 阈值设置寄存器
#define  MISC1_REG      0x29        // 配置寄存器 1
#define  MISC2_REG      0x2A        // 配置寄存器 2

#define  PKG_RSSI_REG   0x2B        // 接收数据包信号能量指示寄存器
#define  CDTH_REG       0x2C        // CD 阈值设置寄存器
#define  RSSI_REG       0x2D        // 接收信号能量指示寄存器
#define  STATUS0_REG    0x2E        // 状态寄存器 0

#define  STATUS1_REG    0x30        // 状态寄存器 1
#define  FIX_LEN_EN_REG 0x31        // 定长收发控制寄存器
#define  FIFO0DATA_REG  0x32        // FIFO0 访问寄存器
#define  FIFO1DATA_REG  0x33        // FIFO1 访问寄存器
#define  ACKFIFO0_DATA_REG   0x34   // ACKFIFO0 访问寄存器

#define  ACKFIFO1_DATA_REG   0x35   // ACKFIFO1 访问寄存器
#define  FIFO0CTRL_REG       0x36   // FIFO0 配置寄存器
#define  FIFO1CTRL_REG       0x37   // FIFO1 配置寄存器
#define  ACKFIFO0_CTRL_REG   0x38   // ACKFIFO0 配置寄存器
#define  ACKFIFO1_CTRL_REG   0x39   // ACKFIFO1 配置寄存器

#define  FIFOSTATUS_REG      0x3A   // FIFO 状态寄存器
#define  CLEAR_REG           0x3B   // FIFO 指针寄存器
#define  PIPECTRL_REG        0x3C   // PIPE 设置寄存器
#define  INT_REG             0x3D   // 中断寄存器
#define  P0ADDR0_REG         0x40   // PIPE0 地址低 16bits

#define  P0ADDR1_REG         0x41   // PIPE0 地址中间 16bits
#define  P0ADDR2_REG         0x42   // PIPE0 地址高 16bits
#define  P1ADDR0_REG         0x43   // PIPE1 地址低 16bits
#define  P1ADDR1_REG         0x44   // PIPE1 地址中间 16bits
#define  P1ADDR2_REG         0x45   // PIPE1 地址高 16bits

#define  P2ADDR_REG          0x46   // PIPE2 地址低 8bits
#define  P3ADDR_REG          0x47   // PIPE3 地址低 8bits

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
    uint32_t tmp_device_id;
    HW_2_4G_mode_e mode;
    /* data */
}HW_2_4G_t;


extern HW_2_4G_t hw_2_4G_data;
void hw2000b_port_init(void);

void hw2000b_init_250k(void);

void set_osc_16M(void);

void set_power_8(void);

void set_power_5(void);

void set_power_2(void);

void set_power_0(void);

void set_power_negative_5(void);

void set_power_negative_10(void);

void set_power_negative_15(void);

void set_power_negative_20(void);

void set_power_negative_25(void);

void set_frequence_2403(void);

void set_frequence_2450(void);

void set_frequence_2483(void);

void hw2000b_fec23_on(void);

void hw2000b_no_fec23(void);

void hw2000b_scramble_on(void);

void hw2000b_scramble_off(void);

void hw2000b_power_down(void);

void hw2000b_sleep(void);

void hw2000b_idle(void);

void hw2000b_power_test(void);

void hw2000b_tx_data(uint8_t *Buf, uint16_t Size);

void hw2000b_rx_data(uint8_t *Buf);

void HW_Process(void);
// 自动配对时的频率
void set_auto_math_freq_2470M(void);
void set_normal_freq(void);

#endif

