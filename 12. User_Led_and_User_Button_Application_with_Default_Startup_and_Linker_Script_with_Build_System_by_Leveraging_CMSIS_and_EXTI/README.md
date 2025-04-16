# STM32F411RE EXTI Peripheral Registers

This document outlines the configuration and usage of the EXTI (External Interrupt/Event Controller) peripheral in the STM32F411RE microcontroller. EXTI allows GPIO pins to trigger interrupts or events based on configurable signal changes (rising edge, falling edge, or both).

---

## 1. GPIO Configuration for EXTI Lines

To use EXTI, corresponding GPIO pins must be configured in input mode and mapped to EXTI lines via the SYSCFG external interrupt configuration registers.

### Common EXTI Line Mapping:
Each GPIO pin (from PA0 to PE15) can be connected to EXTI0 to EXTI15 respectively.

### Usage:
- `GPIOx_MODER` : Set pin to input mode
- `SYSCFG_EXTICR` : Select GPIO port source for the EXTI line
- `EXTI_IMR` : Unmask interrupt
- `EXTI_RTSR` / `EXTI_FTSR` : Enable rising/falling edge trigger

### Example (Configure PA0 as EXTI0):
```
// Enable GPIOA and SYSCFG clocks
RCC->AHB1ENR |= (1U << 0);  // GPIOAEN
RCC->APB2ENR |= (1U << 14); // SYSCFGEN

// Set PA0 as input (default after reset)
GPIOA->MODER &= ~(3U << (0 * 2)); // Input mode

// Select PA0 as EXTI0 source
SYSCFG->EXTICR[0] &= ~(0xF << 0); // EXTICR1, EXTI0 = PA0

// Enable EXTI0 interrupt
EXTI->IMR |= (1U << 0);  // Unmask EXTI0

// Trigger on rising edge
EXTI->RTSR |= (1U << 0);
EXTI->FTSR &= ~(1U << 0);
```

---

## 2. SYSCFG_EXTICR (External Interrupt Configuration Register)

Maps EXTI lines to GPIO ports (PA–PH).

### Register Addresses:
- `SYSCFG_EXTICR[0 to 3]`: `0x40013808 – 0x40013814`

Each register maps 4 EXTI lines:
- `EXTICR1`: EXTI0–3
- `EXTICR2`: EXTI4–7
- `EXTICR3`: EXTI8–11
- `EXTICR4`: EXTI12–15

### Example (EXTI3 to PB3):
```
SYSCFG->EXTICR[0] &= ~(0xF << 12);
SYSCFG->EXTICR[0] |=  (1U << 12); // 0b0001 = Port B
```

---

## 3. EXTI_IMR (Interrupt Mask Register)

Controls whether interrupts from specific EXTI lines are enabled.

### Register Address:
- `EXTI_IMR`: `0x40013C00 + 0x00`

### Usage:
Set bit `n` to unmask EXTI line `n`.

### Example:
```
EXTI->IMR |= (1U << 3); // Unmask EXTI3
```

---

## 4. EXTI_RTSR and EXTI_FTSR

These configure trigger sensitivity:
- `RTSR`: Rising edge
- `FTSR`: Falling edge

### Register Addresses:
- `EXTI_RTSR`: `0x40013C00 + 0x08`
- `EXTI_FTSR`: `0x40013C00 + 0x0C`

### Usage:
Set appropriate bits to detect edges.

### Example (Enable both edges for EXTI5):
```
EXTI->RTSR |= (1U << 5);
EXTI->FTSR |= (1U << 5);
```

---

## 5. NVIC (Nested Vectored Interrupt Controller)

After configuring EXTI, you must enable and prioritize the interrupt in the NVIC.

### Example (Enable EXTI0 in NVIC):
```
NVIC_EnableIRQ(EXTI0_IRQn);
NVIC_SetPriority(EXTI0_IRQn, 1);
```

---

## 6. EXTI_PR (Pending Register)

Interrupt pending bits are set by hardware when an edge is detected. They must be cleared manually in the ISR by writing 1.

### Register Address:
- `EXTI_PR`: `0x40013C00 + 0x14`

### Example (Clear EXTI0 pending bit):
```
EXTI->PR |= (1U << 0); // Write 1 to clear
```

---

## 7. Full Example: EXTI0 Configuration (PA0 with Rising Edge)

```
void EXTI0_Init(void) {
    // Enable GPIOA and SYSCFG clocks
    RCC->AHB1ENR |= (1U << 0);   // GPIOAEN
    RCC->APB2ENR |= (1U << 14);  // SYSCFGEN

    // Set PA0 as input (optional, reset state is input)
    GPIOA->MODER &= ~(3U << (0 * 2));

    // Map EXTI0 to PA0
    SYSCFG->EXTICR[0] &= ~(0xF << 0);

    // Unmask EXTI0
    EXTI->IMR |= (1U << 0);

    // Trigger on rising edge only
    EXTI->RTSR |= (1U << 0);
    EXTI->FTSR &= ~(1U << 0);

    // Enable EXTI0 interrupt in NVIC
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_SetPriority(EXTI0_IRQn, 1);
}

void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1U << 0)) {
        // Handle interrupt (e.g., toggle LED, set flag)
        
        EXTI->PR |= (1U << 0); // Clear interrupt pending bit
    }
}
```

---

## Conclusion

To enable external interrupts using the **EXTI** peripheral on **STM32F411RE**, configure the GPIO as an input, map the EXTI line using `SYSCFG_EXTICRx`, enable edge detection in `EXTI_RTSR` / `EXTI_FTSR`, unmask the line with `EXTI_IMR`, and configure the interrupt in the **NVIC**. When the interrupt fires, clear the pending flag in the ISR by writing `1` to `EXTI_PR`. This setup enables external event detection from buttons, sensors, or other GPIO-driven sources.