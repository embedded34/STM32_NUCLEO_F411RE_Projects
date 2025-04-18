#include "gpio_exti.h"

// Macro to enable the clock for GPIOC (bit 2 in RCC_AHB1ENR)
#define GPIOCEN (1U << 2)

// Macro to enable the clock for SYSCFG (bit 14 in RCC_APB2ENR))
#define SYSCFGEN (1U << 14)

void pc13_exti13_init(void) {
	// Disable global interrupts to ensure that the configuration process is not
	// interrupted, which is crucial for maintaining consistency and avoiding race
	// conditions
	__disable_irq();

	// Enable the clock access to GPIOC
    RCC->AHB1ENR |= GPIOCEN;

    // Configure GPIOC Pin 13 to operate in input mode (MODER13[1:0] = 00)
    GPIOC->MODER &= ~(1U << 26);  // Set bit 26 to 0
    GPIOC->MODER &= ~(1U << 27);  // Set bit 27 to 0

    // Enable the clock access to SYSCFG (system configuration controller)
    RCC->APB2ENR |= SYSCFGEN;

    // Configure the SYSCFG external interrupt configuration register 4 to map
    // EXTI line 13 to Port C (GPIOC Pin 13 will be selected the source input for
    // the EXTI13 external interrupt.)
    SYSCFG->EXTICR[3] |= (1U << 5);

    // Unmask EXTI13 line to allow the line to generate interrupt requests
    EXTI->IMR |= (1U << 13);

    // Enable falling-edge trigger for input line to detect the signal transitions from high to low
    EXTI->FTSR |= (1U << 13);
    EXTI->RTSR &= ~(1U << 13);

    // Enable the EXTI15_10 interrupt line in the NVIC
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    // Enable global interrupts to allow the microcontroller to respond to interrupts
    __enable_irq();
}
