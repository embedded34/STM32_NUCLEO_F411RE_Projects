#include "i2c.h"

/* I2C1 Pinout
 * PB8 ----> SCL
 * PB9 ----> SDA
 */

// Macro to enable the clock for GPIOB (bit 1 in RCC_AHB1ENR)
#define GPIOBEN (1U << 1)

// Macro to enable the clock for I2C1 (bit 21 in RCC_APB1ENR)
#define I2C1EN (1U << 21)

// Macro to set the clock control register (CCR) for 100 kHz I2C speed (standard mode)
// CCR = fPCLK1 / (2 * I2C_SCL_Frequency) = 16MHz / (2 * 100kHz) = 80
#define I2C_100KHZ 80

// Macro to set the maximum rise time for standard mode
// In standard mode, the maximum allowed SCL rise time is 1000 ns.
// TRISE = (maximum_rise_time_in_ns / tPCLK1) + 1 = (1000ns / 62.5ns) + 1 = 17
#define SD_MODE_MAX_RISE_TIME 17

// Macro to represent the PE (peripheral enable) bit (bit 0 in I2C_CR1)
#define CR1_PE (1U << 0)

// Macro to represent the BUSY (bus busy) bit (bit 1 in I2C_SR2)
#define SR2_BUSY (1U << 1)

// Macro to represent the START (start generation) bit (bit 8 in I2C_CR1)
#define CR1_START (1U << 8)

// Macro to represent the SB (start bit for master mode) bit (bit 0 in I2C_SR1)
#define SR1_SB (1U << 0)

// Macro to represent the ADDR (address sent (master mode)/matched (slave mode)) bit (bit 1 in I2C_SR1)
#define SR1_ADDR (1U << 1)

// Macro to represent the TxE (data register empty) bit (bit 7 in I2C_SR1)
#define SR1_TXE (1U << 7)

// Macro to represent the ACK (acknowledge enable) bit (bit 10 in I2C_CR1)
#define CR1_ACK (1U << 10)

// Macro to represent the STOP (stop generation) bit (bit 9 in I2C_CR1)
#define CR1_STOP (1U << 9)

// Macro to represent the RxNE (data register not empty) bit (bit 6 in I2C_SR1)
#define SR1_RXNE (1U << 6)

// Macro to represent the BTF (byte transfer finished) bit (bit 2 in I2C_SR1)
#define SR1_BTF (1U << 2)

void i2c1_gpiob_init(void) {
    // Enable the clock access to GPIOB
    RCC->AHB1ENR |= GPIOBEN;

    // Configure GPIOB Pin 8, 9 to operate in alternate function mode (MODERx[1:0] = 10)
    // PB8
    GPIOB->MODER &= ~(1U << 16);  // Set bit 16 to 0
    GPIOB->MODER |= (1U << 17);   // Set bit 17 to 1
    // PB9
    GPIOB->MODER &= ~(1U << 18);  // Set bit 18 to 0
    GPIOB->MODER |= (1U << 19);   // Set bit 19 to 1

    // Configure the pins as output open-drain, which is required for I2C communication
    GPIOB->OTYPER |= (1U << 8);
    GPIOB->OTYPER |= (1U << 9);

    // Enable pull-up resistors for the I2C1 pins (PUPDRx = 01)
    GPIOB->PUPDR |= (1U << 16);
    GPIOB->PUPDR &= ~(1U << 17);
    GPIOB->PUPDR |= (1U << 18);
    GPIOB->PUPDR &= ~(1U << 19);

    // Set alternate function type to AF4 (0b0100)
    // PB8 (I2C1_SCL)
    GPIOB->AFR[1] &= ~(1U << 0);
    GPIOB->AFR[1] &= ~(1U << 1);
    GPIOB->AFR[1] |= (1U << 2);
    GPIOB->AFR[1] &= ~(1U << 3);
    // PB9 (I2C1_SDA)
    GPIOB->AFR[1] &= ~(1U << 4);
    GPIOB->AFR[1] &= ~(1U << 5);
    GPIOB->AFR[1] |= (1U << 6);
    GPIOB->AFR[1] &= ~(1U << 7);

    // Enable the clock access to I2C1
    RCC->APB1ENR |= I2C1EN;

    // Enter the software reset mode (i.e., reset the I2C1 peripheral)
    I2C1->CR1 |= (1U << 15);

    // Come out of the software reset mode
    I2C1->CR1 &= ~(1U << 15);

    // Set APB1 peripheral clock frequency to 16 MHz
    I2C1->CR2 = (1U << 4);

    // Set the clock control register for 100 kHz standard mode
    // CCR = fPCLK1 / (2 * I2C_SCL_Frequency)
    I2C1->CCR = I2C_100KHZ;

    // Set the rise time for the I2C signals
    // TRISE = (maximum_rise_time_in_ns / tPCLK1) + 1
    I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

    // Enable I2C1 module
    I2C1->CR1 |= CR1_PE;
}

