/**
 * This code demonstrates three use cases involving DMA and UART on the STM32F411RE
 * microcontroller using the CMSIS framework:
 *
 * 1. ADC + DMA2 (main_1):
 *    - Initializes UART2 for debugging and configures ADC1 with DMA2.
 *    - Samples two analog input channels (PA0 and PA1) in scan mode.
 *    - DMA automatically transfers the conversion results into memory.
 *    - The main loop continuously prints the sensor values via UART2.
 *
 * 2. UART RX/TX + DMA1 (main_2):
 *    - Initializes UART2 for bidirectional communication using DMA1.
 *    - Waits for UART2 input via DMA1, and upon reception, echoes the message back.
 *
 * 3. Memory-to-Memory Transfer + DMA2 (main):
 *    - Demonstrates use of DMA2 Stream 0 for memory-to-memory data transfer.
 *    - Copies data from one array to another and confirms the transfer by printing
 *      the results over UART2.
 *
 * Each section is written in its own main function (main_1, main_2, and main)
 * for clarity and modular testing.
 */

#include <stdio.h>
#include "adc_dma.h"
#include "uart.h"

extern uint16_t adc1_raw_data[NUM_OF_CHANNELS];

/**
 * Main_1 function: Initializes UART2 and ADC1 with DMA2 for sensor reading.
 * Continuously reads ADC values and prints them via UART.
 */
int main_1(void) {
    // Initialize UART 2 peripheral for debugging
    uart2_init();

    // Initialize ADC 1 with DMA 2 to continuously read sensor data from ADC 1 channels connected to GPIOA pins
    adc1_dma2_init();

    while (1) {
    	// Print the values from two sensors stored in the adc1_raw_data array to the console
        printf("Value from sensor one : %d \n\r ", adc1_raw_data[0]);
        printf("Value from sensor two : %d \n\r ", adc1_raw_data[1]);

        // Simple delay loop
        for (volatile int i = 0; i < 90000; i++) {
        }
    }

    return 0;
}

/* ================================================================================================================= */
#include <stdio.h>
#include <string.h>
#include "uart_dma.h"

extern uint8_t g_uart_cmplt;
extern uint8_t g_rx_cmplt;
extern uint8_t g_tx_cmplt;
extern char uart2_data_buffer[UART2_DATA_BUFF_SIZE];
char msg_buff[150] ={'\0'}; // String buffer initialized to zero for UART 2 messages

/**
 * Main_2 function: Initializes UART2 and DMA1 for communication.
 * Waits for UART messages with 5 characters, echoes them back using
 * DMA-based transmission.
 */
int main_2(void) {
	// Initialize UART 2 peripheral for both transmitting and receiving
    uart2_rx_tx_init();

    // Initialize DMA 1 controller
    dma1_init();

    // Configure DMA 1 Stream 5 for UART 2 reception
    dma1_stream5_uart2_rx_config();

    sprintf(msg_buff, "Initialization is complete...\n\r");

    // Configure DMA 1 Stream 6 for UART 2 transmission
    dma1_stream6_uart2_tx_config((uint32_t)msg_buff, strlen(msg_buff));

    // Wait until transmission is complete
    while (!g_tx_cmplt) {
    }

    // Continuously check whether a UART message has been received
    while (1) {
        if (g_rx_cmplt) {
        	// Format the received message into the output buffer
            sprintf(msg_buff, "Message received : %s \r\n", uart2_data_buffer);

            // Reset completion flags before the next transmission
            g_rx_cmplt = 0;
            g_tx_cmplt = 0;
            g_uart_cmplt = 0;

            // Transmit the formatted received UART message using DMA
            dma1_stream6_uart2_tx_config((uint32_t)msg_buff, strlen(msg_buff));

            // Wait until transmission is complete
            while (!g_tx_cmplt) {
            }
        }
    }

    return 0;
}

/* ================================================================================================================= */
#include <stdio.h>
#include "dma.h"
#include "uart.h"

#define BUFFER_SIZE 5
uint16_t sensor_data_arr[BUFFER_SIZE] = {892, 731, 1234, 90, 23};
uint16_t temp_data_arr[BUFFER_SIZE];

volatile uint8_t g_transfer_cmplt; // DMA2 Stream 0 transfer complete flag to monitor the transfer status

/**
 * Main function: Initializes UART2 for debugging and DMA2 Stream 0 for
 * memory-to-memory data transfer. Transfers sensor data to a temporary
 * buffer using DMA, then prints the transferred data over UART.
 */
int main(void) {
	// Initialize the transfer complete flag to zero
    g_transfer_cmplt = 0;

    // Initialize UART 2 peripheral for debugging
    uart2_init();

    // Configure DMA 2 Stream 0
    dma2_stream0_mem2mem_config();

    // Start the DMA transfer
    dma2_transfer_start((uint32_t)sensor_data_arr, (uint32_t)temp_data_arr, BUFFER_SIZE);

    // Wait until transmission is complete
    while (!g_transfer_cmplt) {
    }

    // Print the contents of the temp_data_arr to the console, confirming that the data has been successfully transferred
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("Temporary buffer[%d]: %d\r\n", i, temp_data_arr[i]);
    }

    // Reset the transfer completion flag
    g_transfer_cmplt = 0;

    while (1) {
    }

    return 0;
}

void DMA2_Stream0_IRQHandler(void) {
    // Check if transfer complete interrupt occurred, i.e. the DMA transfer has successfully finished
    if ((DMA2->LISR) & LISR_TCIF0) {
    	// Set the flag when a transfer complete event occurs on DMA2 Stream 0
        g_transfer_cmplt = 1;

        // Clear the transfer complete interrupt flag
        DMA2->LIFCR |= LIFCR_CTCIF0;
    }

    // Check if transfer error interrupt occurred
    if ((DMA2->LISR) & LISR_TEIF0) {
        // Do something for error handling...

    	// Clear the transfer error interrupt flag
        DMA2->LIFCR |= LIFCR_CTEIF0;
    }
}
