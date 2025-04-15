#include "spi.h"

// Macro to enable the clock for GPIOA (bit 0 in RCC_AHB1ENR)
#define GPIOAEN (1U << 0)

// Macro to enable the clock for SPI1 (bit 12 in RCC_APB2ENR)
#define SPI1EN (1U << 12)

// Macro to represent the TXE (transmit buffer empty) bit (bit 1 in SPI_SR)
#define SR_TXE (1U << 1)

// Macro to represent the RXNE (receive buffer not empty) bit (bit 0 in SPI_SR)
#define SR_RXNE (1U << 0)

// Macro to represent the BSY (busy flag) bit (bit 7 in SPI_SR)
#define SR_BSY (1U << 7)

void spi1_gpioa_init(void) {
	// Enable the clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Configure GPIOA Pin 5, 6, 7 to operate in alternate function mode (MODERx[1:0] = 10)
    // PA5
    GPIOA->MODER &= ~(1U << 10);  // Set bit 10 to 0
    GPIOA->MODER |= (1U << 11);   // Set bit 11 to 1
    // PA6
    GPIOA->MODER &= ~(1U << 12);  // Set bit 12 to 0
    GPIOA->MODER |= (1U << 13);   // Set bit 13 to 1
    // PA7
    GPIOA->MODER &= ~(1U << 14);  // Set bit 14 to 0
    GPIOA->MODER |= (1U << 15);   // Set bit 15 to 1

    // Configure GPIOA Pin 9 as a general-purpose output pin (MODER9[1:0] = 01), which will be used for SS
    GPIOA->MODER |= (1U << 18);   // Set bit 18 to 1
    GPIOA->MODER &= ~(1U << 19);  // Set bit 19 to 0

    // Set alternate function type to AF5
    // PA5 (SPI1_SCK)
    GPIOA->AFR[0] |= (1U << 20);
    GPIOA->AFR[0] &= ~(1U << 21);
    GPIOA->AFR[0] |= (1U << 22);
    GPIOA->AFR[0] &= ~(1U << 23);
    // PA6 (SPI1_MISO)
    GPIOA->AFR[0] |= (1U << 24);
    GPIOA->AFR[0] &= ~(1U << 25);
    GPIOA->AFR[0] |= (1U << 26);
    GPIOA->AFR[0] &= ~(1U << 27);
    // PA7 (SPI1_MOSI)
    GPIOA->AFR[0] |= (1U << 28);
    GPIOA->AFR[0] &= ~(1U << 29);
    GPIOA->AFR[0] |= (1U << 30);
    GPIOA->AFR[0] &= ~(1U << 31);
}

void spi1_config(void) {
	// Enable the clock access to SPI1
    RCC->APB2ENR |= SPI1EN;

    // Configure the SPI clock prescaler to set the baud rate by dividing the APB2 peripheral clock by 4 (fPCLK/4)
    SPI1->CR1 |= (1U << 3);
    SPI1->CR1 &= ~(1U << 4);
    SPI1->CR1 &= ~(1U << 5);

    // Set CPOL to 1 and CPHA to 1 (Mode 3))
    SPI1->CR1 |= (1U << 0);  // Clock phase
    SPI1->CR1 |= (1U << 1);  // Clock polarity

    // Enable full-duplex mode for simultaneous data transmit and receive
    SPI1->CR1 &= ~(1U << 10);

    // Transmit the MSB first
    SPI1->CR1 &= ~(1U << 7);

    // Set SPI1 to master mode, making it the controller of the SPI bus
    SPI1->CR1 |= (1U << 2);

    // Configure the SPI1 data frame size to 8 bits
    SPI1->CR1 &= ~(1U << 11);

    // Enable software slave management of the SS line by setting SSM=1 and SSI=1
    SPI1->CR1 |= (1U << 8);
    SPI1->CR1 |= (1U << 9);

    // Enable SPI1 module
    SPI1->CR1 |= (1U << 6);
}

void spi1_transmit(uint8_t *data, uint32_t size) {
    uint32_t i = 0;
    uint8_t temp;

    while (i < size) {
    	// Wait until the transmit buffer is empty before sending the next byte
        while (!(SPI1->SR & (SR_TXE))) {
        }

        // Write the current byte of data to the data register
        SPI1->DR = data[i];
        i++;
    }

    // Wait until the transmit buffer is empty before sending the next byte
    while (!(SPI1->SR & (SR_TXE))) {
    }

    // Ensure the SPI1 bus is not busy before continuing
    while ((SPI1->SR & (SR_BSY))) {
    }

    // Clear OVR (overrun) flag automatically by reading the data register
    temp = SPI1->DR;
    temp = SPI1->SR;
    (void)temp;
}

void spi1_receive(uint8_t *data, uint32_t size) {
    while (size) {
        // Send a dummy data to generate clock pulses
        SPI1->DR = 0;

        // Wait until data is received
        while (!(SPI1->SR & (SR_RXNE))) {
        }

        // Read the received data from data register
        *data++ = (SPI1->DR);
        size--;
    }
}

void spi1_cs_enable(void) {
	// Pull the SS line low to enable the slave device
	GPIOA->ODR &= ~(1U << 9);
}

void spi1_cs_disable(void) {
	// Pull the SS line high to disable the slave device
	GPIOA->ODR |= (1U << 9);
}
