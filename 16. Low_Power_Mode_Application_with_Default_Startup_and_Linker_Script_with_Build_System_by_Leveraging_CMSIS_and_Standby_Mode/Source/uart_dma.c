#include "uart_dma.h"

// Macro to enable the clock for GPIOA (bit 0 in RCC_AHB1ENR)
#define GPIOAEN (1U << 0)

// Macro to enable the clock for UART2 (bit 17 in RCC_APB1ENR)
#define UART2EN (1U << 17)

// Macro to define the baud rate for UART communication in bps
#define UART_BAUDRATE 115200

// Macro to define the system clock frequency which is 16 MHz (default)
#define SYS_FREQ 16000000

// Macro to set the APB1 peripheral clock frequency to the system frequency
#define APB1_CLK SYS_FREQ

// Macro to enable the receiver (bit 2 in USART_CR1)
#define CR1_RE (1U << 2)

// Macro to enable the transmitter (bit 3 in USART_CR1)
#define CR1_TE (1U << 3)

// Macro to enable the transmission complete interrupt (bit 6 in USART_CR1)
#define CR1_TCIE (1U << 6)

// Macro to enable the UART module (bit 13 in USART_CR1)
#define CR1_UE (1U << 13)

// Macro to check whether the transmission is complete (bit 6 in USART_SR)
#define SR_TC (1U << 6)

// Macro to represent the TXE (transmit data register empty) bit (bit 7 in USART_SR)
#define SR_TXE (1U << 7)

// Macro to enable DMA mode for reception (bit 6 in USART_CR3)
#define CR3_DMAR (1U << 6)

// Macro to enable DMA mode for transmission (bit 7 in USART_CR3)
#define CR3_DMAT (1U << 7)

// Macro to enable clock for DMA1 controller (bit 21 in RCC_AHB1ENR register)
#define DMA1EN (1U << 21)

// Macro to enable the DMA stream (bit 0 in DMA_SxCR register)
#define DMA_SCR_EN (1U << 0)

// Macro to enable memory increment mode (increments memory address pointer after each data transfer) (bit 10 in DMA_SxCR register)
#define DMA_SCR_MINC (1U << 10)

// Macro to enable peripheral increment mode (increments peripheral address pointer after each data transfer) (bit 9 in DMA_SxCR register)
#define DMA_SCR_PINC (1U << 9)

// Macro to enable DMA circular mode (bit 8 in DMA_SxCR register)
#define DMA_SCR_CIRC (1U << 8)

// Macro to enable transfer complete interrupt (bit 4 in DMA_SxCR register)
#define DMA_SCR_TCIE (1U << 4)

// Macro to enable transfer error interrupt (bit 2 in DMA_SxCR register)
#define DMA_SCR_TEIE (1U << 2)

// Macro to disable DMA direct mode (bit 2 in DMA_SxFCR register)
#define DMA_SFCR_DMDIS (1U << 2)

// Macro to clear direct mode error interrupt flag for Stream 5 (bit 8 in DMA_HIFCR)
#define HIFCR_CDMEIF5 (1U << 8)

// Macro to clear transfer error interrupt flag for Stream 5 (bit 9 in DMA_HIFCR)
#define HIFCR_CTEIF5 (1U << 9)

// Macro to clear transfer complete interrupt flag for Stream 5 (bit 11 in DMA_HIFCR)
#define HIFCR_CTCIF5 (1U << 11)

// Macro to clear direct mode error interrupt flag for Stream 6 (bit 18 in DMA_HIFCR)
#define HIFCR_CDMEIF6 (1U << 18)

// Macro to clear transfer error interrupt flag for Stream 6 (bit 19 in DMA_HIFCR)
#define HIFCR_CTEIF6 (1U << 19)

// Macro to clear transfer complete interrupt flag for Stream 6 (bit 21 in DMA_HIFCR)
#define HIFCR_CTCIF6 (1U << 21)

