#ifndef INCLUDE_STANDBY_MODE_H_
#define INCLUDE_STANDBY_MODE_H_

#include <stdint.h>
#include "stm32f4xx.h"

/* Function Declarations */
void pa0_wakeup_pin_init(void);
void standby_pa0_wakeup_pin_setup(void);
uint32_t get_pa0_wakeup_pin_state(void);

#endif /* INCLUDE_STANDBY_MODE_H_ */
