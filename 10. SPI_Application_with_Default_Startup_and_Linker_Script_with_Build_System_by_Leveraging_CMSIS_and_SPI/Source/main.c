/**
 * This code demonstrates how to initialize the SPI 1 peripheral for serial communication on
 * an STM32F4 microcontroller using a custom SPI1 driver built on top of the CMSIS framework.
 */
#include "spi.h"
#include "uart.h"

/**
 * Main function: Demonstrates SPI1 peripheral usage.
 */
int main(void) {
    // Initialize UART 2 peripheral for debugging
    uart2_init();

    // SPI1 communication-related code lines

    return 0;
}

/**
 * Example Usage:
 *
 * This code demonstrates how to configure SPI1 using GPIOA pins (PA5: SCK, PA6: MISO, PA7: MOSI)
 * and communicate with a peripheral device (e.g., SPI EEPROM or sensor).
 * PA9 is used as a manual chip select (SS) pin.
 *
 * Steps:
 * 1. Initialize GPIOA for SPI functionality.
 * 2. Configure SPI1 peripheral.
 * 3. Enable the slave device by pulling CS (PA9) low.
 * 4. Transmit or receive data.
 * 5. Disable the slave device by pulling CS high.
 *
 * Example:
 *
 *      uint8_t tx_data[] = {0x9F};        // Example command to read device ID
 *      uint8_t rx_data[3];                // Buffer to hold response
 *
 *      spi1_gpioa_init();                 // Step 1
 *      spi1_config();                     // Step 2
 *
 *      spi1_cs_enable();                  // Step 3
 *      spi1_transmit(tx_data, 1);         // Step 4a: Send command
 *      spi1_receive(rx_data, 3);          // Step 4b: Read response
 *      spi1_cs_disable();                 // Step 5
 *
 * The received device ID will be stored in rx_data[0], rx_data[1], and rx_data[2].
 */
