#ifndef INCLUDE_I2C_H_
#define INCLUDE_I2C_H_

#include <stdint.h>
#include "stm32f4xx.h"

/* Function Declarations */
void i2c1_gpiob_init(void);
void i2c1_single_byte_read(uint8_t saddr, uint8_t maddr, uint8_t *data);
void i2c1_multiple_bytes_read(uint8_t saddr, uint8_t maddr, uint32_t n, uint8_t *data);
void i2c1_multiple_bytes_write(uint8_t saddr, uint8_t maddr, uint32_t n, uint8_t *data);

#endif /* INCLUDE_I2C_H_ */
