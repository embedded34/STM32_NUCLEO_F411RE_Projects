#include "standby_mode.h"

// Macro to enter Standby mode when the CPU enters deepsleep
#define PWR_MODE_STANDBY (PWR_CR_PDDS)

// Bit mask for wake-up pin (GPIOA Pin 0)
#define WK_PIN (1U << 0)

static void set_low_power_mode(uint32_t pwr_mode);

void pa0_wakeup_pin_init(void) {
	// Enable the clock access to GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure GPIOA Pin 0 as an input pin (MODER0[1:0] = 00)
    GPIOA->MODER &= ~(1U << 0);
    GPIOA->MODER &= ~(1U << 1);

    // Configure GPIOA Pin 0 with no pull-up or pull-down resistors (PUPDR0[1:0] = 00)
    GPIOA->PUPDR &= ~(1U << 0);
    GPIOA->PUPDR &= ~(1U << 1);

    // PA0 is configured as the wake-up pin and is active low.
}

void standby_pa0_wakeup_pin_setup(void) {
	// Wait for the wake-up pin to be released, ensuring that the pin is in a stable state before proceeding
    while (get_pa0_wakeup_pin_state() == 0) {
    }

    // Disable the wake-up pin to clear any residual settings
    // WKUP pin is used for general purpose I/O.
    // An event on the WKUP pin does not wake up the device from Standby mode.
    PWR->CSR &= ~(1U << 8);

    // Clear all wake-up flags to reset the wake-up status
    PWR->CR |= (1U << 2);

    // Enable the wake-up pin
    // WKUP pin is used for wake up from Standby mode.
    PWR->CSR |= (1U << 8);

    // Set the low-power mode to Standby mode
    set_low_power_mode(PWR_MODE_STANDBY);

    // Set the SLEEPDEEP bit in the Cortex-M4 System Control Register to enable deep sleep mode
    SCB->SCR |= (1U << 2);

    // Execute the WFI (wait for interrupt) instruction
    __WFI();
}

uint32_t get_pa0_wakeup_pin_state(void) {
	// Return the current state of the wake-up pin PA0
	return ((GPIOA->IDR & WK_PIN) == WK_PIN);
}

static void set_low_power_mode(uint32_t pwr_mode) {
	// Update the PWR_CR register, specifically targeting the bits related to different power modes
	// such as PDDS (Power Down Deepsleep), LPDS (Low-Power	Deepsleep), FPDS (Flash Power Down in Stop Mode),
	// LPLVDS (Low-Power Regulator in Low Voltage in Deepsleep), and MRLVDS (Main Regulator in Low Voltage in Deepsleep)
    MODIFY_REG(PWR->CR, (PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_FPDS | PWR_CR_LPLVDS | PWR_CR_MRLVDS), pwr_mode);
}
