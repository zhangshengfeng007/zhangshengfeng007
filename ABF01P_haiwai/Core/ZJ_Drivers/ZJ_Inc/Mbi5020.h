#ifndef _MBI5020_H
#define _MBI5020_H
#include "includes.h"

//ע�������ת��
//ͬһ���֣����ֽ��ȷ��ͣ�ͬһ���ֽڸ�λ�ȷ���
#define RF_CH1_ON_BIT	0x0003
#define RF_CH2_ON_BIT	0x0003 << 2
#define RF_CH3_ON_BIT	0x0003 << 4
#define EMS_CH1_ON_BIT	0x0003 << 8
#define EMS_CH2_ON_BIT	0x0003 << 10
#define EMS_CH3_ON_BIT	0x0003 << 12


//#define EMS_CH1_ON_BIT	  0x0003 << 0
//#define EMS_CH2_ON_BIT	  0x0003 << 2
//#define EMS_CH3_ON_BIT	  0x0003 << 4
//#define RF_CH1_ON_BIT	  0x0003 << 8
//#define RF_CH2_ON_BIT	  0x0003 << 10
//#define RF_CH3_ON_BIT	  0x0003 << 12

#define SPI_VBAT_OUT_ON()         HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET)
#define SPI_VBAT_OUT_OFF()        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET)

#define VCC3V3_EN_PIN          GPIO_PIN_1
#define VCC3V3_EN_PORT			   GPIOF

#if ((ARF001 == DEVICE_R1_RPO)||(ARF001 == DEVICE_R1_RPO_MAX))
#define VCC3V3_OUT_ON()         HAL_GPIO_WritePin(VCC3V3_EN_PORT,VCC3V3_EN_PIN,GPIO_PIN_SET)
#define VCC3V3_OUT_OFF()        HAL_GPIO_WritePin(VCC3V3_EN_PORT,VCC3V3_EN_PIN,GPIO_PIN_RESET)
#elif (ARF001 == DEVICE_R1_HAIWAI)
#define VCC3V3_OUT_ON()
#define VCC3V3_OUT_OFF()
#endif

#define EMS_CHANNEL_ON()          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_SET)
#define EMS_CHANNEL_OFF()         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET)
#define RF_CHANNEL_ON()           {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);}
#define RF_CHANNEL_OFF()          {HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);}
void Mbi5020_SPI_Transmit( uint8_t *pData, uint16_t Size, uint32_t Timeout);
extern void Set_Mbi5020_Out(uint16_t output_on_bit);

#endif


