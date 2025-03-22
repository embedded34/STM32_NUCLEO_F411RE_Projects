/**
 * This section of the code defines constants and macros for memory addresses and bit masks.
 * UL Suffix: It leads to more predictable, platform-independent code, ensuring that the values behave consistently across different compilers and
 * systems. 
 * The address of each register that can be read/written should be prefixed with "*(volatile unsigned int *)" or "*(volatile uint32_t *)".
 */

#include <stdint.h>

// Base address for peripherals
#define PERIPH_BASE (0x40000000UL)

// Offset for AHB1 peripheral bus
#define AHB1PERIPH_OFFSET (0x00020000UL)

// Base address for AHB1 peripherals
#define AHB1PERIPH_BASE (PERIPH_BASE + AHB1PERIPH_OFFSET)

// Offset for GPIOA
#define GPIOA_OFFSET (0x0000UL)

// Base address for GPIOA
#define GPIOA_BASE (AHB1PERIPH_BASE + GPIOA_OFFSET)

// Offset for RCC (Reset and Clock Control)
#define RCC_OFFSET (0x3800UL)

// Base address for RCC
#define RCC_BASE (AHB1PERIPH_BASE + RCC_OFFSET)

// Offset for AHB1EN register (Peripheral clock enable)
#define AHB1ENR_OFFSET (0x30UL)

// Address of AHB1EN peripheral clock enable register
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + AHB1ENR_OFFSET))

// Offset for mode register (MODER) for GPIO pins
#define MODER_OFFSET (0x00UL)

// Address of GPIOA mode register
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + MODER_OFFSET))

// Offset for output data register (ODR)
#define ODR_OFFSET (0x14UL)

// Address of GPIOA output data register
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE + ODR_OFFSET))

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
    RCC_AHB1ENR |= GPIOAEN;

    // Configure GPIOA Pin 5 as a general-purpose output pin (MODER5[1:0] = 01)
    GPIOA_MODER |= (1U << 10);   // Set bit 10 to 1
    GPIOA_MODER &= ~(1U << 11);  // Set bit 11 to 0

    while (1) {
        // Toggle PA5 (LED_PIN)
        GPIOA_ODR ^= LED_PIN;

        // Delay loop for visible blinking in a rapid pace
        for (volatile int i = 0; i < 100000; i++) {
        }
    }

    return 0;
}
