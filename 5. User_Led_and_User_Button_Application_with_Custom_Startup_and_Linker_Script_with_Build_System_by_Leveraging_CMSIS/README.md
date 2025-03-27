# STM32F411RE Common GPIO and RCC Registers

This document explains the usage of essential STM32F411RE registers used for controlling GPIO pins and enabling peripheral clocks.

## 1. RCC AHB1ENR (AHB1 Peripheral Clock Enable Register)

The `RCC_AHB1ENR` register enables or disables the clocks for peripherals on the AHB1 bus, such as GPIO, DMA, etc.

### Register Address:
- **RCC_AHB1ENR**: `0x40023800 + 0x30`

### Usage:
To enable the clock for a specific GPIO port, modify the corresponding bit in the `RCC_AHB1ENR` register.

Example:
```c
// Enable GPIOA clock
RCC->AHB1ENR |= (1U << 0);
```

## 2. GPIOx_MODER (GPIO Port Mode Register)

The `GPIOx_MODER` register is used to configure the operating mode of each pin of a specific GPIO port.

### Register Address:
- **GPIOx_MODER**: `0x40020000 + 0x00` (For GPIOA, replace `x` with the port letter)

### Pin Modes:
- `00`: Input mode (reset state)
- `01`: General-purpose output
- `10`: Alternate function mode
- `11`: Analog mode

### Usage:
To configure a pin as an output, set the corresponding bits in the `GPIOx_MODER` register.

Example:
```c
// Set GPIOA Pin 0 as output
GPIOA->MODER |= (1U << 0);  // Set bit 0 to 1
GPIOA->MODER &= ~(1U << 1); // Set bit 1 to 0
```

## 3. GPIOx_BSRR (GPIO Port Bit Set/Reset Register)

The `GPIOx_BSRR` register is used to set or reset the GPIO pins.

### Register Address:
- **GPIOx_BSRR**: `0x40020000 + 0x18` (For GPIOA, replace `x` with the port letter)

### Usage:
- Set a pin high:
```c
GPIOA->BSRR |= (1U << 0);  // Set GPIOA Pin 0 high
```
- Reset a pin low:
```c
GPIOA->BSRR |= (1U << (0 + 16));  // Set GPIOA Pin 0 low
```

## 4. GPIOx_IDR (GPIO Port Input Data Register)

The `GPIOx_IDR` register allows you to read the state (high or low) of each GPIO pin.

### Register Address:
- **GPIOx_IDR**: `0x40020000 + 0x10` (For GPIOA, replace `x` with the port letter)

### Usage:
To read the state of a pin, check the corresponding bit in the `GPIOx_IDR` register.

Example:
```c
// Read the state of GPIOA Pin 0
uint32_t pinState = (GPIOA->IDR & (1U << 0)) != 0;
```

## Conclusion

The STM32 microcontroller provides fine-grained control over GPIO pins and peripherals through these registers. By manipulating the `RCC_AHB1ENR`, `GPIOx_MODER`, `GPIOx_BSRR`, and `GPIOx_IDR` registers, you can configure pin modes, enable peripherals, and read/write GPIO pin states efficiently.