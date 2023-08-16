/**
******************************************************************************
* @file    ZJ_YG6550.c
* @author  L
* @version V1.0
* @date    2021.05.27
* @brief   YG6550 6�ᴫ�������ֳ���
* @check   2021.05.27
* @history
******************************************************************************
*/
#include "includes.h"
#include "YG6550.h"

AxesRaw_t GU6500XYZdataACCEL,GU6500XYZdataGYRO;

extern I2C_HandleTypeDef hi2c1;
const uint32_t I2cxTimeout = EVAL_I2Cx_TIMEOUT_MAX;

extern void MX_I2C1_Init(void);

/**
* ��������: I2Cͨ�Ŵ���������
* �������: ��
* �� �� ֵ: ��
* ˵    ��: һ����I2Cͨ�ų�ʱʱ���øú���
*/
static void I2C_EEPROM_Error (void)
{
  /* ����ʼ��I2Cͨ������ */
  HAL_I2C_DeInit(&hi2c1);
  HAL_Delay(1);
  MX_I2C1_Init();
}


/**
* ��������: ͨ��I2Cд��һ�����ݵ�ָ���Ĵ�����
* �������: Addr��I2C�豸��ַ
*           Reg��Ŀ��Ĵ���
*           RegSize���Ĵ����ߴ�(8λ����16λ)
*           pBuffer��������ָ��
*           Length������������
* �� �� ֵ: HAL_StatusTypeDef���������
* ˵    ��: ��ѭ�����������һ����ʱʱ��
*/
//HAL_StatusTypeDef jdhdhe;
HAL_StatusTypeDef I2C_GY6550_Write(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  //  hi2c1.State = HAL_I2C_STATE_READY;
  status = HAL_I2C_Mem_Write(&hi2c1, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2cxTimeout);

  /* ���I2Cͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ����I2Cͨ�Ŵ��������� */
    I2C_EEPROM_Error();
  }
  return status;
}

/**
* ��������: ͨ��I2C��ȡһ�μĴ������ݴ�ŵ�ָ���Ļ�������
* �������: Addr��I2C�豸��ַ
*           Reg��Ŀ��Ĵ���
*           RegSize���Ĵ����ߴ�(8λ����16λ)
*           pBuffer��������ָ��
*           Length������������
* �� �� ֵ: HAL_StatusTypeDef���������
* ˵    ��: ��
*/
HAL_StatusTypeDef I2C_GY6550_Read(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(&hi2c1, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2cxTimeout);

  /* ���I2Cͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ����I2Cͨ�Ŵ��������� */
    I2C_EEPROM_Error();
  }
  return status;
}
#define GYRO_ADDRESS    0xD4 //���ݵ�ַ
//#define LSM6DSO_W_ADDRESS 0xD4
//#define LSM6DSO_R_ADDRESS 0xD5
#define	SMPLRT_DIV      0x10	//�����ǲ����ʣ�����ֵ��0x07(125Hz)  70
#define	CONFIG          0x11	//�����ǲ����ʣ�����ֵ��0x06(5Hz)    72
//#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
//#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define LSM6DSO_ID       0x6C
#define LSM6DSO_WHO_AM_I 0x0F

uint8_t I2c_Buf_Write[10]={0x70,0x72,0x10,0x01};
uint8_t I2c_Buf_Read[10]={0};

_STATE_enum CHECK_GYRO_IIC(void)
{
  if(I2C_GY6550_Read(GYRO_ADDRESS,LSM6DSO_WHO_AM_I,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Read[0],1) == HAL_OK)
  {
    if(I2c_Buf_Read[0] == LSM6DSO_ID)
    {
      SysInfo.GYRO_SENSER_STATE = INIT_SUCCESS;
      return INIT_SUCCESS;
    }
    else
    {
      SysInfo.GYRO_SENSER_STATE = INIT_ERROR;
      return INIT_ERROR;
    }
  }
  else
  {
    SysInfo.GYRO_SENSER_STATE = INIT_ERROR;
    return INIT_ERROR;
  }
}

void GY6550_INIT(void)
{
  if(CHECK_GYRO_IIC() == INIT_SUCCESS)
  {
    I2C_GY6550_Write(GYRO_ADDRESS,SMPLRT_DIV,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Write[0],1);
    HAL_Delay(1);// ����ʱ������
    I2C_GY6550_Write(GYRO_ADDRESS,CONFIG,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Write[1],1);
    HAL_Delay(1);// ����ʱ������
    I2C_GY6550_Read(GYRO_ADDRESS,SMPLRT_DIV,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Read[0],1);
    HAL_Delay(1);// ����ʱ������
    I2C_GY6550_Read(GYRO_ADDRESS,CONFIG,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Read[1],1);
  }
  else
  {
//taskENTER_CRITICAL();
//    MX_I2C1_Init();
//	printf("\n\r GY6550 INIT FEIL\n\r");
    I2C_GY6550_Write(GYRO_ADDRESS,SMPLRT_DIV,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Write[0],1);
    HAL_Delay(1);// ����ʱ������
    I2C_GY6550_Write(GYRO_ADDRESS,CONFIG,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Write[1],1);
    HAL_Delay(1);// ����ʱ������
    I2C_GY6550_Read(GYRO_ADDRESS,SMPLRT_DIV,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Read[0],1);
    HAL_Delay(1);// ����ʱ������
    I2C_GY6550_Read(GYRO_ADDRESS,CONFIG,I2C_MEMADD_SIZE_8BIT,&I2c_Buf_Read[1],1);
//	taskEXIT_CRITICAL();
  }
}

