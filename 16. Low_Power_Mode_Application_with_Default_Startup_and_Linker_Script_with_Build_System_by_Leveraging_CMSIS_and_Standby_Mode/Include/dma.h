#ifndef INCLUDE_DMA_H_
#define INCLUDE_DMA_H_

#include <stdint.h>
#include "stm32f4xx.h"

// Macro to check whether a transmission complete event is occurred on Stream 0 (bit 5 in DMA_LISR)
#define LISR_TCIF0 (1U << 5)

// Macro to clear transfer complete interrupt flag for Stream 0 (bit 5 in DMA_LIFCR)
#define LIFCR_CTCIF0 (1U << 5)

// Macro to check whether a transmission error is occurred on Stream 0 (bit 3 in DMA_LISR)
#define LISR_TEIF0 (1U << 3)

// Macro to clear transfer error interrupt flag for Stream 0 (bit 3 in DMA_LIFCR)
#define LIFCR_CTEIF0 (1U << 3)

/* Function Declarations */
void dma2_stream0_mem2mem_config(void);
void dma2_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len);

#endif /* INCLUDE_DMA_H_ */
