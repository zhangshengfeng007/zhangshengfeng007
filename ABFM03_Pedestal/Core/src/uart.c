#include "uart.h"
#include <string.h>
#include "flash.h"

#define USART_RECEIVE_BUF_LEN   100


#pragma pack(1)
typedef struct
{
uint8_t usart_receive_buf[USART_RECEIVE_BUF_LEN];	//串口接收缓存
uint8_t usart_send_buf[USART_RECEIVE_BUF_LEN];
uint8_t receive_frame_data[USART_RECEIVE_BUF_LEN];
uint16_t usart_receive_len;
uint16_t frame_len;
uint8_t frame_num;
uint8_t usart_send_len;
uint8_t is_full;
uint8_t status;
bool receive_complish;
}_usart_buf_TypeDef;
#pragma pack()


RCC_ClocksType RCC_Clocks;

_usart_buf_TypeDef uart_buf;

uint32_t peer_code=0;

_TEST_MODE_HANDLE_TypeDef test_mode={0,0,0,0,0,0};
#ifdef __GNUC__

  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

#else

  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif /* __GNUC__ */

int fputc(int ch,FILE *f)
{
	LPUART_SendData((uint8_t)ch);
	while (LPUART_GetFlagStatus(LPUART_FLAG_TXC) == RESET);
	LPUART_ClrFlag(LPUART_FLAG_TXC);

	return ch;
}


void clear_uart_buf(void)
{
	memset(&uart_buf,0,sizeof(_usart_buf_TypeDef));
}


