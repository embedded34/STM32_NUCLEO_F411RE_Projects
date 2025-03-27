/**
 * This code demonstrates how to toggle an LED connected to GPIO pin PA5
 * on an STM32F4 microcontroller using custom-defined GPIO drives that
 * comprising of the CMSIS framework.
 */
#include "gpio.h"

/**
 * Main function: Controls the LED connected to GPIO PA5 via the user push button connected to GPIO PC13.
 */
int main(void) {
	bool btn_state; // Hold the state of the push button

    // Initialize user LED peripheral
    led_init();

    // Initialize user button peripheral
    button_init();

    while (1) {
        // Get the current state of the push button
        btn_state = get_button_state();

        if (btn_state) {
            // Turn the LED on
            led_on();
        } else {
            // Turn the LED off
            led_off();
        }
    }

    return 0;
}
