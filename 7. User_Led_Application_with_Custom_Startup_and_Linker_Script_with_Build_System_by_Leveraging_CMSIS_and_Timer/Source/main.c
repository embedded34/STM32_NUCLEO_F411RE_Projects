/**
 * This code demonstrates how to toggle an LED connected to GPIO pin PA5
 * on an STM32F4 microcontroller using custom-defined GPIO and Timer 2
 * drivers that comprising of the CMSIS framework.
 */
#include "gpio.h"
#include "tim.h"

/**
 * Main function: Toggles the LED connected to GPIO PA5 at a 1 Hz frequency via the Timer 2 peripheral, i.e. toggling every second.
 */
int main(void) {
    // Initialize user LED peripheral
    led_init();

    // Initialize Timer 2 peripheral
    tim2_1hz_signal_init();

    while (1) {
    	// Toggle the user LED
    	led_toggle();

    	// Wait for UIF
    	while(!(TIM2->SR & SR_UIF)){
    	}

    	// Clear UIF
    	TIM2->SR &= ~SR_UIF;
    }

    return 0;
}
