#include "gpio.h"

// Bit mask for enabling GPIOA (bit 0) clock
#define GPIOAEN (1U << 0)

// Bit mask of bit-set operation for setting the pin to high
#define LED_BS5 (1U << 5)  // Bit-set for Pin 5

// Bit mask of bit-reset operation for resetting the pin to low
#define LED_BR5 (1U << 21) // Bit-reset for Pin 5

// Bit mask for GPIOA Pin 5
#define PIN5 (1U << 5)

// Alias for the pin representing the LED connected to GPIO PA5
#define LED_PIN PIN5

// Bit mask for enabling GPIOC (bit 2) clock
#define GPIOCEN (1U << 2)

// Bit mask for GPIOC Pin 13
#define BTN_PIN (1U << 13)

void led_init(void) {
	// Enable the clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Configure GPIOA Pin 5 as a general-purpose output pin (MODER5[1:0] = 01)
    GPIOA->MODER |= (1U << 10);  // Set bit 10 to 1
    GPIOA->MODER &= ~(1U << 11); // Set bit 11 to 0
}

void led_on(void) {
	// Set PA5 high
    GPIOA->BSRR |= LED_BS5;
}

void led_off(void) {
	// Set PA5 low
    GPIOA->BSRR |= LED_BR5;
}

void led_toggle(void) {
	// Toggle PA5 (LED_PIN)
    GPIOA->ODR ^= LED_PIN;
}

void button_init(void) {
	// Enable the clock access to GPIOC
    RCC->AHB1ENR |= GPIOCEN;

    // Configure GPIOC Pin 13 as an input pin (MODER13[1:0] = 00)
    GPIOC->MODER &= ~(1U << 26); // Set bit 26 to 0
    GPIOC->MODER &= ~(1U << 27); // Set bit 27 to 0
}

bool get_button_state(void) {
	// Note: The button is internally connected as an active-low input.

    // Check if button is pressed or not
    if (GPIOC->IDR & BTN_PIN) {
        return false; // If the bit is set, the button is not pressed.
    } else {
        return true;  // If the bit is not set, the button is pressed.
    }
}
