/**
 * This code demonstrates how to initialize UART2 for debugging,
 * configure an LED output, and handle an external interrupt on pin PC13
 * using the CMSIS framework on an STM32F4 microcontroller.
 */
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "gpio_exti.h"

static void exti13_callback(void);
void EXTI15_10_IRQHandler(void);

/**
 * Main function: Initializes UART2, user LED, and EXTI13 (PC13 interrupt).
 */
int main(void) {
    // Initialize UART 2 peripheral for debugging
    uart2_init();

    // Initialize user LED peripheral
    led_init();

    // Initialize EXTI 13 peripheral
    pc13_exti13_init();

    while (1) {
    }

    return 0;
}

static void exti13_callback(void) {
    printf("An external interrupt occurred (Button is pressed)...\n\r");
    led_toggle();
}

void EXTI15_10_IRQHandler(void) {
    if ((EXTI->PR & (LINE13)) != 0) {
        // Clear PR (pending bit) flag
        EXTI->PR |= LINE13;

        // Execute the callback function to handle the external interrupt
        exti13_callback();
    }
}
