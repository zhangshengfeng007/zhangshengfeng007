/**
******************************************************************************
* @file    ZJ_YG6550.c
* @author  L
* @version V1.0
* @date    2021.05.27
* @brief   YG6550 6轴传感器部分程序  
* @check   2021.05.27
* @history 
******************************************************************************
*/
#include "includes.h"


#define GYRO_ADDRESS        0xD4 //陀螺地址
//#define LSM6DSO_W_ADDRESS 0xD4	
//#define LSM6DSO_R_ADDRESS 0xD5
#define	SMPLRT_DIV          0x10	//陀螺仪采样率，典型值：0x07(125Hz)  70
#define	CONFIG              0x11	//陀螺仪采样率，典型值：0x06(5Hz)    72
//#define	GYRO_CONFIG		    0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
//#define	ACCEL_CONFIG	    0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define LSM6DSO_ID          0x6C
#define LSM6DSO_WHO_AM_I    0x0F


AxesRaw_t GU6500XYZdataACCEL,GU6500XYZdataGYRO;

extern I2C_HandleTypeDef hi2c1;
const uint32_t I2cxTimeout = EVAL_I2Cx_TIMEOUT_MAX;

extern void MX_I2C1_Init(void);

/**
* 函数功能: I2C通信错误处理函数
* 输入参数: 无
* 返 回 值: 无
* 说    明: 一般在I2C通信超时时调用该函数
*/
static void I2C_EEPROM_Error (void)
{
  /* 反初始化I2C通信总线 */
  HAL_I2C_DeInit(&hi2c1);
  HAL_Delay(1);
  MX_I2C1_Init();
}


/**
* 函数功能: 通过I2C写入一段数据到指定寄存器内
* 输入参数: Addr：I2C设备地址
*           Reg：目标寄存器
*           RegSize：寄存器尺寸(8位或者16位)
*           pBuffer：缓冲区指针
*           Length：缓冲区长度
* 返 回 值: HAL_StatusTypeDef：操作结果
* 说    明: 在循环调用是需加一定延时时间
*/
//HAL_StatusTypeDef jdhdhe;
HAL_StatusTypeDef I2C_GY6550_Write(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  //  hi2c1.State = HAL_I2C_STATE_READY;
  status = HAL_I2C_Mem_Write(&hi2c1, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2cxTimeout); 
  
  /* 检测I2C通信状态 */
  if(status != HAL_OK)
  {
    /* 调用I2C通信错误处理函数 */
    I2C_EEPROM_Error();
  }        
  return status;
}

/**
* 函数功能: 通过I2C读取一段寄存器内容存放到指定的缓冲区内
* 输入参数: Addr：I2C设备地址
*           Reg：目标寄存器
*           RegSize：寄存器尺寸(8位或者16位)
*           pBuffer：缓冲区指针
*           Length：缓冲区长度
* 返 回 值: HAL_StatusTypeDef：操作结果
* 说    明: 无
*/
HAL_StatusTypeDef I2C_GY6550_Read(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2cxTimeout);
  
  /* 检测I2C通信状态 */
  if(status != HAL_OK)
  {
    /* 调用I2C通信错误处理函数 */
    I2C_EEPROM_Error();
  }        
  return status;
}


uint8_t I2c_Buf_Write[10]={0x70,0x72,0x10,0x01};
uint8_t I2c_Buf_Read[10]={0};

_init_result_e CHECK_GYRO_IIC(void)
{
  if(I2C_GY6550_Read(GYRO_ADDRESS,LSM6DSO_WHO_AM_I,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Read[0],1) == HAL_OK)
  {
    if(I2c_Buf_Read[0] == LSM6DSO_ID)
    {
      Sys_Info.G_sensor_state = INIT_SUCCESS;
      return INIT_SUCCESS;
    }
    else
    {
      Sys_Info.G_sensor_state = INIT_ERROR;
      return INIT_ERROR;
    }
  }
  else
  {
    Sys_Info.G_sensor_state = INIT_ERROR;
    return INIT_ERROR;
  }
}

void GY6550_INIT(void)
{
  if(CHECK_GYRO_IIC() == INIT_SUCCESS)
  {
    I2C_GY6550_Write(GYRO_ADDRESS,SMPLRT_DIV,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Write[0],1);
    HAL_Delay(1);// 短延时不能少
    I2C_GY6550_Write(GYRO_ADDRESS,CONFIG,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Write[1],1);
    HAL_Delay(1);// 短延时不能少
    I2C_GY6550_Read(GYRO_ADDRESS,SMPLRT_DIV,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Read[0],1);
    HAL_Delay(1);// 短延时不能少
    I2C_GY6550_Read(GYRO_ADDRESS,CONFIG,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Read[1],1);
  }
}

