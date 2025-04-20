/**
 * This code demonstrates how to initialize UART2 for debugging,
 * configure external interrupts for a user button (PC13), and use the
 * Independent Watchdog Timer (IWDG) to monitor system activity.
 * The program toggles an LED to indicate normal operation and refreshes
 * the watchdog timer unless the user button is pressed.
 * If a reset is triggered by the IWDG, it is detected and indicated by turning on the LED.
 * This code is based on the CMSIS framework for the STM32F4 microcontroller.
 */

#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "gpio_exti.h"
#include "iwdg.h"

static void check_reset_source(void);
static void exti13_callback(void);
void EXTI15_10_IRQHandler(void);

uint8_t g_btn_press;

/**
 * Main function: Initializes UART2, user LED, external interrupt (PC13),
 * and IWDG. In the main loop, it toggles the LED and refreshes the watchdog
 * unless the button is pressed.
 */
int main(void) {
    // Initialize UART 2 peripheral for debugging
    uart2_init();

    // Initialize user LED peripheral
    led_init();

    // Initialize EXTI 13 peripheral
    pc13_exti13_init();

    // Determine if the last reset was caused by the IWDG
    check_reset_source();

    // Initialize IWDG peripheral
    iwdg_init();

    while (1) {
    	// Continually check the state of the user button
    	// If the button hasn’t been pressed, it refreshes the IWDG to prevent a system reset
    	// and toggles the LED, providing a visual indicator of system activity.
        if (g_btn_press != 1) {
            // Refresh IWDG down-counter to the default value
            IWDG->KR = IWDG_KEY_RELOAD;

            led_toggle();

            // Delay loop for visible blinking in a rapid pace
            for (volatile int i = 0; i < 90000; i++) {
            }
        }
    }

    return 0;
}

static void check_reset_source(void) {
	// If the IWDG reset flag in reset and clock control status register (RCC_CSR) is set,
	// it confirms that the watchdog initiated the reset.
    if ((RCC->CSR & RCC_CSR_IWDGRSTF) == (RCC_CSR_IWDGRSTF)) {
        // Clear IWDG reset flag for future reset detection
        RCC->CSR = RCC_CSR_RMVF;

        // Turn the LED on as a visual indication of the IWDG-triggered reset
        led_on();

        printf("The last system reset was caused by IWDG...\n\r");

        // Wait for the user to press the button
        while (g_btn_press != 1) {
        }

        // Clear the button-press state
        g_btn_press = 0;
    }
}

static void exti13_callback(void) {
    printf("An external interrupt occurred (Button is pressed)...\n\r");
    g_btn_press = 1;
}

void EXTI15_10_IRQHandler(void) {
    if ((EXTI->PR & (LINE13)) != 0) {
        // Clear PR (pending bit) flag
        EXTI->PR |= LINE13;

        // Execute the callback function to handle the external interrupt
        exti13_callback();
    }
}
