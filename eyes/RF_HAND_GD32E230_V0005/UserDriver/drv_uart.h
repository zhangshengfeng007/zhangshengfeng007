#ifndef __DRV_UART_H
#define __DRV_UART_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/
#define USART0_RDATA_ADDRESS      ((uint32_t)&USART_RDATA(USART0))
#define UART_MAX_BUFFER	20

/* Exported functions ------------------------------------------------------- */

extern u8 Uart_RX_Buffer[UART_MAX_BUFFER];
extern u8 Uart_RX_Len;
extern Func_Type Uart_RX_Finish;

void Drv_Uart_Init(void);
void Drv_Uart_Send(uint8_t *data, uint8_t size);

#endif