#define	ACCEL_XOUT_H	0x29
#define	ACCEL_XOUT_L	0x28
#define	ACCEL_YOUT_H	0x2B
#define	ACCEL_YOUT_L	0x2A
#define	ACCEL_ZOUT_H	0x2D
#define	ACCEL_ZOUT_L	0x2C
//******��ȡMPU9250����****************************************
void READ_MPU6500_ACCEL(void)
{
  uint8_t buffer[2];

  I2C_GY6550_Read(GYRO_ADDRESS,ACCEL_XOUT_H,I2C_MEMADD_SIZE_8BIT,&buffer[1],1);
  I2C_GY6550_Read(GYRO_ADDRESS,ACCEL_XOUT_L,I2C_MEMADD_SIZE_8BIT,&buffer[0],1);
  GU6500XYZdataACCEL.AXIS_X = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataACCEL.AXIS_X /=164;//��ȡ����X������
  //	GU6500XYZdataACCEL.AXIS_X = GU6500XYZdataACCEL.AXIS_X >> 4;//��ȡ����X������

  I2C_GY6550_Read(GYRO_ADDRESS,ACCEL_YOUT_H,I2C_MEMADD_SIZE_8BIT,&buffer[1],1);
  I2C_GY6550_Read(GYRO_ADDRESS,ACCEL_YOUT_L,I2C_MEMADD_SIZE_8BIT,&buffer[0],1);
  GU6500XYZdataACCEL.AXIS_Y = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataACCEL.AXIS_Y /=164;//��ȡ����Y������
  //	GU6500XYZdataACCEL.AXIS_Y = GU6500XYZdataACCEL.AXIS_Y >> 4;//��ȡ����X������

  I2C_GY6550_Read(GYRO_ADDRESS,ACCEL_ZOUT_H,I2C_MEMADD_SIZE_8BIT,&buffer[1],1);
  I2C_GY6550_Read(GYRO_ADDRESS,ACCEL_ZOUT_L,I2C_MEMADD_SIZE_8BIT,&buffer[0],1);
  GU6500XYZdataACCEL.AXIS_Z = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataACCEL.AXIS_Z /=164; //��ȡ����Z������
  //	GU6500XYZdataACCEL.AXIS_Z = GU6500XYZdataACCEL.AXIS_Z >> 4;//��ȡ����X������

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
  //	GU6500XYZdataGYRO.AXIS_X /=164;//��ȡ����X������
  //	GU6500XYZdataGYRO.AXIS_X = GU6500XYZdataGYRO.AXIS_X >> 4;//��ȡ����X������

  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_YOUT_H,I2C_MEMADD_SIZE_8BIT,&buffer[1],1);
  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_YOUT_L,I2C_MEMADD_SIZE_8BIT,&buffer[0],1);
  GU6500XYZdataGYRO.AXIS_Y = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataGYRO.AXIS_Y /=164;//��ȡ����Y������
  //	GU6500XYZdataGYRO.AXIS_Y = GU6500XYZdataGYRO.AXIS_Y >> 4;//��ȡ����X������

  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_ZOUT_H,I2C_MEMADD_SIZE_8BIT,&buffer[1],1);
  I2C_GY6550_Read(GYRO_ADDRESS,GYRO_ZOUT_L,I2C_MEMADD_SIZE_8BIT,&buffer[0],1);
  GU6500XYZdataGYRO.AXIS_Z = (buffer[1]<<8)|buffer[0];
  //	GU6500XYZdataGYRO.AXIS_Z /=164; //��ȡ����Z������
  //	GU6500XYZdataGYRO.AXIS_Z = GU6500XYZdataGYRO.AXIS_Z >> 4;//��ȡ����X������

}

void GY6500_Read()
{
  if(CHECK_GYRO_IIC() == INIT_SUCCESS)
  {
    READ_MPU6500_ACCEL();
    READ_MPU6500_GYRO();
  }
}

uint8_t g_ACCELquick = 0;///��������ƶ���־λ
///������ᴫ������Ҫ׼ȷ�������жϣ����ܻ���Ҫ������̬�ж�
///���ᴦ������
uint32_t ACCELDataBIG;
uint32_t ACCELDataLittle;

