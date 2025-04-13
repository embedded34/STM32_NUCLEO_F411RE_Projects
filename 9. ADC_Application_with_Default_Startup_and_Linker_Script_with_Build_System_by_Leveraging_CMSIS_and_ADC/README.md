# STM32F411RE ADC1 Peripheral Registers

This document explains the usage of essential STM32F411RE ADC1 (Analog-to-Digital Converter) registers, which are used to configure and control the ADC peripheral for analog signal sampling and conversion.

---

## 0. GPIO Analog Mode Configuration for ADC1

To use ADC1, the appropriate GPIO pin(s) must be configured in analog mode to accept analog input signals.

### Common ADC1 Input Pin Mappings on STM32F411RE:
- **Channel 0**: PA0  
- **Channel 1**: PA1  
- **Channel 2**: PA2  
- *(up to Channel 15 available on various pins)*

### Usage:
- `GPIOA_MODER`: Set pin(s) to Analog mode (`11`)
- `GPIOA_PUPDR`: Disable pull-up/pull-down resistors (`00`)

### Example (Configuring PA0 for ADC1_IN0):
```
// Enable GPIOA clock
RCC->AHB1ENR |= (1U << 0); // GPIOAEN = Bit 0

// Set PA0 to analog mode (MODER = 11)
GPIOA->MODER |= (3U << (0 * 2)); // MODER0 = 11

// Disable pull-up/pull-down for PA0
GPIOA->PUPDR &= ~(3U << (0 * 2)); // PUPDR0 = 00
```

---

## 1. RCC_APB2ENR (APB2 Peripheral Clock Enable Register)

The `RCC_APB2ENR` register enables or disables clocks for peripherals on the APB2 bus, including ADC1.

### Register Address:
- **RCC_APB2ENR**: `0x40023800 + 0x44`

### Usage:
To enable the ADC clock, set the corresponding bit in the `RCC_APB2ENR` register.

### Example:
```
// Enable ADC1 clock
RCC->APB2ENR |= (1U << 8); // ADC1EN = Bit 8
```

---

## 2. ADC1_CCR (Common Control Register)

Contains common configuration for ADC1. You typically use this to configure the clock prescaler.

### Register Address:
- **ADC1_CCR**: `0x40012300 + 0x04`

### Usage:
Set the ADC prescaler.

### Example:
```
// Set ADC1 prescaler to PCLK2 divided by 4
ADC1->CCR &= ~(3U << 16);
ADC1->CCR |=  (1U << 16); // ADCPRE = 01: divide by 4
```

---

## 3. ADC1_CR2 (Control Register 2)

Controls ADC1 operations such as conversion start, alignment, and data handling.

### Register Address:
- **ADC1_CR2**: `0x40012000 + 0x08`

### Usage:
Enable ADC, set alignment, and trigger conversion.

### Example:
```
// Enable ADC1 and set right alignment
ADC1->CR2 |= (1U << 0);   // ADON: Enable ADC
ADC1->CR2 &= ~(1U << 11); // ALIGN = 0: Right alignment
```

---

## 4. ADC1_SQR3 (Regular Sequence Register 3)

Defines the first channel in the conversion sequence (used when only 1 channel is needed).

### Register Address:
- **ADC1_SQR3**: `0x40012000 + 0x34`

### Usage:
Specify the input channel to convert.

### Example (Select Channel 0 → PA0):
```
ADC1->SQR3 &= ~0x1F;     // Clear bits
ADC1->SQR3 |=  0x00;     // SQ1 = Channel 0
```

---

## 5. ADC1_CR1 (Control Register 1)

Contains configuration bits for resolution, scan mode, and analog watchdog.

### Register Address:
- **ADC1_CR1**: `0x40012000 + 0x04`

### Usage:
Set resolution (e.g., 12-bit, 10-bit), and enable scan mode if needed.

### Example:
```
// Set resolution to 12-bit (default)
ADC1->CR1 &= ~(3U << 24); // RES = 00
```

---

## 6. ADC1_SR (Status Register)

Indicates the current status of ADC operations (e.g., end of conversion).

### Register Address:
- **ADC1_SR**: `0x40012000 + 0x00`

### Usage:
Check whether the conversion is complete.

### Example:
```
// Wait for end of conversion
while (!(ADC1->SR & (1U << 1))); // EOC = Bit 1
```

---

## 7. ADC1_DR (Regular Data Register)

Contains the result of the most recent ADC conversion.

### Register Address:
- **ADC1_DR**: `0x40012000 + 0x4C`

### Usage:
Read this register after EOC is set to get the digital value.

### Example:
```
// Read converted value
uint16_t value = ADC1->DR;
```

---

## 8. Start a Conversion

A single conversion can be started by setting the SWSTART bit in `ADC1_CR2`.

### Example:
```
// Start conversion
ADC1->CR2 |= (1U << 30); // SWSTART = Bit 30
```

---

## Conclusion

To use **ADC1** on the **STM32F411RE**, begin by enabling the ADC clock via `RCC_APB2ENR`, configure GPIO pins to analog mode, and set the conversion parameters through `ADC1_CR1`, `CR2`, and `SQR3`. Once configured, start a conversion with `SWSTART`, wait for the end-of-conversion flag in `ADC1_SR`, and then read the result from `ADC1_DR`. This setup allows you to digitize analog signals for use in digital processing and control logic.