# STM32F411RE I2C1 Peripheral Registers

This document explains the usage of essential STM32F411RE I2C1 (Inter-Integrated Circuit) registers, which are used to configure and control the I2C1 peripheral for half-duplex serial communication with I2C-compatible devices.

---

## 1. GPIO Alternate Function Configuration for I2C1

To use I2C1, the associated GPIO pins must be configured in alternate function mode with the appropriate AF (Alternate Function) number.

### Common I2C1 Pin Mappings on STM32F411RE:
- **SCL** (Clock) : PB8 (AF4)  
- **SDA** (Data)  : PB9 (AF4)

### Usage:
- `GPIOB_MODER`   : Set pins to Alternate Function mode (`10`)
- `GPIOB_AFRH`    : Set AF4 (I2C1) for PB8–PB9
- `GPIOB_OTYPER`  : Set to open-drain output
- `GPIOB_PUPDR`   : Enable pull-up resistors

### Example (Configuring PB8–PB9 for I2C1):
```
// Enable GPIOB clock
RCC->AHB1ENR |= (1U << 1); // GPIOBEN = Bit 1

// Set PB8, PB9 to Alternate Function mode (MODER = 10)
GPIOB->MODER &= ~((3U << (8 * 2)) | (3U << (9 * 2)));
GPIOB->MODER |=  ((2U << (8 * 2)) | (2U << (9 * 2)));

// Configure output type as open-drain
GPIOB->OTYPER |= (1U << 8) | (1U << 9);

// Enable pull-up resistors
GPIOB->PUPDR &= ~((3U << (8 * 2)) | (3U << (9 * 2)));
GPIOB->PUPDR |=  ((1U << (8 * 2)) | (1U << (9 * 2)));

// Set AF4 for I2C1 on PB8–PB9
GPIOB->AFR[1] &= ~((0xF << ((8 - 8) * 4)) | (0xF << ((9 - 8) * 4)));
GPIOB->AFR[1] |=  ((4U << ((8 - 8) * 4)) | (4U << ((9 - 8) * 4)));
```

---

## 2. RCC_APB1ENR (APB1 Peripheral Clock Enable Register)

The `RCC_APB1ENR` register enables the clock for I2C1 on the APB1 bus.

### Register Address:
- **RCC_APB1ENR**: `0x40023800 + 0x40`

### Usage:
Enable I2C1 clock by setting Bit 21 (`I2C1EN`).

### Example:
```
// Enable I2C1 clock
RCC->APB1ENR |= (1U << 21); // I2C1EN = Bit 21
```

---

## 3. I2C1_CR1 (Control Register 1)

Controls I2C1 configuration including peripheral enable, ACK, start/stop condition generation.

### Register Address:
- **I2C1_CR1**: `0x40005400 + 0x00`

### Usage:
Set parameters such as:
- `PE`: Peripheral enable
- `ACK`: Acknowledge enable
- `START`: Start generation
- `STOP`: Stop generation

### Example (Enable I2C1, ACK, and generate START condition):
```
I2C1->CR1 |= (1U << 10); // ACK: Acknowledge enable
I2C1->CR1 |= (1U << 0);  // PE: Enable I2C1
I2C1->CR1 |= (1U << 8);  // START: Generate START condition
```

---

## 4. I2C1_CR2 (Control Register 2)

Configures the peripheral input clock frequency (in MHz) and optional event/interrupt enable.

### Register Address:
- **I2C1_CR2**: `0x40005400 + 0x04`

### Usage:
Set frequency of APB1 clock (e.g., 16 MHz → `0b10000` = 16).

### Example:
```
I2C1->CR2 = (1U << 4); // Frequency = 16 MHz
```

---

## 5. I2C1_SR1 and SR2 (Status Registers)

Indicate I2C status such as start, address match, data transfer, and bus status.

### Register Addresses:
- **I2C1_SR1**: `0x40005400 + 0x14`
- **I2C1_SR2**: `0x40005400 + 0x18`

