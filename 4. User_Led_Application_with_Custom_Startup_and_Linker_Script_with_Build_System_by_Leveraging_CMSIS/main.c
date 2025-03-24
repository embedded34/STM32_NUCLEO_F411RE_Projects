/**
 * This code demonstrates how to toggle an LED connected to GPIO pin PA5
 * on an STM32F4 microcontroller using CMSIS framework.
 */
#include "stm32f4xx.h"

// Bit mask for enabling GPIOA (bit 0) clock
#define GPIOAEN (1U << 0)

// Bit mask for GPIOA Pin 5
#define PIN5 (1U << 5)

// Alias for the pin representing the LED connected to GPIO PA5
#define LED_PIN PIN5

/**
 * Main function: Controls the LED connected to GPIO PA5.
 */
 int main(void) {
    // Enable the clock access to GPIOA
	 RCC->AHB1ENR |= GPIOAEN;

    // Configure GPIOA Pin 5 as a general-purpose output pin (MODER5[1:0] = 01)
	GPIOA->MODER |= (1U << 10);   // Set bit 10 to 1
	GPIOA->MODER &= ~(1U << 11);  // Set bit 11 to 0

    while (1) {
        // Toggle PA5 (LED_PIN)
    	GPIOA->ODR ^= LED_PIN;

        // Delay loop for visible blinking in a rapid pace
        for (volatile int i = 0; i < 100000; i++) {
        }
    }

    return 0;
}
