#include "tim.h"

// Macro to enable the clock for TIM2
#define TIM2_ENABLE (1U << 0)

// Macro to enable the counter in the TIM2 control register 1 (TIM2_CR1)
#define CR1_CEN (1U << 0)

void tim2_1hz_signal_init(void) {
    // Enable clock access to TIM2
    RCC->APB1ENR |= TIM2_ENABLE;

    // Set prescaler value to divide the input clock
    // By default, the input clock frequency of is 16 MHz.
    // 16 MHz / 1600 = 10000 cycles per second (10 kHz counter clock frequency)
    TIM2->PSC = 1600 - 1;

    // Set auto-reload value to make the timer count up to a specified value
    // Timer will count from 0 to 9999, creating a period of 10000 ticks.
    // Since timer clock is 10 kHz, counting 10000 ticks results in 1 second.
    // 10000 / 10 kHz = 1 s
    TIM2->ARR = 10000 - 1;

    // Clear timer counter to reset the timer counter to 0
    TIM2->CNT = 0;

    // Ensure the timer update interrupt flag is cleared before enabling the counter
    TIM2->SR &= ~SR_UIF;

    // Enable timer and make it start counting
    TIM2->CR1 |= CR1_CEN;
}