// Macro to check whether a transmission complete event is occurred on Stream 5 (bit 11 in DMA_HISR)
#define HIFSR_TCIF5 (1U << 11)

// Macro to check whether a transmission complete event is occurred on Stream 6 (bit 21 in DMA_HISR)
#define HIFSR_TCIF6 (1U << 21)

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart2_set_baudrate(uint32_t periph_clk, uint32_t baudrate);

// Array to store the incoming UART2 data
char uart2_data_buffer[UART2_DATA_BUFF_SIZE];

uint8_t g_uart_cmplt; // UART2 event flag
uint8_t g_rx_cmplt;   // DMA1 Stream 5 event flag (indicates new data has been received)
uint8_t g_tx_cmplt;   // DMA1 Stream 6 event flag (indicates new data has been transmitted)

void uart2_rx_tx_init(void) {
	/************UART2 GPIOA Pins Configuration**********/
	// Enable the clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Configure GPIOA Pin 2 to operate in alternate function mode (MODER2[1:0] = 10)
    GPIOA->MODER &= ~(1U << 4);
    GPIOA->MODER |= (1U << 5);

    // Configure GPIOA Pin 3 to operate in alternate function mode (MODER3[1:0] = 10)
    GPIOA->MODER &= ~(1U << 6);
    GPIOA->MODER |= (1U << 7);

    // Set PA2 alternate function type to AF7 (UART2_TX)
    GPIOA->AFR[0] |= (1U << 8);
    GPIOA->AFR[0] |= (1U << 9);
    GPIOA->AFR[0] |= (1U << 10);
    GPIOA->AFR[0] &= ~(1U << 11);

    // Set PA3 alternate function type to AF7 (UART2_RX)
    GPIOA->AFR[0] |= (1U << 12);
    GPIOA->AFR[0] |= (1U << 13);
    GPIOA->AFR[0] |= (1U << 14);
    GPIOA->AFR[0] &= ~(1U << 15);

    /************UART2 Configuration**********/
    // Enable the clock access to UART2
    RCC->APB1ENR |= UART2EN;

    // Set UART2 baud rate
    uart2_set_baudrate(APB1_CLK, UART_BAUDRATE);

    // Select to use DMA for both transmission and reception
    USART2->CR3 = CR3_DMAT | CR3_DMAR;

    // Configure transfer direction to both transmit and receive
    USART2->CR1 = CR1_TE | CR1_RE;

    // Clear any pending transmission complete flags
    USART2->SR &= ~SR_TC;

    // Enable the transmission complete interrupt
    USART2->CR1 |= CR1_TCIE;

    // Enable UART2 module
    USART2->CR1 |= CR1_UE;

    // Enable the USART2 interrupt line in the NVIC to handle UART2 interrupts
    NVIC_EnableIRQ(USART2_IRQn);
}

