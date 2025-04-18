#ifndef INCLUDE_ADC_H_
#define INCLUDE_ADC_H_

#include <stdint.h>
#include "stm32f4xx.h"

/* Function Declarations */
void pa1_adc1_init(void);
void start_adc1_conversion(void);
uint32_t adc1_read(void);

#endif /* INCLUDE_ADC_H_ */
