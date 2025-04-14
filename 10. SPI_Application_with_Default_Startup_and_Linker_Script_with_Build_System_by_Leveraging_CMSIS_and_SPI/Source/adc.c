#include "adc.h"

// Macro to enable the clock for GPIOA (bit 0 in RCC_AHB1ENR)
#define GPIOAEN (1U << 0)

// Macro to enable the clock for ADC1 (bit 8 in RCC_APB2ENR)
#define ADC1EN (1U << 8)

// Macro to select channel 1 for the ADC conversion in the ADC_SQR3 register
#define ADC_CH1 (1U << 0)

// Macro to set the conversion sequence length to 1 in the ADC_SQR1 register
#define ADC_SEQ_LEN_1 0x00

// Macro to enable the ADC module (bit 0 in ADC_CR2)
#define CR2_ADCON (1U << 0)

// Macro to enable continuous conversion mode (bit 1 in ADC_CR2)
#define CR2_CONT (1U << 1)

// Macro to start the ADC conversion of regular channels (bit 30 in ADC_CR2)
#define CR2_SWSTART (1U << 30)

// Macro to represent the regular channel end of conversion flag (bit 1 in ADC_SR)
#define SR_EOC (1U << 1)

void pa1_adc1_init(void) {
	// Enable the clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Configure GPIOA Pin 1 to operate in analog input mode (MODER1[1:0] = 11)
    GPIOA->MODER |= (1U << 2);  // Set bit 2 to 1
    GPIOA->MODER |= (1U << 3);  // Set bit 3 to 1

    // Enable clock access to the ADC1 module
    RCC->APB2ENR |= ADC1EN;

    // Set channel 1 as the start of the conversion sequence
    ADC1->SQR3 = ADC_CH1;

    // Set conversion sequence length to 1, meaning only one channel will be converted
    ADC1->SQR1 = ADC_SEQ_LEN_1;

    // Enable ADC1 module
    ADC1->CR2 |= CR2_ADCON;
}

void start_adc1_conversion(void) {
    // Enable continuous conversion mode
    ADC1->CR2 |= CR2_CONT;

    // Start ADC1 conversion
    ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc1_read(void) {
	// Wait until the conversion is complete
    while (!(ADC1->SR & SR_EOC)) {
    }

    // Read the converted digital value
    return (ADC1->DR);
}
