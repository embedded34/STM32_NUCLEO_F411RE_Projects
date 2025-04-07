# STM32F411RE SysTick Timer Configuration

This document explains the usage of the SysTick timer in the STM32F411RE microcontroller. The SysTick is a 24-bit countdown timer included in the Cortex-M4 core and is commonly used to generate periodic interrupts for system timekeeping or task scheduling.

## 1. SysTick Overview

The SysTick timer provides a simple way to generate periodic interrupts using the processor's clock. It's useful for implementing delay functions, RTOS tick interrupts, and periodic tasks.

### SysTick Registers:
- **SYST_CSR** (Control and Status Register)
- **SYST_RVR** (Reload Value Register)
- **SYST_CVR** (Current Value Register)
- **SYST_CALIB** (Calibration Value Register)

## 2. SYST_CSR (Control and Status Register)

Controls the main operation of the SysTick timer.

### Bit Definitions:
- Bit 0: `ENABLE` – Counter enable (0 = disable, 1 = enable)
- Bit 1: `TICKINT` – Interrupt enable (0 = disable, 1 = enable)
- Bit 2: `CLKSOURCE` – Clock source (0 = external clock, 1 = processor clock)
- Bit 16: `COUNTFLAG` – Returns 1 if timer counted to 0 since last read

### Usage:
```
// Enable SysTick with system clock, interrupt enabled
SysTick->CTRL = (1U << 2) | (1U << 1) | (1U << 0);
```

## 3. SYST_RVR (Reload Value Register)

Holds the value that the counter loads when it reaches zero.

### Usage:

- Set SYST_RVR to the number of clock cycles you want between each SysTick interrupt.
 
- Maximum value: 0xFFFFFF (24-bit)

For example, to generate a 1 millisecond interrupt with a 16 MHz system clock:

16 MHz means 16,000,000 cycles per second → 16,000 cycles = 1 ms

```
SysTick->LOAD = 16000U - 1;  // 16,000 cycles = 1 ms at 16 MHz
```

## 4. SYST_CVR (Current Value Register)

Contains the current value of the countdown timer.

### Usage:
- Writing any value clears the counter to 0.

```
SysTick->VAL = 0;  // Clear the current counter value
```

## 5. SYST_CALIB (Calibration Value Register)

Provides a reference value for 10 ms timing, based on the system clock.

## 6. Putting It All Together

### Example: Initialize SysTick for 1 ms interrupt at 16 MHz
```
void SysTick_Init(void) {
    SysTick->LOAD  = 16000U - 1;              // 1 ms reload value
    SysTick->VAL   = 0;                       // Clear current value
    SysTick->CTRL  = (1U << 2) |              // Use processor clock
                     (1U << 1) |              // Enable interrupt
                     (1U << 0);               // Enable SysTick
}
```

## 7. SysTick Interrupt Handler

Make sure to implement the interrupt handler for SysTick in your code:

```
void SysTick_Handler(void) {
    // This function is called every 1 ms
    // Add your periodic code here
}
```

## Conclusion

SysTick is a lightweight, core-integrated timer ideal for periodic interrupts and timing functions. By configuring `SYST_CSR`, `SYST_RVR`, and `SYST_CVR`, you can create precise delay loops or system ticks for RTOS or bare-metal applications on STM32.