#define	ACCEL_XOUT_H	0x29
#define	ACCEL_XOUT_L	0x28
#define	ACCEL_YOUT_H	0x2B
#define	ACCEL_YOUT_L	0x2A
#define	ACCEL_ZOUT_H	0x2D
#define	ACCEL_ZOUT_L	0x2C
//******读取MPU9250数据****************************************
void READ_MPU6500_ACCEL(void)
{
  uint8_t buffer[2];
  
  I2C_GY6550_Read(GYRO_ADDRESS, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &buffer[1], 1);
  I2C_GY6550_Read(GYRO_ADDRESS, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &buffer[0], 1);
  GU6500XYZdataACCEL.AXIS_X = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataACCEL.AXIS_X /=164;//读取计算X轴数据
  //	GU6500XYZdataACCEL.AXIS_X = GU6500XYZdataACCEL.AXIS_X >> 4;//读取计算X轴数据
  
  I2C_GY6550_Read(GYRO_ADDRESS, ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &buffer[1], 1);
  I2C_GY6550_Read(GYRO_ADDRESS, ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &buffer[0], 1);
  GU6500XYZdataACCEL.AXIS_Y = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataACCEL.AXIS_Y /=164;//读取计算Y轴数据
  //	GU6500XYZdataACCEL.AXIS_Y = GU6500XYZdataACCEL.AXIS_Y >> 4;//读取计算X轴数据
  
  I2C_GY6550_Read(GYRO_ADDRESS, ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &buffer[1], 1);
  I2C_GY6550_Read(GYRO_ADDRESS, ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &buffer[0], 1);
  GU6500XYZdataACCEL.AXIS_Z = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataACCEL.AXIS_Z /=164; //读取计算Z轴数据
  //	GU6500XYZdataACCEL.AXIS_Z = GU6500XYZdataACCEL.AXIS_Z >> 4;//读取计算X轴数据
  
}

#define	GYRO_XOUT_H		0x23
#define	GYRO_XOUT_L		0x22	
#define	GYRO_YOUT_H		0x25
#define	GYRO_YOUT_L		0x24
#define	GYRO_ZOUT_H		0x27
#define	GYRO_ZOUT_L		0x26
void READ_MPU6500_GYRO(void)
{
  uint8_t buffer[2];

  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_XOUT_H,I2C_MEMADD_SIZE_8BIT,&buffer[1],1);
  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_XOUT_L,I2C_MEMADD_SIZE_8BIT,&buffer[0],1);
  GU6500XYZdataGYRO.AXIS_X = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataGYRO.AXIS_X /=164;//读取计算X轴数据
  //	GU6500XYZdataGYRO.AXIS_X = GU6500XYZdataGYRO.AXIS_X >> 4;//读取计算X轴数据
  
  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_YOUT_H,I2C_MEMADD_SIZE_8BIT,&buffer[1],1);
  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_YOUT_L,I2C_MEMADD_SIZE_8BIT,&buffer[0],1);
  GU6500XYZdataGYRO.AXIS_Y = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataGYRO.AXIS_Y /=164;//读取计算Y轴数据
  //	GU6500XYZdataGYRO.AXIS_Y = GU6500XYZdataGYRO.AXIS_Y >> 4;//读取计算X轴数据
  
  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_ZOUT_H,I2C_MEMADD_SIZE_8BIT,&buffer[1],1);
  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_ZOUT_L,I2C_MEMADD_SIZE_8BIT,&buffer[0],1);
  GU6500XYZdataGYRO.AXIS_Z = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataGYRO.AXIS_Z /=164; //读取计算Z轴数据
  //	GU6500XYZdataGYRO.AXIS_Z = GU6500XYZdataGYRO.AXIS_Z >> 4;//读取计算X轴数据
  
}

void GY6500_Read()
{
  if(CHECK_GYRO_IIC() == INIT_SUCCESS)
  {
    READ_MPU6500_ACCEL();
    READ_MPU6500_GYRO();
  }
}

uint8_t g_ACCELquick = 0;///三轴高速移动标志位
///如果三轴传感器需要准确的做出判断，可能还需要进行姿态判断
///三轴处理函数
uint32_t ACCELDataBIG;
uint32_t ACCELDataLittle;

