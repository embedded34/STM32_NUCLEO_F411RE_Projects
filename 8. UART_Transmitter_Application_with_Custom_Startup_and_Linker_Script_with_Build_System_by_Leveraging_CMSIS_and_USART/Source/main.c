/**
 * This code demonstrates how to initialize UART 2 peripheral for debugging
 * on an STM32F4 microcontroller using custom-defined UART 2 driver that
 * comprising of the CMSIS framework and prints a message via UART every
 * loop iteration.
 */
#include "uart.h"

/**
 * Main function: Initializes UART 2 for debugging and prints "UART Test String from STM32..." continuously.
 */
int main(void) {
    // Initialize UART 2 peripheral for debugging
    uart2_init();

    while (1) {
    	uart_write_string("UART Test String from STM32...\r\n");
    }

    return 0;
}
