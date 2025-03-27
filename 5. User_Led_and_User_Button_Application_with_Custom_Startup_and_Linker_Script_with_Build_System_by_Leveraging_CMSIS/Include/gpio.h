#ifndef GPIO_H_
#define GPIO_H_

#include <stdbool.h>
#include "stm32f4xx.h"

/* Function Declarations */
void led_init(void);
void led_on(void);
void led_off(void);
void button_init(void);
bool get_button_state(void);

#endif /* GPIO_H_ */
