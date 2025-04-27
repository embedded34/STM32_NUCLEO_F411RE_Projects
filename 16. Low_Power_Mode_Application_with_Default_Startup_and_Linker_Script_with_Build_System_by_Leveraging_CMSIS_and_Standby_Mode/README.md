# STM32F411RE Low Power Modes

This document explains the usage of essential STM32F411RE **low power modes**, which are designed to reduce power consumption during periods of reduced or no activity while still maintaining the device's ability to respond to events such as interrupts or wakeups.

The STM32F411RE microcontroller offers multiple power-saving modes to optimize energy efficiency for battery-powered and energy-sensitive applications.

---

### Key Features:
- Multiple low power modes: **Sleep**, **Stop**, and **Standby**
- Flexible **wake-up sources** including external interrupts, timers, and RTC
- **Clock gating** and **voltage scaling** for optimized power/performance balance
- Integrated **power control (PWR)** interface via registers
- Suitable for **IoT**, **wearable**, and **energy-critical** systems

---

### Typical Use Case:
Used in applications where the system spends significant time idle or waiting for events — such as wireless sensor nodes, portable devices, or systems with strict power budgets.

---

## 1. Sleep Mode

CPU is stopped, but all peripherals keep running. Quickest wake-up time.

### Entry:
```
__WFI(); // Wait For Interrupt instruction
```

### Notes:
- Entered automatically when CPU executes `__WFI()` or `__WFE()` and `SLEEPDEEP` bit is cleared
- Wake-up sources: Any interrupt/event

---

## 2. Stop Mode

All clocks are stopped, but SRAM and register contents are preserved. Lower power than sleep.

### Entry:
```
PWR->CR |= PWR_CR_LPDS;             // (Optional) Low-power regulator in Stop mode
SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  // Set SLEEPDEEP bit
__WFI();                            // Enter Stop mode
```

### Wake-up:
- External interrupt (e.g., EXTI line)
- RTC alarm
- USART (with wakeup from Stop enabled)

---

## 3. Standby Mode

Deepest low-power mode. All clocks off, SRAM and registers lost (except Backup domain). Wake-up resets MCU.

### Entry:
```
PWR->CR |= PWR_CR_CWUF;            // Clear Wake-up flag
PWR->CR |= PWR_CR_PDDS;            // Select Standby mode
SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
__WFI();                           // Enter Standby mode
```

### Wake-up:
- Wakeup pin (PA0)
- RTC alarm/timer
- Reset

---

## 4. PWR_CR (Power Control Register)

Used to configure low power mode settings.

### Key Bits:
- **LPDS** (Bit 0): Low-power deepsleep in Stop mode  
- **PDDS** (Bit 1): Power-down deepsleep (Standby mode)  
- **CWUF** (Bit 2): Clear wake-up flag  
- **VOS** (Bits 15–14): Regulator voltage scaling

---

## 5. SCB->SCR (System Control Register)

Controls sleep behavior of the Cortex-M4 core.

### Key Bits:
- **SLEEPDEEP** (Bit 2): Enables deep sleep mode when set
- **SLEEPONEXIT** (Bit 1): Enters sleep mode on ISR exit if set

---

## 6. Wake-Up Sources Configuration

To wake from Stop or Standby mode, configure:
- **EXTI line** to generate interrupt
- **RTC** alarm or timer
- **USART** wakeup (in Stop mode, requires `USARTx->CR1 |= USART_CR1_UESM`)

### Example (EXTI Wakeup from Stop):
```
EXTI->IMR |= EXTI_IMR_IM0;      // Enable interrupt for line 0
EXTI->RTSR |= EXTI_RTSR_TR0;    // Rising edge trigger
NVIC_EnableIRQ(EXTI0_IRQn);     // Enable in NVIC
```

---

## Conclusion

The STM32F411RE's low power modes — **Sleep**, **Stop**, and **Standby** — allow developers to design energy-efficient applications by reducing or halting power to various parts of the microcontroller. Choosing the appropriate mode and wake-up strategy helps maximize battery life and performance in embedded systems.