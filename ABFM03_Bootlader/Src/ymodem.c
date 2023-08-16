/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t Name_Temp[32]="ARF001_updata.bin";        //定义被比较的文件名
extern uint8_t RecvFile_Name[];
/* Private function prototypes -----------------------------------------------*/
/**
* @brief  Receive byte from sender
* @param  c: Character
* @param  timeout: Timeout
* @retval 0: Byte received
*         -1: Timeout
*/
static  int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
{
  while (timeout-- > 0)
  {
    if (SerialKeyPressed(c) == 1)
    {
      return 0;
    }
  }
  return -1;
}

/**
* @brief  Send a byte
* @param  c: Character
* @retval 0: Byte sent
*/
static uint32_t Send_Byte (uint8_t c)
{
  SerialPutChar(c);
  return 0;
}

/**
* @brief  Update CRC16 for input byte
* @param  CRC input value 
* @param  input byte
* @retval Updated CRC value
*/
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
  uint32_t crc = crcIn;
  uint32_t in = byte|0x100;
  
  do
  {
    crc <<= 1;
    in <<= 1;
    
    if(in&0x100)
    {
      ++crc;
    }
    
    if(crc&0x10000)
    {
      crc ^= 0x1021;
    }
  } while(!(in&0x10000));
  
  return (crc&0xffffu);
}

/**
* @brief  Cal CRC16 for YModem Packet
* @param  data
* @param  length
* @retval CRC value
*/
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
  uint32_t crc = 0;
  const uint8_t* dataEnd = data+size;
  
  while(data<dataEnd)
  {
    crc = UpdateCRC16(crc,*data++);
  }
  crc = UpdateCRC16(crc,0);
  crc = UpdateCRC16(crc,0);
  
  return (crc&0xffffu);
}

/**
* @brief  Receive a packet from sender
* @param  data
* @param  length
* @param  timeout
*          0: end of transmission
*          -1: abort by sender
*          >0: packet length
* @retval 0: normally return
*         -1: timeout or packet error
*         1: abort by user
*/
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
  uint16_t i, packet_size, computedcrc;
  uint8_t c;
  *length = 0;
  if (Receive_Byte(&c, timeout) != 0)
  {
    return -1;
  }
  switch (c)
  {
  case SOH:
    packet_size = PACKET_SIZE;
    break;
  case STX:
    packet_size = PACKET_1K_SIZE;
    break;
  case EOT:
    return 0;
  case CA:
    if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
    {
      *length = -1;
      return 0;
    }
    else
    {
      return -1;
    }
  case ABORT1:
  case ABORT2:
    return 1;
  default:
    return -1;
  }
  *data = c;
  for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
  {
    if (Receive_Byte(data + i, timeout) != 0)
    {
      return -1;
    }
  }
  if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
  {
    return -1;
  }
  
  computedcrc = Cal_CRC16(&data[PACKET_HEADER], (uint32_t)packet_size);
  /* Check that received CRC match the already computed CRC value  
  data[packet_size+3]<<8) | data[packet_size+4] contains the received CRC 
  computedcrc contains the computed CRC value */
  if (computedcrc != (uint16_t)((data[packet_size+3]<<8) | data[packet_size+4]))
  {
    return -1;      /* CRC error */
  }
  
  *length = packet_size;
  return 0;
}