void i2c1_single_byte_read(uint8_t saddr, uint8_t maddr, uint8_t *data) {
	uint16_t temp;

    // Ensure the I2C1 bus is not busy before continuing
    while (I2C1->SR2 & (SR2_BUSY)) {
    }

    // Initiate a start condition on the I2C1 bus to begin communication with the slave device
    I2C1->CR1 |= CR1_START;

    // Wait until the start condition is generated
    while (!(I2C1->SR1 & (SR1_SB))) {
    }

    // Transmit the slave address with the write bit, which is 0
    I2C1->DR = saddr << 1;

    // Wait until the end of slave address transmission from master device
    while (!(I2C1->SR1 & (SR1_ADDR))) {
    }

    // Clear the address flag (ADDR bit) by simply reading I2C1 status register 2
    temp = I2C1->SR2;

    // Wait until the data register is empty
    while (!(I2C1->SR1 & (SR1_TXE))) {
    }

    // Transmit the memory address to read from the slave device
    I2C1->DR = maddr;

    // Wait until the data register is empty
    while (!(I2C1->SR1 & (SR1_TXE))) {
    }

    // Initiate a restart condition on the I2C1 bus
    I2C1->CR1 |= CR1_START;

    // Wait until the restart condition is generated
    while (!(I2C1->SR1 & (SR1_SB))) {
    }

    // Transmit the slave address with the read bit, which is 1, to
    // communicate with the slave device, requesting to read data from it
    I2C1->DR = saddr << 1 | (1U);

    // Wait until the end of slave address transmission from master device
    while (!(I2C1->SR1 & (SR1_ADDR))) {
    }

    // Disable the acknowledge bit to prepare for a stop condition
    I2C1->CR1 &= ~CR1_ACK;

    // Clear the address flag (ADDR bit) by simply reading I2C1 status register 2
    temp = I2C1->SR2;

    // Initiate a stop condition on the I2C1 bus
    I2C1->CR1 |= CR1_STOP;

    // Wait until the receive buffer is not empty, indicating that new data has been
    // received and is available in the data register
    while (!(I2C1->SR1 & (SR1_RXNE))) {
    }

    // Read the received byte of data
    (*data++) = I2C1->DR;

    (void)temp;
}

void i2c1_multiple_bytes_read(uint8_t saddr, uint8_t maddr, uint32_t n, uint8_t *data) {
    uint16_t temp;

    // Ensure the I2C1 bus is not busy before continuing
    while (I2C1->SR2 & (SR2_BUSY)) {
    }

    // Initiate a start condition on the I2C1 bus to begin communication with the slave device
    I2C1->CR1 |= CR1_START;

    // Wait until the start condition is generated
    while (!(I2C1->SR1 & (SR1_SB))) {
    }

    // Transmit the slave address with the write bit, which is 0
    I2C1->DR = saddr << 1;

    // Wait until the end of slave address transmission from master device
    while (!(I2C1->SR1 & (SR1_ADDR))) {
    }

    // Clear the address flag (ADDR bit) by simply reading I2C1 status register 2
    temp = I2C1->SR2;

    // Wait until the data register is empty
    while (!(I2C1->SR1 & (SR1_TXE))) {
    }

    // Transmit the memory address to read from the slave device
    I2C1->DR = maddr;

    // Wait until the data register is empty
    while (!(I2C1->SR1 & (SR1_TXE))) {
    }

    // Initiate a restart condition on the I2C1 bus
    I2C1->CR1 |= CR1_START;

    // Wait until the restart condition is generated
    while (!(I2C1->SR1 & (SR1_SB))) {
    }

    // Transmit the slave address with the read bit, which is 1, to
    // communicate with the slave device, requesting to read data from it
    I2C1->DR = saddr << 1 | (1U);

    // Wait until the end of slave address transmission from master device
    while (!(I2C1->SR1 & (SR1_ADDR))) {
    }

    // Clear the address flag (ADDR bit) by simply reading I2C1 status register 2
    temp = I2C1->SR2;

    // Enable the acknowledge bit returned after a byte is received
    I2C1->CR1 |= CR1_ACK;

    while (n > (0U)) {
        // A single-byte read:
        if (n == (1U)) {
            // Disable the acknowledge bit to prepare for a stop condition
            I2C1->CR1 &= ~CR1_ACK;

            // Initiate a stop condition on the I2C1 bus
            I2C1->CR1 |= CR1_STOP;

            // Wait until the receive buffer is not empty, indicating that new data has been
            // received and is available in the data register
            while (!(I2C1->SR1 & (SR1_RXNE))) {
            }

            // Read the received byte of data
            *data++ = I2C1->DR;

            break;
        }
        // A multiple-byte read:
        else {
            // Wait until the receive buffer is not empty, indicating that new data has been
            // received and is available in the data register
            while (!(I2C1->SR1 & (SR1_RXNE))) {
            }

            // Read the received byte of data
            (*data++) = I2C1->DR;

            // Decrement the byte counter
            n--;
        }
    }

    (void)temp;
}

void i2c1_multiple_bytes_write(uint8_t saddr, uint8_t maddr, uint32_t n, uint8_t *data) {
    uint16_t temp;

    // Ensure the I2C1 bus is not busy before continuing
    while (I2C1->SR2 & (SR2_BUSY)) {
    }

    // Initiate a start condition on the I2C1 bus to begin communication with the slave device
    I2C1->CR1 |= CR1_START;

    // Wait until the start condition is generated
    while (!(I2C1->SR1 & (SR1_SB))) {
    }

    // Transmit the slave address with the write bit, which is 0
    I2C1->DR = saddr << 1;

    // Wait until the end of slave address transmission from master device
    while (!(I2C1->SR1 & (SR1_ADDR))) {
    }

    // Clear the address flag (ADDR bit) by simply reading I2C1 status register 2
    temp = I2C1->SR2;

    // Wait until the data register is empty
    while (!(I2C1->SR1 & (SR1_TXE))) {
    }

    // Transmit the memory address to write in the slave device
    I2C1->DR = maddr;

    for (uint32_t i = 0; i < n; i++) {
        // Wait until the data register is empty
        while (!(I2C1->SR1 & (SR1_TXE))) {
        }

        // Transmit one byte to the specified memory address of the slave device
        I2C1->DR = *data++;
    }

    // Wait until the byte transfer has fully completed (both data register and shift register are empty)
    while (!(I2C1->SR1 & (SR1_BTF))) {
    }

    // Initiate a stop condition on the I2C1 bus
    I2C1->CR1 |= CR1_STOP;

    (void)temp;
}
