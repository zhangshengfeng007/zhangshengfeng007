/***********************************************************
* @file    drv_uart.c
* @brief  uart程序
* @version V1.0
* @date    2023.01.29
***********************************************************/
#include "drv_uart.h"

static void Drv_Uart_NVIC_Config(void);
static void Drv_Uart_DMA_Config(void);
static void Drv_Uart_GPIO_Config(void);

u8 Uart_RX_Buffer[UART_MAX_BUFFER] = {0};
u8 Uart_RX_Len = 0;
Func_Type Uart_RX_Finish = Func_DISABLE;

/*
*************************************************************
* 功能说明: 串口初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Drv_Uart_Init(void)
{
	Drv_Uart_NVIC_Config();
	Drv_Uart_DMA_Config();
	Drv_Uart_GPIO_Config();

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART0);
	
	/* USART configure */
	usart_deinit(USART0);
	usart_baudrate_set(USART0, 115200U);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_dma_receive_config(USART0, USART_DENR_ENABLE);
	usart_dma_transmit_config(USART0, USART_DENT_ENABLE);
	usart_enable(USART0);

	/* wait IDLEF set and clear it */
    while(RESET == usart_flag_get(USART0, USART_FLAG_IDLE));
    usart_flag_clear(USART0, USART_FLAG_IDLE);
    usart_interrupt_enable(USART0,USART_INT_IDLE);
}

/*
*************************************************************
* 功能说明: 串口NVIC配置
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
static void Drv_Uart_NVIC_Config(void)
{
	nvic_irq_enable(USART0_IRQn, 0);
}

static void Drv_Uart_DMA_Config(void)
{
	dma_parameter_struct dma_init_struct;

	rcu_periph_clock_enable(RCU_DMA);

	/* deinitialize DMA channel2 (USART0 rx) */
	dma_deinit(DMA_CH2);
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_addr = (uint32_t)Uart_RX_Buffer;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.number = UART_MAX_BUFFER;
	dma_init_struct.periph_addr = USART0_RDATA_ADDRESS;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA_CH2, &dma_init_struct);
	/* configure DMA mode */
	dma_circulation_disable(DMA_CH2);	//关闭循环
	/* enable DMA channel2 */
	dma_channel_enable(DMA_CH2);
}

/*
*************************************************************
* 功能说明: 串口GPIO初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
static void Drv_Uart_GPIO_Config(void)
{
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOB);
	
	/* connect port to USARTx_Tx */
	gpio_af_set(TX0_PORT, GPIO_AF_0, TX0_PIN);
	/* connect port to USARTx_Rx */
	gpio_af_set(RX0_PORT, GPIO_AF_0, RX0_PIN);

	/* configure USART Tx as alternate function push-pull */
	gpio_mode_set(TX0_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, TX0_PIN);
	gpio_output_options_set(TX0_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, TX0_PIN);

	/* configure USART Rx as alternate function push-pull */
	gpio_mode_set(RX0_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, RX0_PIN);
	gpio_output_options_set(RX0_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, RX0_PIN);
}

/*
*************************************************************
* 功能说明: 串口数据发送
* 形    参: data发送的数据，size数据长度
* 返 回 值: 无
*************************************************************
*/
void Drv_Uart_Send(uint8_t *data, uint8_t size)
{
	uint16_t i = 0;
	uint16_t j = 0;
	
	for(i = 0;i < size;i++)
	{
		j = 0;
		while(usart_flag_get(USART0, USART_FLAG_TBE) == RESET)
		{
			j++;
			if(j > 5000){
				break;
			}
		}
		/* Write one byte to the transmit data register */       
		usart_data_transmit(USART0, data[i]);
	}
	
}

/*!
    \brief      this function handles USART interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
	if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)){
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);

		/* number of data received */
		Uart_RX_Len = UART_MAX_BUFFER - (dma_transfer_number_get(DMA_CH2));
		Uart_RX_Finish = Func_FINISH;

		/* disable DMA and reconfigure */
		dma_channel_disable(DMA_CH2);
		dma_transfer_number_config(DMA_CH2, UART_MAX_BUFFER);
		dma_channel_enable(DMA_CH2);
	}
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}