/**
* @brief  Receive a file using the ymodem protocol
* @param  buf: Address of the first byte
* @retval The size of the file
*/
//#pragma optimize=none
int32_t Ymodem_Receive (uint8_t *buf)
{
  uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];
  uint8_t file_size[FILE_SIZE_LENGTH];
  uint8_t *file_ptr, *buf_ptr;
  uint8_t session_done = 0,file_done = 0,session_begin = 0;
  int32_t i, packet_length, packets_received = 0, errors = 0, size = 0, temp_test_mun = 0;
  uint32_t flashdestination; 
  uint32_t ramsource;
  uint8_t Receive_FileName_Length;
  
  /* Initialize flashdestination variable */
  flashdestination = APPLICATION_ADDRESS;
  
  for (session_done = 0, errors = 0, session_begin = 0; ;)
  {
    for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
    //for (packets_received = 0, buf_ptr = buf; ;)
    {
      switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
      {
      case 0:
        switch (packet_length)
        {
        case -1:                                                        //* Abort by sender
          Send_Byte(ACK);
          return 0;
          
        case 0:                                                         //* End of transmission
//          HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
//          if(file_done !=1)
//          {
//            Send_Byte(NAK);
//            file_done = 1;
//          }
//          else
//          {
//            Send_Byte(ACK);
//						Send_Byte(CRC16);
//          }
          Send_Byte(ACK);             
          file_done = 1;
          break;
          
        default:                                                        //* Normal packet
          if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
          {
            Send_Byte(NAK);                                             //发送应答NAK，接收失败要求重发
          }
          else
          {
            if (packets_received == 0)                                  //接收第一帧数据
            {
              if (packet_data[PACKET_HEADER] != 0)                      //包含文件信息：文件名，文件长度等
              {
                for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                {
                  RecvFile_Name[i++] = *file_ptr++;                          //保存文件名
                }
                Receive_FileName_Length = i;
                RecvFile_Name[i++] = '\0';
                
                for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < (FILE_SIZE_LENGTH - 1));)
                {
                  file_size[i++] = *file_ptr++;                         //保存文件大小
                }
                file_size[i++] = '\0';
                
                Str2Int(file_size, &size);                              //将文件大小字符串转换成整型  
                
                if(memcmp(RecvFile_Name,Name_Temp,Receive_FileName_Length) )	//比较文件名，相等则继续，不等则返回错误信息          
                {
                  return -4;
                }
                
                temp_test_mun = size;
                
                if (size > (USER_FLASH_SIZE + 1))                       //升级固件过大
                {
                  /* End session */
                  Send_Byte(CA);
                  Send_Byte(CA);                                        //连续发送 2 次中止符 CA
                  return -1;
                }
                /* erase user application area */
                FLASH_If_Erase(APPLICATION_ADDRESS);                    //擦除相应的 flash 空间
                Send_Byte(ACK);
                Send_Byte(CRC16);
                
              }
              else                                                      //文件名数据包为空，结束传输
              {
                Send_Byte(ACK);
                file_done = 1;                                          //停止接收
                session_done = 1;                                       //结束对话
                break;
              }
            }
            else                                                        //收到数据包
            {
              memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);        //拷贝数据
              
//              if(1 == AES_DECODE((unsigned char*)AES_KEY,buf_ptr,1024))         //解密失败返回错误
//              {
//                return -5;
//              }
              
//              LL_GPIO_TogglePin(LV1_LED_OUT_GPIO_Port , LV2_LED_OUT_Pin);
//              LL_GPIO_SetOutputPin(LV1_LED_OUT_GPIO_Port , LV1_LED_OUT_Pin);
              
              ramsource = (uint32_t)buf;																//8 位强制转化成 32 为数据
              
              temp_test_mun = temp_test_mun - packet_length;
              
              if (temp_test_mun >= 0)																		//剩余字节大于128byte的时候烧写对应的FLASH大小
              {
                if (FLASH_If_Write(&flashdestination, &ramsource, (uint16_t) packet_length)  == 0)//烧写升级数据
                {
                  Send_Byte(ACK);     //flash 烧写成功，回复 ACK
                }
                else
                {
                  Send_Byte(CA);
                  Send_Byte(CA);      //flash 烧写错误，连续发送 2 次中止符 CA
                  //HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
                  return -2;
                }
              }
              else   																										//剩余字节小于128byte的时候烧写实际数据占用空间的FLASH大小
              {
                if (FLASH_If_Write(&flashdestination, &ramsource, (uint16_t) (128 + temp_test_mun))  == 0)//烧写实际剩余字节数
                {              
                  Send_Byte(ACK);     //flash 烧写成功，回复 ACK
                }
                else
                {
                  Send_Byte(CA);
                  Send_Byte(CA);      //flash 烧写错误，连续发送 2 次中止符 CA
                  return -2;
                }                        
              } 
            }
            packets_received ++;    //收到数据包的个数                        
            session_begin = 1;      //设置接收中标志
          }
        }
        break;
      case 1:                          //用户中止
        {
          Send_Byte(CA);
          Send_Byte(CA);               //连续发送 2 次中止符 CA
          return -3;                   //烧写中止
        }
      default:
        {
          if (session_begin > 0)        //传输过程中发生错误
          {
            errors ++;
          }
          if (errors > MAX_ERRORS)      //错误超过上限
          {
            Send_Byte(CA);
            Send_Byte(CA);              //连续发送 2 次中止符 CA
            return 0;                  //传输过程发生过多错误
          }
          Send_Byte(CRC16);             //发送'C',继续接收
//          LL_GPIO_TogglePin(LV3_LED_OUT_GPIO_Port , LV3_LED_OUT_Pin);
        }break;
      }
      if (file_done != 0)               //文件接收完毕，退出循环
      {
        break;
      }
    }
    if (session_done != 0)
    {
      break;
    }
  }
  return (int32_t)size;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
