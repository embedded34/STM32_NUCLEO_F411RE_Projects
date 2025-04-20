#ifndef INCLUDE_IWDG_H_
#define INCLUDE_IWDG_H_

#include "stm32f4xx.h"

// Macro to reload the counter so that the watchdog timer system reset is prevented
#define IWDG_KEY_RELOAD (0x0000AAAAU)

/* Function Declarations */
void iwdg_init(void);

#endif /* INCLUDE_IWDG_H_ */
