#ifndef INCLUDE_TIM_H_
#define INCLUDE_TIM_H_

#include "stm32f4xx.h"

// Update interrupt flag bit
#define SR_UIF (1U << 0)

/* Function Declarations */
void tim2_1hz_signal_init(void);

#endif /* INCLUDE_TIM_H_ */
