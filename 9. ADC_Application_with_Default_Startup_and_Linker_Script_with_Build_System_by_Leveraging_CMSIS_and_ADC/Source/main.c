/**
 * This code demonstrates how to initialize the UART 2 peripheral for debugging
 * on an STM32F4 microcontroller using a custom UART 2 driver built on top of
 * the CMSIS framework. It continuously reads a sensor value via ADC 1 and prints
 * the result over UART in each loop iteration.
 */
#include "adc.h"
#include "uart.h"

// A global variable that stores the ADC value that’s read from the sensor
int sensor_value;

/**
 * Main function: Initializes UART2 for debugging and continuously prints "Sensor Value: <value>" over UART.
 */
int main(void) {
    // Initialize UART 2 peripheral for debugging
    uart2_init();

    // Initialize ADC 1
    pa1_adc1_init();

    // Start analog to digital conversion process
    start_adc1_conversion();

    while (1) {
    	// Read and store the latest converted value from the ADC 1
    	sensor_value = adc1_read();

    	// Print the sensor value to the terminal or console via the UART
    	uart_write_formatted_string("Sensor Value: %d\r\n", sensor_value);
    }

    return 0;
}
