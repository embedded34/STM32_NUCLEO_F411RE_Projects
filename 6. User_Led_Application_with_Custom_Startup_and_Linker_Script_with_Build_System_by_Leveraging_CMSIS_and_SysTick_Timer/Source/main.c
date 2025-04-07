/**
 * This code demonstrates how to toggle an LED connected to GPIO pin PA5
 * on an STM32F4 microcontroller using custom-defined GPIO and SysTick timer
 * drivers that comprising of the CMSIS framework.
 */
#include "gpio.h"
#include "systick.h"

/**
 * Main function: Toggles the LED connected to GPIO PA5 at a 500 ms interval via the SysTick timer.
 */
int main(void) {
    // Initialize user LED peripheral
    led_init();

    while (1) {
    	// Delay for 500 ms
    	systick_msec_delay(500);

    	// Toggle the user LED
    	led_toggle();
    }

    return 0;
}