void ACCELDataProcessing() ////加速度
{
  static AxesRaw_t LastXYZdata;
  static uint16_t count;
  static uint32_t ACCELDifference;////三轴差值		
  static uint32_t ACCELdata;////三轴瞬间加速
  
  ACCELDifference += abs(LastXYZdata.AXIS_X - GU6500XYZdataACCEL.AXIS_X) + 
    abs(LastXYZdata.AXIS_Y - GU6500XYZdataACCEL.AXIS_Y) + 
      abs(LastXYZdata.AXIS_Z - GU6500XYZdataACCEL.AXIS_Z);
  
  ACCELdata = abs(GU6500XYZdataACCEL.AXIS_X) + 
    abs(GU6500XYZdataACCEL.AXIS_Y) + 
      abs(GU6500XYZdataACCEL.AXIS_Z);
  if(ACCELdata > 0xa000)
  {
    g_ACCELquick = 1;
  }
  
  count++;
  if(count>=10)
  {
    count = 0;
    
    if(ACCELDifference > 350 * 0x0f)//500*50*3)///三轴动作
    {
      if(ACCELDifference > 5000 * 0x0f)
      {
        ACCELDataBIG = 1;
      }
      else
      {
        ACCELDataLittle = 1;
        ACCELDataBIG = 0;
      }
    }
    else//三轴动作过小，机器停止运行
    {
      ACCELDataLittle = 0;
      ACCELDataBIG = 0;
    }		
    ACCELDifference = 0;
  }
  
  LastXYZdata.AXIS_X = GU6500XYZdataACCEL.AXIS_X;
  LastXYZdata.AXIS_Y = GU6500XYZdataACCEL.AXIS_Y;
  LastXYZdata.AXIS_Z = GU6500XYZdataACCEL.AXIS_Z;
}

uint32_t GYRODataBIG;
uint32_t GYROLDataLittle;

void GYRODataProcessing() ///角速度
{
  static uint16_t countss;
  static uint32_t GYROVariation;////三轴差值	
  
  GYROVariation += abs(GU6500XYZdataGYRO.AXIS_X) + 
    abs(GU6500XYZdataGYRO.AXIS_Y) + 
      abs(GU6500XYZdataGYRO.AXIS_Z);	
  
  countss++;
  if(countss>=10)
  {
    countss = 0;
    if(GYROVariation > 300 * 0x0f)//500*50*3)///三轴动作
    {
      GYROLDataLittle  = 1;
      if(GYROVariation > 5500 * 0x0f)
      {
        GYRODataBIG = 1;
      }
      else
      {
        GYROLDataLittle = 1;
        GYRODataBIG = 0;
      }
    }
    else//三轴动作过小，机器停止运行
    {
      GYROLDataLittle = 0;
      GYRODataBIG = 0;
    }		
    
    GYROVariation = 0;
  }
}

//================================================================================================
// Descriptions:          6轴数据处理函数
// Input parameters:      
// Output parameters:
// Returned value:
// Created by:        zhangx
// Created date:      2021.05.27
//------------------------------------------------------------------------------------------------
// Modified by:
// Modified date:
// Descriptions:
//
//=================================================================================================
uint8_t hdhd1[5];
uint8_t AttitudeProcessing(void)
{
  static uint16_t countss;
  static uint16_t timer = 0;
  static uint8_t time_cnt_flag = 0;
  static uint8_t flage = 0;
  
  GY6500_Read();
  ACCELDataProcessing();  //加速度 ACCEL
  GYRODataProcessing();   //陀螺   GYRO
  
  if(++countss>=10)
  {
    //		printf("%d,%d,%d\n", GU6500XYZdataACCEL.AXIS_X , GU6500XYZdataACCEL.AXIS_Y,GU6500XYZdataACCEL.AXIS_Z );
    //		printf("%d,%d,%d\n", GU6500XYZdataGYRO.AXIS_X , GU6500XYZdataGYRO.AXIS_Y,GU6500XYZdataGYRO.AXIS_Z );
    countss = 0;	
    flage = 0;
    //			ACCELDataLittle = 1;
    //			ACCELDataBIG = 0;	//如果加速度
    //			GYROLDataLittle = 1;//角速度
    //			GYRODataBIG = 0;	
    if((ACCELDataBIG == 0)&&(GYRODataBIG == 1))//如果加速度小 或者没有加速度  角速度大
    {
      hdhd1[0]++; ///不输出
      flage = 1;
    }
    else if((ACCELDataBIG == 1)&&(GYRODataBIG == 0))//加速度大      角速度小 或者 没有加速度 
    {
      hdhd1[1]++;
      flage = 1;
    }
    else if((ACCELDataBIG == 1)&&(GYRODataBIG == 1))//加速度大      角速度大 
    {
      hdhd1[2]++;//不输出
      flage = 1;
    }
    else if((ACCELDataLittle == 1)&&(GYRODataBIG == 0))//加速度小      角速度小
    {
      hdhd1[3]++;
//      flage = 1;
    }	
    else
    {
      //   hdhd1[4]++;
    }
    //   printf("%d,%d,%d,%d\n", hdhd1[0],hdhd1[1],hdhd1[2],hdhd1[3] );	
  }
  
  if(flage)
  {
    time_cnt_flag = 1;
    timer = 20;
  }
  else
  {
    if(time_cnt_flag)
    {
      if(timer > 0)
      {
        timer--;
      }
      else
      {
        time_cnt_flag = 0;
      }
    }
  }
  
  return time_cnt_flag;
}

/*

判断办法 

如果加速 变化大 就证明在运动
如果 单纯角速度变化大  证明不在运动
如果 加速度和角速度都很大  也证明在运动 


*/



