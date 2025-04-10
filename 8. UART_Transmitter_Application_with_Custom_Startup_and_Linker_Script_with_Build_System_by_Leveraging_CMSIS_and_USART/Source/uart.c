#include <stdint.h>
#include "uart.h"

// Macro to enable the clock for GPIOA (bit 0 in RCC_AHB1ENR)
#define GPIOAEN (1U << 0)

// Macro to enable the clock for UART2 (bit 17 in RCC_APB1ENR)
#define UART2EN (1U << 17)

// Macro to define the baud rate for UART communication in bps
#define DBG_UART_BAUDRATE 115200

// Macro to define the system frequency which is 16 MHz (default)
#define SYS_FREQ 16000000

// Macro to set the APB1 peripheral clock frequency to the system frequency
#define APB1_CLK SYS_FREQ

// Macro to enable the transmitter (bit 3 in USART_CR1)
#define CR1_TE (1U << 3)

// Macro to enable the UART module (bit 13 in USART_CR1)
#define CR1_UE (1U << 13)

// Macro to represent the TXE (transmit data register empty) bit (bit 7 in USART_SR)
#define SR_TXE (1U << 7)

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart2_set_baudrate(uint32_t periph_clk, uint32_t baudrate);

void uart2_init(void) {
    // Enable the clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Configure GPIOA Pin 2 to operate in alternate function mode (MODER2[1:0] = 10)
    GPIOA->MODER &= ~(1U << 4);  // Set bit 4 to 0
    GPIOA->MODER |= (1U << 5);   // Set bit 5 to 1

    // Set alternate function type to AF7 (UART2_TX)
    GPIOA->AFR[0] |= (1U << 8);    // Set bit 8 to 1
    GPIOA->AFR[0] |= (1U << 9);    // Set bit 9 to 1
    GPIOA->AFR[0] |= (1U << 10);   // Set bit 10 to 1
    GPIOA->AFR[0] &= ~(1U << 11);  // Set bit 11 to 0

    // Enable the clock access to UART2
    RCC->APB1ENR |= UART2EN;

    // Set UART2 baud rate
    uart2_set_baudrate(APB1_CLK, DBG_UART_BAUDRATE);

    // Configure transfer direction as a transmitter
    USART2->CR1 |= CR1_TE;

    // Enable UART2 module
    USART2->CR1 |= CR1_UE;
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate) {
    return ((periph_clk + (baudrate / 2U)) / baudrate);
}

static void uart2_set_baudrate(uint32_t periph_clk, uint32_t baudrate) {
    // Set the baud rate for UART2 by writing the computed divisor to the BRR
    USART2->BRR = compute_uart_bd(periph_clk, baudrate);
}

void uart_write_character(int ch) {
    // Ensure that the transmit data register is empty before writing a new data
    while (!(USART2->SR & SR_TXE)) {
    }

    // Write the character to the data register for transmission
    // This is correct for 8-bit data frame transmission.
    USART2->DR = (ch & 0xFF);
}

void uart_write_string(const char *str) {
	// Loop through each character until the null terminator is reached
    while (*str) {
    	// Send the current character over UART
    	uart_write_character(*str++);
    }
}
