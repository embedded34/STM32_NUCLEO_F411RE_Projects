#ifndef INCLUDE_UART_DMA_H_
#define INCLUDE_UART_DMA_H_

#include <stdint.h>
#include "stm32f4xx.h"

#define UART2_DATA_BUFF_SIZE 6

/* Function Declarations */
void uart2_rx_tx_init(void);
void dma1_init(void);
void dma1_stream5_uart2_rx_config(void);
void dma1_stream6_uart2_tx_config(uint32_t msg_to_snd, uint32_t msg_len);

#endif /* INCLUDE_UART_DMA_H_ */