void dma1_init(void) {
	// Enable the clock access to DMA1
    RCC->AHB1ENR |= DMA1EN;

    // Enable the DMA1_Stream6 interrupt line in the NVIC to handle DMA1-related interrupts
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void dma1_stream5_uart2_rx_config(void) {
	// Disable DMA1 stream before making any configurations
    DMA1_Stream5->CR &= ~DMA_SCR_EN;

    // Wait until DMA1 Stream5 is disabled before configuring it
    while ((DMA1_Stream5->CR & (DMA_SCR_EN))) {
    }

    // Clear any existing interrupt flags for DMA1 Stream5
    DMA1->HIFCR = HIFCR_CDMEIF5 | HIFCR_CTEIF5 | HIFCR_CTCIF5;

    // Set peripheral address to the UART2 data register
    DMA1_Stream5->PAR = (uint32_t)(&(USART2->DR));

    // Set memory address to the uart2_data_buffer array
    DMA1_Stream5->M0AR = (uint32_t)(&uart2_data_buffer);

    // Set the number of data items to transfer
    DMA1_Stream5->NDTR = (uint16_t)UART2_DATA_BUFF_SIZE;

    // Select the DMA Channel 4
    DMA1_Stream5->CR &= ~(1U << 25);
    DMA1_Stream5->CR &= ~(1U << 26);
    DMA1_Stream5->CR |= (1U << 27);

    // Enable memory address increment mode
    DMA1_Stream5->CR |= DMA_SCR_MINC;

    // Enable the transfer complete interrupt
    DMA1_Stream5->CR |= DMA_SCR_TCIE;

    // Enable circular mode for continuous data transfer (reception)
    DMA1_Stream5->CR |= DMA_SCR_CIRC;

    // Set data transfer direction as peripheral-to-memory
    DMA1_Stream5->CR &= ~(1U << 6);
    DMA1_Stream5->CR &= ~(1U << 7);

    // Enable DMA1 stream
    DMA1_Stream5->CR |= DMA_SCR_EN;

    // Enable the DMA1_Stream5 interrupt line in the NVIC to handle DMA1-related interrupts
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void dma1_stream6_uart2_tx_config(uint32_t msg_to_snd, uint32_t msg_len) {
	// Disable DMA1 stream before making any configurations
    DMA1_Stream6->CR &= ~DMA_SCR_EN;

    // Wait until DMA1 Stream6 is disabled before configuring it
    while ((DMA1_Stream6->CR & (DMA_SCR_EN))) {
    }

    // Clear any existing interrupt flags for DMA1 Stream6
    DMA1->HIFCR = HIFCR_CDMEIF6 | HIFCR_CTEIF6 | HIFCR_CTCIF6;

    // Set peripheral address to the UART2 data register
    DMA1_Stream6->PAR = (uint32_t)(&(USART2->DR));

    // Set memory address to the data buffer that will be transmitted
    DMA1_Stream6->M0AR = msg_to_snd;

    // Set the number of data items to transfer
    DMA1_Stream6->NDTR = msg_len;

    // Select the DMA Channel 4
    DMA1_Stream6->CR &= ~(1U << 25);
    DMA1_Stream6->CR &= ~(1U << 26);
    DMA1_Stream6->CR |= (1U << 27);

    // Enable memory address increment mode
    DMA1_Stream6->CR |= DMA_SCR_MINC;

    // Set data transfer direction as memory-to-peripheral
    DMA1_Stream6->CR |= (1U << 6);
    DMA1_Stream6->CR &= ~(1U << 7);

    // Enable the transfer complete interrupt
    DMA1_Stream6->CR |= DMA_SCR_TCIE;

    // Enable DMA2 stream
    DMA1_Stream6->CR |= DMA_SCR_EN;
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate) {
    return ((periph_clk + (baudrate / 2U)) / baudrate);
}

static void uart2_set_baudrate(uint32_t periph_clk, uint32_t baudrate) {
    // Set the baud rate for UART2 by writing the computed divisor to the BRR
    USART2->BRR = compute_uart_bd(periph_clk, baudrate);
}

void USART2_IRQHandler(void) {
	// Set the flag when a UART2 event occurs
    g_uart_cmplt = 1;

    // Clear any pending transmission complete flags
    USART2->SR &= ~SR_TC;
}

void DMA1_Stream5_IRQHandler(void) {
    if ((DMA1->HISR) & HIFSR_TCIF5) {
    	// Set the flag when a transfer complete event occurs on DMA1 Stream 5
        g_rx_cmplt = 1;

        // Clear the transfer complete interrupt flag
        DMA1->HIFCR |= HIFCR_CTCIF5;
    }
}

void DMA1_Stream6_IRQHandler(void) {
    if ((DMA1->HISR) & HIFSR_TCIF6) {
    	// Set the flag when a transfer complete event occurs on DMA1 Stream 6
        g_tx_cmplt = 1;

        // Clear the transfer complete interrupt flag
        DMA1->HIFCR |= HIFCR_CTCIF6;
    }
}
