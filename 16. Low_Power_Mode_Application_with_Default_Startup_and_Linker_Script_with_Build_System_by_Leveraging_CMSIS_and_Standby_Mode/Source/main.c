/**
 * This code demonstrates how to configure the STM32F411RE microcontroller
 * to wake up from Standby mode using an external interrupt on PA0 (wake-up pin)
 * and a user button connected to PC13 (EXTI13).
 *
 * Features:
 * - Initializes UART2 for debugging output.
 * - Configures PA0 as a wake-up pin.
 * - Checks whether the system resumed from Standby mode and handles reset flags.
 * - Sets up EXTI13 to trigger standby entry when the button is pressed.
 *
 * The main loop remains idle. When the user button is pressed, the MCU enters
 * Standby mode and can be woken up by toggling PA0.
 * In normal mode, connect a jumper wire from PA0 to the ground. To trigger a wake-up event,
 * pull out the jumper wire and connect it to 3.3V, causing a change in logic that will wake the
 * microcontroller from standby mode.
 */

#include <stdio.h>
#include "uart.h"
#include "gpio_exti.h"
#include "standby_mode.h"

static void check_reset_source(void);
static void exti13_callback(void);
void EXTI15_10_IRQHandler(void);

/**
 * Main function: Initializes UART2, configures PA0 as a wake-up pin,
 * checks the reset source, and sets up the external interrupt on PC13.
 * The main loop remains idle.
 */
int main(void) {
	// Initialize UART 2 peripheral for debugging
	uart2_init();

	// Configure the wake-up pin to prepare the microcontroller to respond to external wake-up signals
	pa0_wakeup_pin_init();

    // Determine if the last reset was caused by the standby mode or another source
    check_reset_source();

    // Initialize EXTI 13 peripheral
    pc13_exti13_init();

    while (1) {
    }
}

static void check_reset_source(void) {
	// Enable the clock access to PWR (power controller peripheral)
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // Check whether the standby flag (SBF) is set
    if ((PWR->CSR & PWR_CSR_SBF) == (PWR_CSR_SBF)) {
        // Clear standby flag
        PWR->CR |= PWR_CR_CSBF;

        printf("The system has resumed from Standby...\n\r");

    	// Wait for the wake-up pin to be released, ensuring that the pin is in a stable state before proceeding
        while (get_pa0_wakeup_pin_state() == 0) {
        }
    }

    // Check whether the wake-up flag (WUF) is set
    if ((PWR->CSR & PWR_CSR_WUF) == PWR_CSR_WUF) {
    	// Clears the flag to reset the wake-up status
        PWR->CR |= PWR_CR_CWUF;
    }
}

static void exti13_callback(void) {
	standby_pa0_wakeup_pin_setup();
}

void EXTI15_10_IRQHandler(void) {
    if ((EXTI->PR & (LINE13)) != 0) {
        // Clear PR (pending bit) flag
        EXTI->PR |= LINE13;

        // Execute the callback function to handle the external interrupt
        exti13_callback();
    }
}
