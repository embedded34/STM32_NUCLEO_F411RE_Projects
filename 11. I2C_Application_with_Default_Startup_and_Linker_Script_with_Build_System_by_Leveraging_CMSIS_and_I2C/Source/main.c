/**
 * This code demonstrates how to initialize the I2C 1 peripheral for serial communication on
 * an STM32F4 microcontroller using a custom I2C1 driver built on top of the CMSIS framework.
 */
#include "i2c.h"
#include "uart.h"

/**
 * Main function: Demonstrates I2C1 peripheral usage.
 */
int main(void) {
    // Initialize UART 2 peripheral for debugging
    uart2_init();

    // I2C1 communication-related code lines

    return 0;
}

/**
 * Example Usage:
 *
 * This code demonstrates how to configure I2C1 using GPIOB pins (PB8: SCL, PB9: SDA)
 * and communicate with a peripheral device (e.g., I2C EEPROM or temperature sensor).
 *
 * Steps:
 * 1. Initialize GPIOB for I2C functionality.
 * 2. Configure I2C1 peripheral.
 * 3. Transmit memory address or command to the slave device.
 * 4. Read or write data.
 * 5. End the transmission with a stop condition.
 *
 * Example 1: Single-byte read from a register
 *
 *      uint8_t received_data;
 *
 *      i2c1_gpiob_init();
 *
 *      i2c1_single_byte_read(0x50, 0x00, &received_data);  // Read 1 byte from address 0x00
 *
 * Example 2: Multi-byte read (e.g., reading 3 bytes from EEPROM)
 *
 *      uint8_t buffer[3];
 *
 *      i2c1_gpiob_init();
 *
 *      i2c1_multiple_bytes_read(0x50, 0x10, 3, buffer);  // Read 3 bytes from memory address 0x10
 *
 * Example 3: Multi-byte write (e.g., writing 3 bytes to EEPROM)
 *
 *      uint8_t data_to_write[] = {0x11, 0x22, 0x33};
 *
 *      i2c1_gpiob_init();
 *
 *      i2c1_multiple_bytes_write(0x50, 0x10, 3, data_to_write); // Write 3 bytes starting at address 0x10
 *
 * In these examples:
 * - 0x50 is the 7-bit I2C address of the slave device.
 * - 0x10 is the memory/register address inside the slave.
 * - Data is either sent to or received from that memory location.
 */

