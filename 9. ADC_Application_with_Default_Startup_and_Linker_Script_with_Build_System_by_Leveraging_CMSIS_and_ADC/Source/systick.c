#include "systick.h"

// Macro to enable the SysTick timer
#define CTRL_ENABLE (1U << 0)

// Macro to select the internal clock source for the SysTick timer
#define CTRL_CLKSOURCE (1U << 2)

// Macro to check the COUNTFLAG bit, which indicates when the timer has counted to zero
#define CTRL_COUNTFLAG (1U << 16)

// Macro to define the number of clock cycles in 1 millisecond
// By default, the frequency of the MCU is 16 MHz.
// 16 MHz / 1000 = 16000 cycles per millisecond
#define CLK_CYCLES_IN_ONE_MSEC (16000U)

void systick_msec_delay(uint32_t delay) {
    // Load the SysTick timer with the number of clock cycles for 1 millisecond
    SysTick->LOAD = CLK_CYCLES_IN_ONE_MSEC - 1;

    // Clear SysTick current value register to reset the timer
    SysTick->VAL = 0;

    // Select internal clock source
    SysTick->CTRL |= CTRL_CLKSOURCE;

    // Enable SysTick timer
    SysTick->CTRL |= CTRL_ENABLE;

    // Handle the specified delay duration
    for (volatile uint32_t i = 0; i < delay; i++) {
        // Wait for the COUNTFLAG bit to be set, indicating the timer has counted down
        while ((SysTick->CTRL & CTRL_COUNTFLAG) == 0) {
        }
    }

    // Disable SysTick timer
    SysTick->CTRL &= ~CTRL_ENABLE;
}