void MX_USART1_UART_Init(void)
{
	//LPUSRT_TX<->PA6
	//LPUSRT_RX<->PA7

	LPUART_InitType LPUART_InitStructure;
	GPIO_InitType GPIO_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	DMA_InitType DMA_InitStructure;

	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA/* | RCC_APB2_PERIPH_AFIO*/, ENABLE);
	RCC_ConfigLPUARTClk(RCC_LPUARTCLK_SRC_APB1);
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_LPUART, ENABLE);
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);

	LPUART_DeInit();

	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.GPIO_Pull= GPIO_PULL_UP;
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_LPUART;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.GPIO_Pull= GPIO_PULL_UP;
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_LPUART;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel                   = LPUART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority           = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	DMA_DeInit(DMA_CH1);
	DMA_InitStructure.PeriphAddr     = (LPUART_BASE + 0x10);
	DMA_InitStructure.MemAddr        = (uint32_t)(uart_buf.usart_send_buf);
	DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
	DMA_InitStructure.BufSize        = USART_RECEIVE_BUF_LEN;
	DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
	DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
	DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.CircularMode   = DMA_MODE_NORMAL;
	DMA_InitStructure.Priority       = DMA_PRIORITY_VERY_HIGH;
	DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
	DMA_Init(DMA_CH1, &DMA_InitStructure);
	DMA_RequestRemap(DMA_REMAP_LPUART_TX, DMA, DMA_CH1, ENABLE);

	/* Enable LPUARTy Tx DMA Channel Transfer Complete Interrupt */
	DMA_ConfigInt(DMA_CH1, DMA_INT_TXC, ENABLE);


	/* Enable the USARTy_Tx DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel            = DMA_Channel1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority    = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	LPUART_InitStructure.BaudRate            = 115200;
	LPUART_InitStructure.Parity              = LPUART_PE_NO;
	LPUART_InitStructure.RtsThreshold        = LPUART_RTSTH_FIFOFU;
	LPUART_InitStructure.HardwareFlowControl = LPUART_HFCTRL_NONE;
	LPUART_InitStructure.Mode                = LPUART_MODE_RX | LPUART_MODE_TX;
//	LPUART_Init(&LPUART_InitStructure);

  LPUART_ClrIntPendingBit(LPUART_INT_FIFO_NE);
	LPUART_FlushRxFifo();

	while(LPUART_GetFlagStatus(LPUART_FLAG_FIFO_NE) != RESET)
	{
			LPUART_ReceiveData();
			LPUART_ClrFlag(LPUART_FLAG_FIFO_NE);
	}

	LPUART_ConfigInt(LPUART_INT_FIFO_NE, ENABLE);

	LPUART_EnableDMA(LPUART_DMAREQ_TX, ENABLE);
	DMA_EnableChannel(DMA_CH1, DISABLE);

	clear_uart_buf();

	LPUART_Init(&LPUART_InitStructure);
}

void  MX_TIMER6_Init(void)
{
	//APB1Pescale=4,APB1PeriphClk=12MHZ;if(APB1Pescale==1) {TIM3_6_CLK=APB1PeriphClkX1=12MHZ}  else {TIM3_6_CLK=APB1PeriphClkX2=24MHZ}


	TIM_TimeBaseInitType TIM_TimeBaseStructure;
	NVIC_InitType NVIC_InitStructure;

	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM6, ENABLE);

	TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.Period    = (12000-1);//定时5ms
	TIM_TimeBaseStructure.Prescaler = (10-1);
	TIM_TimeBaseStructure.ClkDiv    = 0;
	TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
	TIM_InitTimeBase(TIM6, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel                   = LPTIM_TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority           = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

	TIM_ConfigPrescaler(TIM6, 10-1, TIM_PSC_RELOAD_MODE_IMMEDIATE);
	NVIC_Init(&NVIC_InitStructure);
	TIM_ConfigInt(TIM6, TIM_INT_UPDATE, ENABLE);
	TIM_Enable(TIM6, ENABLE);


	RCC_GetClocksFreqValue(&RCC_Clocks);
}



void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXDNE) != RESET)
	{
			//接收数据放入buf中
	if(uart_buf.usart_receive_len<USART_RECEIVE_BUF_LEN)
	{
		uart_buf.usart_receive_buf[uart_buf.usart_receive_len]=LPUART_ReceiveData();
		uart_buf.usart_receive_len++;
	}
	else
	{
		uart_buf.is_full=0;
	}
	TIM_Enable(TIM6, DISABLE);
	TIM_SetCnt(TIM6, 0x0000);//复位超时定器
	TIM_Enable(TIM6, ENABLE);
  USART_ClrIntPendingBit(USART1, USART_FLAG_RXDNE);

	}
}


void LPUART_IRQHandler(void)
{
    if (LPUART_GetIntStatus(LPUART_INT_FIFO_NE) != RESET)
    {
				//接收数据放入buf中
				if(uart_buf.usart_receive_len<USART_RECEIVE_BUF_LEN)
				{
					uart_buf.usart_receive_buf[uart_buf.usart_receive_len]=LPUART_ReceiveData();
					uart_buf.usart_receive_len++;
				}
				else
				{
					uart_buf.is_full=0;
				}

				TIM_Enable(TIM6, DISABLE);
				TIM_SetCnt(TIM6, 0x0000);//复位超时定器
				TIM_Enable(TIM6, ENABLE);
		}
}



void DMA_Channel1_2_IRQHandler(void)
{
    if(DMA_GetIntStatus(DMA_INT_TXC1, DMA) != RESET)
    {

        DMA_ConfigInt(DMA_CH1, DMA_INT_TXC, DISABLE);
        DMA_ClrIntPendingBit(DMA_INT_TXC1, DMA);
    }

}






void LPTIM_TIM6_IRQHandler(void)
{
	if (TIM_GetIntStatus(TIM6, TIM_INT_UPDATE) != RESET)
	{
			//设置协议处理标志位
		  memcpy(uart_buf.receive_frame_data,uart_buf.usart_receive_buf,uart_buf.usart_receive_len);
		  uart_buf.frame_len=uart_buf.usart_receive_len;
		  uart_buf.usart_receive_len=0;
		  uart_buf.frame_num++;

			TIM_Enable(TIM6, DISABLE);
		  TIM_SetCnt(TIM6, 0x0000);//复位超时定器
			TIM_ClrIntPendingBit(TIM6, TIM_INT_UPDATE);
	}
}


void send_data_to_usart(uint8_t *pbuf,uint8_t len)
{
	DMA_EnableChannel(DMA_CH1, DISABLE);
	memcpy(uart_buf.usart_send_buf,pbuf,len);
	uart_buf.usart_send_len=len;
	DMA_SetCurrDataCounter(DMA_CH1, uart_buf.usart_send_len);
	DMA_EnableChannel(DMA_CH1, ENABLE);
}



/**************************************************************************************
* FunctionName   : crc16_compute(uint8_t const * p_data, uint32_t size, uint16_t const * p_crc)
* Description    : CRC校验
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
uint16_t crc16_compute(uint8_t const * p_data, uint32_t size)
{
	uint16_t crc =0xFFFF;

	for (uint32_t i = 0; i < size; i++)
	{
			crc  = (uint8_t)(crc >> 8) | (crc << 8);
			crc ^= p_data[i];
			crc ^= (uint8_t)(crc & 0xFF) >> 4;
			crc ^= (crc << 8) << 4;
			crc ^= ((crc & 0xFF) << 4) << 1;
	}

	return crc;
}


//download test peer code:
//0x000A0015AA5555AA3774
//0x000A0015A2000014B0D6
void Uart_Protocol_Process(void)
{
	uint8_t tx_buf[20];
	uint8_t *pbuf=uart_buf.receive_frame_data;
	uint8_t buf_len=uart_buf.frame_len;
	uint16_t Crc=0;
	uint32_t Length=0;
	uint16_t cmd=0;
	uint8_t *pframe_num=&(uart_buf.frame_num);
	if(*pframe_num>0)
	{
		if(pbuf[1]<2 || pbuf[1]!=buf_len)
		{
			(*pframe_num)--;
			return;
		}
		else
		{
			Length=pbuf[1];
			Crc=crc16_compute(pbuf,Length-2);
		}
		if(pbuf[Length-2]==((Crc>>8)& 0xff) && pbuf[Length-1]==(Crc & 0xff))
		{
			cmd=(pbuf[2]<<8)|pbuf[3];

		}
		else
		{
			(*pframe_num)--;
			return;
		}

		switch(cmd)
		{
			case comm_peer_code_set:
			{
				peer_code=((pbuf[4]<<24)|(pbuf[5]<<16)|(pbuf[6]<<8)|pbuf[7]);
				SavePeerCode(peer_code);
				HW2000_Init_250k();
				memcpy(tx_buf,pbuf,buf_len);
				tx_buf[0]=0x10;
				Crc=crc16_compute(tx_buf,buf_len-2);
				tx_buf[buf_len-2]=((Crc>>8)& 0xff);
				tx_buf[buf_len-1]=(Crc & 0xff);
				send_data_to_usart(tx_buf,buf_len);
			}
			break;

			case comm_peer_code_get:
			{
				peer_code=ReadPeerCode();
				tx_buf[0]=0x10;
				tx_buf[1]=0x0A;
				tx_buf[2]=0x00;
				tx_buf[3]=comm_peer_code_get;
				tx_buf[4]=((peer_code>>24)&0xFF);
				tx_buf[5]=((peer_code>>16)&0xFF);
				tx_buf[6]=((peer_code>>8)&0xFF);
				tx_buf[7]=(peer_code&0xFF);
				peer_code=ReadPeerCode();
				Crc=crc16_compute(tx_buf,8);
				tx_buf[8]=((Crc>>8)& 0xff);
				tx_buf[9]=(Crc & 0xff);
				send_data_to_usart(tx_buf,0x0A);
			}
			break;

			case comm_screem_test:
			{
				test_mode.Screem_Flag=0x01;
				test_mode.Key_Flag=0x00;
				test_mode.Screem_Count=0x00;
				memcpy(tx_buf,pbuf,buf_len);
				tx_buf[0]=0x10;
				Crc=crc16_compute(tx_buf,buf_len-2);
				tx_buf[buf_len-2]=((Crc>>8)& 0xff);
				tx_buf[buf_len-1]=(Crc & 0xff);
				send_data_to_usart(tx_buf,buf_len);
			}
			break;

			case comm_key_test:
			{
				test_mode.Screem_Flag=0x00;
				test_mode.Key_Flag=0x01;
				test_mode.Key_State=0x00;
				test_mode.Key_Value=0;
				memcpy(tx_buf,pbuf,buf_len);
				tx_buf[0]=0x10;
				Crc=crc16_compute(tx_buf,buf_len-2);
				tx_buf[buf_len-2]=((Crc>>8)& 0xff);
				tx_buf[buf_len-1]=(Crc & 0xff);
				send_data_to_usart(tx_buf,buf_len);
			}
			break;

			case comm_2_4G_LOSS_RATE:
			{
				tx_buf[0] = 0x10;
				tx_buf[1] = 0x08;
				tx_buf[2] = 0x00;
				tx_buf[3] = comm_2_4G_LOSS_RATE;
				tx_buf[4] = 0;
				tx_buf[5] = SysInfo.Loss_Rate;

				Crc = crc16_compute(tx_buf, 6);
				tx_buf[6] = ((Crc>>8)& 0xff);
				tx_buf[7] = (Crc & 0xff);
				send_data_to_usart(tx_buf, 8);
				break;
			}

			default:
			{

				break;
			}
		}
		(*pframe_num)--;
	}
}

