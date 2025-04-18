#ifndef INCLUDE_SPI_H_
#define INCLUDE_SPI_H_

#include <stdint.h>
#include "stm32f4xx.h"

/* Function Declarations */
void spi1_gpioa_init(void);
void spi1_config(void);
void spi1_transmit(uint8_t *data, uint32_t size);
void spi1_receive(uint8_t *data, uint32_t size);
void spi1_cs_enable(void);
void spi1_cs_disable(void);

#endif /* INCLUDE_SPI_H_ */
