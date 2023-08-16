/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t Name_Temp[32]="ARF001_updata.bin";        //���屻�Ƚϵ��ļ���
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
            Send_Byte(NAK);                                             //����Ӧ��NAK������ʧ��Ҫ���ط�
          }
          else
          {
            if (packets_received == 0)                                  //���յ�һ֡����
            {
              if (packet_data[PACKET_HEADER] != 0)                      //�����ļ���Ϣ���ļ������ļ����ȵ�
              {
                for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                {
                  RecvFile_Name[i++] = *file_ptr++;                          //�����ļ���
                }
                Receive_FileName_Length = i;
                RecvFile_Name[i++] = '\0';
                
                for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < (FILE_SIZE_LENGTH - 1));)
                {
                  file_size[i++] = *file_ptr++;                         //�����ļ���С
                }
                file_size[i++] = '\0';
                
                Str2Int(file_size, &size);                              //���ļ���С�ַ���ת��������  
                
                if(memcmp(RecvFile_Name,Name_Temp,Receive_FileName_Length) )	//�Ƚ��ļ��������������������򷵻ش�����Ϣ          
                {
                  return -4;
                }
                
                temp_test_mun = size;
                
                if (size > (USER_FLASH_SIZE + 1))                       //�����̼�����
                {
                  /* End session */
                  Send_Byte(CA);
                  Send_Byte(CA);                                        //�������� 2 ����ֹ�� CA
                  return -1;
                }
                /* erase user application area */
                FLASH_If_Erase(APPLICATION_ADDRESS);                    //������Ӧ�� flash �ռ�
                Send_Byte(ACK);
                Send_Byte(CRC16);
                
              }
              else                                                      //�ļ������ݰ�Ϊ�գ���������
              {
                Send_Byte(ACK);
                file_done = 1;                                          //ֹͣ����
                session_done = 1;                                       //�����Ի�
                break;
              }
            }
            else                                                        //�յ����ݰ�
            {
              memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);        //��������
              
//              if(1 == AES_DECODE((unsigned char*)AES_KEY,buf_ptr,1024))         //����ʧ�ܷ��ش���
//              {
//                return -5;
//              }
              
//              LL_GPIO_TogglePin(LV1_LED_OUT_GPIO_Port , LV2_LED_OUT_Pin);
//              LL_GPIO_SetOutputPin(LV1_LED_OUT_GPIO_Port , LV1_LED_OUT_Pin);
              
              ramsource = (uint32_t)buf;																//8 λǿ��ת���� 32 Ϊ����
              
              temp_test_mun = temp_test_mun - packet_length;
              
              if (temp_test_mun >= 0)																		//ʣ���ֽڴ���128byte��ʱ����д��Ӧ��FLASH��С
              {
                if (FLASH_If_Write(&flashdestination, &ramsource, (uint16_t) packet_length)  == 0)//��д��������
                {
                  Send_Byte(ACK);     //flash ��д�ɹ����ظ� ACK
                }
                else
                {
                  Send_Byte(CA);
                  Send_Byte(CA);      //flash ��д������������ 2 ����ֹ�� CA
                  //HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
                  return -2;
                }
              }
              else   																										//ʣ���ֽ�С��128byte��ʱ����дʵ������ռ�ÿռ��FLASH��С
              {
                if (FLASH_If_Write(&flashdestination, &ramsource, (uint16_t) (128 + temp_test_mun))  == 0)//��дʵ��ʣ���ֽ���
                {              
                  Send_Byte(ACK);     //flash ��д�ɹ����ظ� ACK
                }
                else
                {
                  Send_Byte(CA);
                  Send_Byte(CA);      //flash ��д������������ 2 ����ֹ�� CA
                  return -2;
                }                        
              } 
            }
            packets_received ++;    //�յ����ݰ��ĸ���                        
            session_begin = 1;      //���ý����б�־
          }
        }
        break;
      case 1:                          //�û���ֹ
        {
          Send_Byte(CA);
          Send_Byte(CA);               //�������� 2 ����ֹ�� CA
          return -3;                   //��д��ֹ
        }
      default:
        {
          if (session_begin > 0)        //��������з�������
          {
            errors ++;
          }
          if (errors > MAX_ERRORS)      //���󳬹�����
          {
            Send_Byte(CA);
            Send_Byte(CA);              //�������� 2 ����ֹ�� CA
            return 0;                  //������̷����������
          }
          Send_Byte(CRC16);             //����'C',��������
//          LL_GPIO_TogglePin(LV3_LED_OUT_GPIO_Port , LV3_LED_OUT_Pin);
        }break;
      }
      if (file_done != 0)               //�ļ�������ϣ��˳�ѭ��
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