### Important Flags:
- `SB`   (SR1 Bit 0): Start bit
- `ADDR` (SR1 Bit 1): Address sent/matched
- `TXE`  (SR1 Bit 7): Transmit register empty
- `RXNE` (SR1 Bit 6): Receive register not empty
- `BTF`  (SR1 Bit 2): Byte transfer finished
- `BUSY` (SR2 Bit 1): I2C bus is busy

### Example (Wait for ADDR and clear by reading SR2):
```
while (!(I2C1->SR1 & (1U << 1))); // Wait for ADDR
volatile uint16_t temp = I2C1->SR2; // Clear ADDR flag
```

---

## 6. I2C1_CCR (Clock Control Register)

Defines SCL frequency based on APB1 clock for standard or fast mode.

### Register Address:
- **I2C1_CCR**: `0x40005400 + 0x1C`

### Standard Mode Example (100 kHz with 16 MHz PCLK1):
```
CCR = 16MHz / (2 × 100kHz) = 80
```

```
I2C1->CCR = 80;
```

---

## 7. I2C1_TRISE (Maximum Rise Time Register)

Defines maximum allowed SCL rise time, calculated from peripheral clock.

### Register Address:
- **I2C1_TRISE**: `0x40005400 + 0x20`

### Standard Mode Example:
```
TRISE = (1000ns / 62.5ns) + 1 = 17
```

```
I2C1->TRISE = 17;
```

---

## 8. I2C1_DR (Data Register)

Used to transmit and receive data.

### Register Address:
- **I2C1_DR**: `0x40005400 + 0x10`

### Usage:
- Write to send data
- Read to receive data

### Example (Transmit and receive one byte):
```
// Write
I2C1->DR = data;

// Read
uint8_t received = I2C1->DR;
```

---

## 9. Enable I2C1 Peripheral

Once configured, enable I2C1 by setting the `PE` bit in `I2C1_CR1`.

### Example:
```
I2C1->CR1 |= (1U << 0); // PE: Enable I2C1
```

---

## 10. Full Example: I2C1 Initialization and Single-Byte Read

```
void I2C1_Init(void) {
    // Enable clocks
    RCC->AHB1ENR |= (1U << 1);   // GPIOBEN
    RCC->APB1ENR |= (1U << 21);  // I2C1EN

    // Configure PB8 and PB9 as AF4 open-drain
    GPIOB->MODER &= ~((3U << (8 * 2)) | (3U << (9 * 2)));
    GPIOB->MODER |= ((2U << (8 * 2)) | (2U << (9 * 2)));
    GPIOB->OTYPER |= (1U << 8) | (1U << 9);
    GPIOB->PUPDR |= (1U << (8 * 2)) | (1U << (9 * 2));
    GPIOB->PUPDR &= ~((1U << (8 * 2 + 1)) | (1U << (9 * 2 + 1)));
    GPIOB->AFR[1] &= ~((0xF << 0) | (0xF << 4));
    GPIOB->AFR[1] |= ((4U << 0) | (4U << 4));

    // Reset I2C1
    I2C1->CR1 |= (1U << 15);
    I2C1->CR1 &= ~(1U << 15);

    // Configure I2C1
    I2C1->CR2 = 16;    // PCLK1 = 16 MHz
    I2C1->CCR = 80;    // 100 kHz
    I2C1->TRISE = 17;  // Maximum rise time

    // Enable I2C1
    I2C1->CR1 |= (1U << 0);  // PE
}
```

---

## Conclusion

To use **I2C1** on the **STM32F411RE**, enable the peripheral clock via `RCC_APB1ENR`, configure the GPIO pins (PB8–PB9) for **AF4 open-drain mode**, and set up the I2C timing parameters using `I2C_CR2`, `I2C_CCR`, and `I2C_TRISE`. Once enabled via the `PE` bit in `I2C1_CR1`, data transmission and reception can be performed using the `I2C1_DR` register, monitored with flags in `I2C1_SR1` and `I2C1_SR2`. This setup enables communication with I2C-compatible devices like sensors, EEPROMs, and real-time clocks.