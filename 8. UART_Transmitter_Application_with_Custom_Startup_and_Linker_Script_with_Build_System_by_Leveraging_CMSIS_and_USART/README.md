# STM32F411RE USART2 Peripheral Registers

This document explains the usage of essential STM32F411RE USART2 (Universal Synchronous/Asynchronous Receiver/Transmitter) registers, which are used to configure and control the UART2 peripheral for serial communication.

## 0. GPIO Alternate Function Configuration for USART2
To use USART2, the appropriate GPIO pins must be configured to alternate function mode and mapped to the USART2 peripheral.

### Common USART2 Pin Mappings on STM32F411RE:
- **TX (Transmit)**: PA2
- **RX (Receive)** : PA3
- **Alternate Function Number**: AF7

### Usage:
- `GPIOA_MODER`: Set PA2 and PA3 to Alternate Function mode (10)
- `GPIOA_AFRL` : Set AF7 (0111) for PA2 and PA3

### Example:
```
// Enable GPIOA clock
RCC->AHB1ENR |= (1U << 0); // GPIOAEN = Bit 0

// Set PA2 and PA3 to alternate function mode (MODER = 10)
GPIOA->MODER &= ~((3U << (2 * 2)) | (3U << (3 * 2))); // Clear bits
GPIOA->MODER |=  ((2U << (2 * 2)) | (2U << (3 * 2))); // Set AF mode

// Set PA2 and PA3 to AF7 (USART2)
GPIOA->AFR[0] &= ~((0xF << (4 * 2)) | (0xF << (4 * 3))); // Clear bits
GPIOA->AFR[0] |=  ((7U << (4 * 2)) | (7U << (4 * 3)));   // AF7 = USART2
```

---

## 1. RCC_APB1ENR (APB1 Peripheral Clock Enable Register)

The `RCC_APB1ENR` register enables or disables the clocks for peripherals on the APB1 bus, including USART2.

### Register Address:
- **RCC_APB1ENR**: `0x40023800 + 0x40`

### Usage:
To enable the clock for USART2, set the corresponding bit in the `RCC_APB1ENR` register.

### Example:
```
// Enable USART2 clock
RCC->APB1ENR |= (1U << 17); // USART2EN = Bit 17
```

---

## 2. USART2_CR1 (Control Register 1)

The `USART2_CR1` register is used to configure basic USART2 operations such as enabling the transmitter, receiver, and USART itself.

### Register Address:
- **USART2_CR1**: `0x40004400 + 0x0C`

### Usage:
Enable USART2 and configure transmission/reception.

### Example:
```
// Enable USART2, Transmitter, and Receiver
USART2->CR1 |= (1U << 13); // UE: USART enable
USART2->CR1 |= (1U << 3);  // TE: Transmitter enable
USART2->CR1 |= (1U << 2);  // RE: Receiver enable
```

---

## 3. USART2_BRR (Baud Rate Register)

The `USART2_BRR` register is used to set the baud rate for communication based on the peripheral clock.

### Register Address:
- **USART2_BRR**: `0x40004400 + 0x08`

### Usage:
Configure the baud rate by calculating the correct value based on the APB1 clock frequency.

### Example:
```
// Set baud rate to 9600 (assuming 16 MHz APB1 clock frequency)
USART2->BRR = 0x683; // Pre-calculated value for 9600 bps
```

---

## 4. USART2_SR (Status Register)

The `USART2_SR` register provides flags indicating the status of USART2, such as whether data has been received or transmitted.

### Register Address:
- **USART2_SR**: `0x40004400 + 0x00`

### Usage:
Check this register to monitor transmission and reception status.

### Example:
```
// Wait until transmit data register is empty
while (!(USART2->SR & (1U << 7))); // TXE: Transmit data register empty

// Wait until data is received
while (!(USART2->SR & (1U << 5))); // RXNE: Read data register not empty
```

---

## 5. USART2_DR (Data Register)

The `USART2_DR` register is used to send and receive data. Writing to it transmits data; reading from it retrieves received data.

### Register Address:
- **USART2_DR**: `0x40004400 + 0x04`

### Usage:
Use this register for sending and receiving one byte at a time.

### Example:
```
// Transmit a character
USART2->DR = 'A';

// Receive a character
char received = USART2->DR;
```

---

## 6. USART2_CR2 / CR3 (Control Registers 2 & 3)

These registers provide advanced control over USART2 features such as stop bits, flow control, and LIN mode.

### Register Addresses:
- **USART2_CR2**: `0x40004400 + 0x10`
- **USART2_CR3**: `0x40004400 + 0x14`

### Example:
```
// Set 1 stop bit (default)
USART2->CR2 &= ~(3U << 12);

// Disable hardware flow control
USART2->CR3 &= ~((1U << 9) | (1U << 8)); // CTS and RTS
```

---

## Conclusion

To use USART2 on the STM32F411RE, both the peripheral and GPIO alternate functions must be correctly configured. By enabling the clock in `RCC_APB1ENR`, setting the GPIO pins (e.g., `PA2` and `PA3`) to alternate function mode with AF7, and configuring USART2 control and status registers (`USART2_CR1`, `USART2_BRR`, `USART2_SR`, `USART2_DR`), you can set up reliable UART communication. This is useful for debugging, data exchange, and communicating with other devices such as sensors, computers, or microcontrollers.