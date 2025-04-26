#ifndef INCLUDE_ADC_DMA_H_
#define INCLUDE_ADC_DMA_H_

#include "stm32f4xx.h"

// Macro to define the number of channels for the DMA2
#define NUM_OF_CHANNELS 2

/* Function Declarations */
void adc1_dma2_init(void);

#endif /* INCLUDE_ADC_DMA_H_ */
