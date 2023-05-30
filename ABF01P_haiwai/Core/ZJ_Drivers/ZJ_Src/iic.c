#include "iic.h"

#if (G_SENSOR_SELECT == USE_G_SENSOR)
#define SlaveAddress 0x32 // 定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                          // ALT  ADDRESS引脚接地时地址为0x30，接电源时地址为0x32

void delay_us(uint16_t time)
{
   uint16_t i = 0;
   while (time--)
   {
      i = 10; // 自己定义
      while (i--)
         ;
   }
}

void IIC_Init(void)
{
   GPIO_InitTypeDef GPIO_Initure;
   __HAL_RCC_GPIOB_CLK_ENABLE();                   // 开启GPIOB时钟
   GPIO_Initure.Pin = L3DH_SCL_Pin | L3DH_SDA_Pin; // PB6,7引脚
   GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;        // 复用推挽输出
   GPIO_Initure.Pull = GPIO_PULLUP;                // 上拉模式
   GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // GPIO速度高速
   HAL_GPIO_Init(GPIOB, &GPIO_Initure);            // 初始化GPIOB
}
// 产生I2C开始信号
void LIS3DH_Start(void)
{
   L3DH_SDA_OUT(); // sda线输出
   L3DH_SDA_Set();
   delay_us(10);
   L3DH_SCL_Set(); // SCL=1
   delay_us(10);
   L3DH_SDA_Clr(); // SDA=0
   delay_us(10);
   L3DH_SCL_Clr(); // SCL=0
}

// 产生I2C停止信号
void LIS3DH_Stop(void)
{
   L3DH_SDA_OUT(); // sda线输出
   L3DH_SDA_Clr(); // SDA=0
   delay_us(10);
   L3DH_SCL_Set(); // SCL=1
   delay_us(10);
   L3DH_SDA_Set();
   delay_us(10);
   L3DH_SCL_Clr(); // SCL=0
   delay_us(10);
}
// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
uint8_t I2C_Wait_Ack(void)
{
   uint8_t ucErrTime = 0;
   L3DH_SDA_IN();  // SDA设置为输入
   L3DH_SDA_Set(); //   GPIO_SetBits(I2C_PORT,PIN_SDA);
   delay_us(5);
   L3DH_SCL_Set();
   delay_us(5);
   while (L3DH_SDA_Read_In == SET)
   {
      ucErrTime++;
      if (ucErrTime > 250)
      {
         LIS3DH_Stop();
         return 1;
      }
   }
   L3DH_SCL_Clr();
   return 0;
}

// 产生ACK应答
void I2C_Ack(void)
{
   L3DH_SCL_Clr();
   L3DH_SDA_OUT();
   L3DH_SDA_Clr();
   delay_us(5);
   L3DH_SCL_Set();
   delay_us(5);
   L3DH_SCL_Clr();
}

// 不产生ACK应答
void I2C_NAck(void)
{
   L3DH_SCL_Clr();
   L3DH_SDA_OUT();
   L3DH_SDA_Set();
   delay_us(5);
   L3DH_SCL_Set();
   delay_us(5);
   L3DH_SCL_Clr();
}

// I2C发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void LIS3DH_SendByte(uint8_t txd)
{
   uint8_t t;
   L3DH_SDA_OUT();
   L3DH_SCL_Clr(); // 拉低时钟开始数据传输
   for (t = 0; t < 8; t++)
   {
      if ((txd & 0x80) == 0x80)
      {
         L3DH_SDA_Set();
      } // SDA=1，写 1
      else
      {
         L3DH_SDA_Clr();
      } // SDA=0，写 0
      txd <<= 1;
      delay_us(5); // 对TEA5767这三个延时都是必须的
      L3DH_SCL_Set();
      delay_us(5);
      L3DH_SCL_Clr();
      delay_us(5);
   }
}

// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t LIS3DH_RecvByte(unsigned char ack)
{
   unsigned char i, receive = 0;
   L3DH_SDA_IN(); // SDA设置为输入
   for (i = 0; i < 8; i++)
   {
      L3DH_SCL_Clr(); //        GPIO_ResetBits(I2C_PORT,PIN_SCL);
      delay_us(5);
      L3DH_SCL_Set(); //        GPIO_SetBits(I2C_PORT,PIN_SCL);
      receive <<= 1;
      if (L3DH_SDA_Read_In == SET)
         receive++;
      delay_us(5);
   }
   if (!ack)
      I2C_NAck(); // 发送nACK
   else
      I2C_Ack(); // 发送ACK
   return receive;
}
void L3DH_SDA_IN(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   /*Configure GPIO pin : PC10 */
   GPIO_InitStruct.Pin = L3DH_SDA_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void L3DH_SDA_OUT(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   GPIO_InitStruct.Pin = L3DH_SDA_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
   L3DH_SDA_Set();
}
//******单字节写入*******************************************

void Single_Write_LIS3DH(uint8_t REG_Address, uint8_t REG_data)
{
   LIS3DH_Start();                // 起始信号
   LIS3DH_SendByte(SlaveAddress); // 发送设备地址+写信号
   I2C_Wait_Ack();
   LIS3DH_SendByte(REG_Address); // 内部寄存器地址，请参考中文pdf22页
   I2C_Wait_Ack();
   LIS3DH_SendByte(REG_data); // 内部寄存器数据，请参考中文pdf22页
   I2C_Wait_Ack();
   LIS3DH_Stop(); // 发送停止信号
}
//********单字节读取*****************************************
uint8_t Single_Read_LIS3DH(uint8_t REG_Address)
{
   uint8_t REG_data;
   LIS3DH_Start();                // 起始信号
   LIS3DH_SendByte(SlaveAddress); // 发送设备地址+写信号
   REG_data = I2C_Wait_Ack();
   LIS3DH_SendByte(REG_Address); // 发送存储单元地址，从0开始
   REG_data = I2C_Wait_Ack();
   LIS3DH_Start();                    // 起始信号
   LIS3DH_SendByte(SlaveAddress + 1); // 发送设备地址+读信号
   REG_data = I2C_Wait_Ack();
   REG_data = LIS3DH_RecvByte(0); // 读出寄存器数据
   LIS3DH_Stop();                 // 停止信号
   return REG_data;
}

#endif
