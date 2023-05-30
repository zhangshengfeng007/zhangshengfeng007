#ifndef __IIC_H
#define __IIC_H
#include "includes.h"

 #if (G_SENSOR_SELECT == USE_G_SENSOR)
//#define PIN_SCL    GPIO_Pin_6
//#define PIN_SDA    GPIO_Pin_7
//#define I2C_PORT   GPIOB
#define  L3DH_SCL_Pin         G_SENSOR_SCL_Pin
#define  L3DH_SCL_GPIO_Port   G_SENSOR_SCL_GPIO_Port

#define  L3DH_SDA_Pin         G_SENSOR_SDA_Pin
#define  L3DH_SDA_GPIO_Port   G_SENSOR_SDA_GPIO_Port
//IIC������������
#define L3DH_SCL_Clr() HAL_GPIO_WritePin(L3DH_SCL_GPIO_Port, L3DH_SCL_Pin, GPIO_PIN_RESET);
#define L3DH_SCL_Set() HAL_GPIO_WritePin(L3DH_SCL_GPIO_Port, L3DH_SCL_Pin, GPIO_PIN_SET);
#define L3DH_SDA_Clr() HAL_GPIO_WritePin(L3DH_SDA_GPIO_Port, L3DH_SDA_Pin, GPIO_PIN_RESET);
#define L3DH_SDA_Set() HAL_GPIO_WritePin(L3DH_SDA_GPIO_Port, L3DH_SDA_Pin, GPIO_PIN_SET);
#define L3DH_SDA_Read_In       HAL_GPIO_ReadPin(L3DH_SDA_GPIO_Port,L3DH_SDA_Pin)

//SDA�ķ�������
void L3DH_SDA_IN(void);
void L3DH_SDA_OUT(void);

//I2C���в�������
void IIC_Init(void);                   //��ʼ��I2C��IO��
void LIS3DH_Start(void);                  //����I2C��ʼ�ź�
void LIS3DH_Stop(void);                   //����I2Cֹͣ�ź�
void LIS3DH_SendByte(uint8_t txd);            //I2C����һ���ֽ�
uint8_t LIS3DH_RecvByte(unsigned char ack);   //I2C��ȡһ���ֽ�
uint8_t I2C_Wait_Ack(void);                 //I2C�ȴ�ACK�ź�
void I2C_Ack(void);                    //I2C����ACK�ź�
void I2C_NAck(void);                   //I2C������ACK�ź�

void Single_Write_LIS3DH(uint8_t REG_Address,uint8_t REG_data);
uint8_t Single_Read_LIS3DH(uint8_t REG_Address);
#endif
#endif