void ACCELDataProcessing() ////?��?��?��
{
  static AxesRaw_t LastXYZdata;
  static uint16_t count = 0;
  static uint32_t ACCELDifference = 0;////��y?��2??��

  ACCELDifference += abs(LastXYZdata.AXIS_X - GU6500XYZdataACCEL.AXIS_X) +
    abs(LastXYZdata.AXIS_Y - GU6500XYZdataACCEL.AXIS_Y) +
      abs(LastXYZdata.AXIS_Z - GU6500XYZdataACCEL.AXIS_Z);

  count++;
  if(count>=10)
  {
    count = 0;

    if(ACCELDifference > 400 * 0x0f)//400 * 0x0f)//500*50*3)///��y?��?������
    {
      if(ACCELDifference > 4000 * 0x0f)//5000 * 0x0f)
      {
        ACCELDataBIG = 1;
      }
      else
      {
        ACCELDataLittle = 1;
        ACCELDataBIG = 0;
      }
    }
    else//��y?��?������1yD?��??��?�¨���?1??DD
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

void GYRODataProcessing() ///???��?��
{
  static uint16_t countss = 0;
  static uint32_t GYROVariation = 0;////��y?��2??��

  GYROVariation += abs(GU6500XYZdataGYRO.AXIS_X) +
    abs(GU6500XYZdataGYRO.AXIS_Y) +
      abs(GU6500XYZdataGYRO.AXIS_Z);

  countss++;
  if(countss>=10)
  {
    countss = 0;
    if(GYROVariation > 400 * 0x0f)//500*50*3)///��y?��?������
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
    else//��y?��?������1yD?��??��?�¨���?1??DD
    {
      GYROLDataLittle = 0;
      GYRODataBIG = 0;
    }

    GYROVariation = 0;
  }
}

//================================================================================================
// Descriptions:          6?����y?Y��|����o����y
// Input parameters:
// Output parameters:
// Returned value:
// Created by:        zhangx
// Created date:      2021.05.27
//------------------------------------------------------------------------------------------------
// Modified by:
// Modified date:
// Descriptions:
//                 25ms调用一次
//=================================================================================================
uint8_t AttitudeProcessing(void)
{
  static uint16_t countss = 0;
  static uint16_t timer = 0;
  static uint8_t time_cnt_flag = 0;
  static uint8_t flage = 0;

  countss++;

  if(countss%2 == 0)
  {
    GY6500_Read();
    ACCELDataProcessing();  //?��?��?�� ACCEL
    GYRODataProcessing();   //����?Y   GYRO
  }

  if(countss >= 8)//10)//20)
  {
//        printf("\n\r%d,%d,%d\n\r", GU6500XYZdataACCEL.AXIS_X , GU6500XYZdataACCEL.AXIS_Y,GU6500XYZdataACCEL.AXIS_Z );
//        printf("\n\r%d,%d,%d\n\r", GU6500XYZdataGYRO.AXIS_X , GU6500XYZdataGYRO.AXIS_Y,GU6500XYZdataGYRO.AXIS_Z );
    countss = 0;
    flage = 0;
    //      ACCELDataLittle = 1;
    //      ACCELDataBIG = 0;  //��?1??��?��?��
    //      GYROLDataLittle = 1;//???��?��
    //      GYRODataBIG = 0;
    if((ACCELDataBIG == 0)&&(GYRODataBIG == 1))//��?1??��?��?��D? ?��????��D?��?��?��  ???��?���䨮
    {
      flage = 1;
    }
    else if((ACCELDataBIG == 1)&&(GYRODataBIG == 0))//?��?��?���䨮      ???��?��D? ?��?? ??��D?��?��?��
    {
      flage = 1;
    }
    else if((ACCELDataBIG == 1)&&(GYRODataBIG == 1))//?��?��?���䨮      ???��?���䨮
    {
      flage = 1;
    }
    else if((ACCELDataLittle == 1)&&(GYRODataBIG == 0))//?��?��?��D?      ???��?��D?
    {
      flage = 1;
    }

  }

  if(flage)
  {
    time_cnt_flag = 1;
    timer = 8;//10;//20;//30;
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

//	printf1("\n\r Touch=%d,ACC_BIG=%d,ACCE_Little=%d,GYRODataBIG=%d\n\r",SysInfo.Skin_Touch_Flag,ACCELDataBIG,ACCELDataLittle,GYRODataBIG);


  if((!GU6500XYZdataACCEL.AXIS_X && !GU6500XYZdataACCEL.AXIS_Y && !GU6500XYZdataACCEL.AXIS_Z) &&
    (!GU6500XYZdataGYRO.AXIS_X && !GU6500XYZdataGYRO.AXIS_Y && !GU6500XYZdataGYRO.AXIS_Z))
  {
    GY6550_INIT();
  }

  return time_cnt_flag;
}



