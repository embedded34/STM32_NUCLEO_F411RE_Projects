# STM32F411RE SPI1 Peripheral Registers

This document explains the usage of essential STM32F411RE SPI1 (Serial Peripheral Interface) registers, which are used to configure and control the SPI1 peripheral for full-duplex serial communication.

---

## 1. GPIO Alternate Function Configuration for SPI1

To use SPI1, the associated GPIO pins must be configured in alternate function mode with the appropriate AF (Alternate Function) number.

### Common SPI1 Pin Mappings on STM32F411RE:
- **SCK**  (Clock)   : PA5 (AF5)  
- **MISO** (Master In Slave Out): PA6 (AF5)  
- **MOSI** (Master Out Slave In): PA7 (AF5)  
- **NSS**  (Slave Select - optional): PA4 (AF5)  

### Usage:
- `GPIOA_MODER`: Set pins to Alternate Function mode (`10`)
- `GPIOA_AFRL`: Set AF5 (SPI1) for PA4–PA7
- `GPIOA_OSPEEDR`: Set high speed if needed
- `GPIOA_PUPDR`: Configure pull-up/down depending on mode

### Example (Configuring PA5–PA7 for SPI1):
```
// Enable GPIOA clock
RCC->AHB1ENR |= (1U << 0); // GPIOAEN = Bit 0

// Set PA5, PA6, PA7 to Alternate Function mode (MODER = 10)
GPIOA->MODER &= ~((3U << (5 * 2)) | (3U << (6 * 2)) | (3U << (7 * 2)));
GPIOA->MODER |=  ((2U << (5 * 2)) | (2U << (6 * 2)) | (2U << (7 * 2)));

// Set AF5 for SPI1 on PA5–PA7
GPIOA->AFR[0] &= ~((0xF << (5 * 4)) | (0xF << (6 * 4)) | (0xF << (7 * 4)));
GPIOA->AFR[0] |=  ((5U << (5 * 4)) | (5U << (6 * 4)) | (5U << (7 * 4)));
```

---

## 2. RCC_APB2ENR (APB2 Peripheral Clock Enable Register)

The `RCC_APB2ENR` register enables the clock for SPI1 on the APB2 bus.

### Register Address:
- **RCC_APB2ENR**: `0x40023800 + 0x44`

### Usage:
Enable SPI1 clock by setting Bit 12 (`SPI1EN`).

### Example:
```
// Enable SPI1 clock
RCC->APB2ENR |= (1U << 12); // SPI1EN = Bit 12
```

---

## 3. SPI1_CR1 (Control Register 1)

Controls SPI1 configuration including master/slave mode, baud rate, polarity, phase, and data frame format.

### Register Address:
- **SPI1_CR1**: `0x40013000 + 0x00`

### Usage:
Set parameters such as:
- `MSTR`: Master mode
- `BR[2:0]`: Baud rate control
- `CPOL` / `CPHA`: Clock polarity and phase
- `DFF`: Data frame format (8 or 16 bits)
- `SSI` / `SSM`: Software slave management
- `SPE`: SPI enable

### Example (Master, fPCLK/16, 8-bit, CPOL=0, CPHA=0, software NSS):
```
SPI1->CR1 = (1U << 2) |    // MSTR: Master mode
            (3U << 3) |    // BR[2:0] = 011: fPCLK/16
            (1U << 9) |    // SSM: Software slave management enabled
            (1U << 8);     // SSI: Internal slave select
```

---

## 4. SPI1_CR2 (Control Register 2)

Additional SPI configuration like interrupt enables and data size.

### Register Address:
- **SPI1_CR2**: `0x40013000 + 0x04`

### Usage:
Used less frequently for basic operation, often set to 0 for polling mode.

---

## 5. SPI1_SR (Status Register)

Indicates SPI status flags, such as transmit buffer empty or receive buffer not empty.

### Register Address:
- **SPI1_SR**: `0x40013000 + 0x08`

### Important Flags:
- `TXE` (Bit 1): Transmit buffer empty
- `RXNE` (Bit 0): Receive buffer not empty
- `BSY` (Bit 7): SPI busy

### Example (Wait for transmit buffer to be empty):
```
while (!(SPI1->SR & (1U << 1))); // Wait until TXE is set
```

---

## 6. SPI1_DR (Data Register)

Used to send and receive data.

### Register Address:
- **SPI1_DR**: `0x40013000 + 0x0C`

### Usage:
- Write to send data
- Read to receive data

### Example (Transmit and receive 8-bit data):
```
SPI1->DR = data; // Write to transmit
while (!(SPI1->SR & (1U << 0))); // Wait until RXNE is set
uint8_t received = SPI1->DR;     // Read received data
```

---

## 7. Enable SPI1 Peripheral

Once configured, enable SPI1 by setting the `SPE` bit in `SPI1_CR1`.

### Example:
```
SPI1->CR1 |= (1U << 6); // SPE: Enable SPI
```

---

## 8. Full Example: SPI1 Initialization as Master (8-bit, Software NSS)

```
void SPI1_Init(void) {
    // Enable GPIOA and SPI1 clocks
    RCC->AHB1ENR  |= (1U << 0);  // GPIOAEN
    RCC->APB2ENR  |= (1U << 12); // SPI1EN

    // Configure PA5, PA6, PA7 as AF5 (SPI1)
    GPIOA->MODER  &= ~((3U << (5*2)) | (3U << (6*2)) | (3U << (7*2)));
    GPIOA->MODER  |=  ((2U << (5*2)) | (2U << (6*2)) | (2U << (7*2)));
    GPIOA->AFR[0] &= ~((0xF << (5*4)) | (0xF << (6*4)) | (0xF << (7*4)));
    GPIOA->AFR[0] |=  ((5U << (5*4)) | (5U << (6*4)) | (5U << (7*4)));

    // Configure SPI1: Master mode, baud rate = fPCLK/16, 8-bit, software NSS
    SPI1->CR1 = (1U << 2)  | // MSTR
                (3U << 3)  | // BR = fPCLK/16
                (1U << 9)  | // SSM
                (1U << 8);   // SSI

    // Enable SPI1
    SPI1->CR1 |= (1U << 6); // SPE
}
```

---

## Conclusion

To use **SPI1** on the **STM32F411RE**, enable the peripheral clock via `RCC_APB2ENR`, configure the GPIO pins to **AF5**, and set up the SPI1 parameters in `SPI1_CR1`. After enabling the SPI by setting `SPE`, data transmission and reception can be performed using the `SPI1_DR` register, monitored with status flags in `SPI1_SR`. This setup enables reliable synchronous communication with SPI-compatible devices like sensors, displays, and memory chips.