#ifndef _MBI5020_H
#define _MBI5020_H
#include "includes.h"

//ע�������ת��
//ͬһ���֣����ֽ��ȷ��ͣ�ͬһ���ֽڸ�λ�ȷ��� 123
#define CLOSE_RF_EMS_OUT_BIT	0x0000
#define RF_CH1_ON_BIT	0x0003
#define RF_CH2_ON_BIT	0x0003 << 2
#define RF_CH3_ON_BIT	0x0003 << 4
#define EMS_CH1_ON_BIT	0x0003 << 8
#define EMS_CH2_ON_BIT	0x0003 << 10
#define EMS_CH3_ON_BIT	0x0003 << 12


#define EMS_CHANNEL_ON()          {HAL_GPIO_WritePin(EMS_CH1_N_PORT, EMS_CH1_N_Pin, GPIO_PIN_SET);HAL_GPIO_WritePin(EMS_CH1_P_PORT, EMS_CH1_P_Pin, GPIO_PIN_SET);}
#define EMS_CHANNEL_OFF()         {HAL_GPIO_WritePin(EMS_CH1_N_PORT, EMS_CH1_N_Pin, GPIO_PIN_RESET);HAL_GPIO_WritePin(EMS_CH1_P_PORT, EMS_CH1_P_Pin, GPIO_PIN_RESET);}

#define RF_CHANNEL_ON()           {HAL_GPIO_WritePin(RF_CH1_N_PORT, RF_CH1_N_Pin, GPIO_PIN_SET);HAL_GPIO_WritePin(RF_CH1_P_PORT, RF_CH1_P_Pin, GPIO_PIN_SET);}
#define RF_CHANNEL_OFF()          {HAL_GPIO_WritePin(RF_CH1_N_PORT, RF_CH1_N_Pin, GPIO_PIN_RESET);HAL_GPIO_WritePin(RF_CH1_P_PORT, RF_CH1_P_Pin, GPIO_PIN_RESET);}
void Mbi5020_SPI_Transmit( uint8_t *pData, uint16_t Size, uint32_t Timeout);
extern void Set_Mbi5020_Out(uint16_t output_on_bit);

#endif


