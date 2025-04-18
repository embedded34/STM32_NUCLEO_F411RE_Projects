# STM32F411RE RTC Peripheral Registers

This document outlines the configuration and usage of the RTC (Real-Time Clock) peripheral in the STM32F411RE microcontroller. The RTC provides a clock/calendar feature with programmable alarms and periodic wakeups, even during low-power modes.

---

## 1. Enabling Access to the RTC

The RTC peripheral is located in the **Backup Domain**, which requires enabling the **Power Interface Clock** and unlocking write access to backup domain registers.

### Steps:
- Enable power interface clock
- Enable access to backup domain
- Enable LSE or LSI as RTC clock source
- Enable RTC in RCC

### Example:
```
// Enable power interface clock
RCC->APB1ENR |= (1U << 28); // PWREN

// Enable access to backup domain
PWR->CR |= (1U << 8); // DBP

// Enable LSE (Low-Speed External oscillator)
RCC->BDCR |= (1U << 0);  // LSEON
while (!(RCC->BDCR & (1U << 1))); // Wait for LSERDY

// Select LSE as RTC clock source
RCC->BDCR &= ~(3U << 8);
RCC->BDCR |=  (1U << 8); // RTCSEL = 01 -> LSE

// Enable RTC
RCC->BDCR |= (1U << 15); // RTCEN
```

---

## 2. RTC Initialization Mode

Before modifying RTC registers, enter **initialization mode**.

### Example:
```
// Disable write protection for RTC registers
RTC->WPR = 0xCAU;
RTC->WPR = 0x53U;

// Enter init mode
RTC->ISR |= (1U << 7); // INIT
while (!(RTC->ISR & (1U << 6))); // Wait for INITF flag

// Configure RTC prescalers (assuming 32.768kHz LSE)
RTC->PRER = (127 << 16) | 255; // Asynchronous and synchronous prescalers

// Exit init mode
RTC->ISR &= ~(1U << 7);

// Enable write protection again
RTC->WPR = 0xFFU;
```

---

## 3. Setting Time and Date

After RTC is initialized, you can write time and date values.

### Example (Set Time to 12:30:45, Date to 18-Apr-2025):
```
// Disable write protection
RTC->WPR = 0xCAU;
RTC->WPR = 0x53U;

// Enter init mode (same as above)

// Set time (BCD format)
RTC->TR = (0x12 << 16) | (0x30 << 8) | 0x45;

// Set date: Friday (5), 2025-04-18
RTC->DR = (5 << 13) | (0x25 << 8) | (0x04 << 4) | 0x18;

// Exit init mode
RTC->ISR &= ~(1U << 7);

// Enable write protection
RTC->WPR = 0xFFU;
```

---

## 4. Reading Time and Date

Use `RTC->TR` and `RTC->DR` to read current time and date.

### Example:
```
uint32_t time = RTC->TR;
uint32_t date = RTC->DR;

uint8_t hours   = (time >> 16) & 0x3F;
uint8_t minutes = (time >> 8)  & 0x7F;
uint8_t seconds =  time        & 0x7F;

uint8_t year  = (date >> 8) & 0xFF;
uint8_t month = (date >> 4) & 0x1F;
uint8_t day   =  date       & 0x3F;
```

---

## 5. RTC Alarms

The RTC can generate interrupts using alarms (Alarm A or B).

### Steps:
- Disable Alarm
- Configure alarm time
- Enable alarm and interrupt
- Enable RTC alarm interrupt in NVIC

### Example (Alarm A at 12:31:00):
```c
// Disable Alarm A
RTC->CR &= ~(1U << 8);
while (!(RTC->ISR & (1U << 0))); // Wait for ALRAWF

// Set alarm time (BCD)
RTC->ALRMAR = (0x12 << 16) | (0x31 << 8); // Hours and Minutes, seconds = 00

// Enable alarm interrupt and alarm A
RTC->CR |= (1U << 12) | (1U << 8);

// Enable EXTI line for RTC Alarm
EXTI->IMR |= (1U << 17);
EXTI->RTSR |= (1U << 17);

// Enable RTC Alarm in NVIC
NVIC_EnableIRQ(RTC_Alarm_IRQn);
NVIC_SetPriority(RTC_Alarm_IRQn, 1);
```

### ISR:
```
void RTC_Alarm_IRQHandler(void) {
    if (RTC->ISR & (1U << 8)) {
        RTC->ISR &= ~(1U << 8); // Clear Alarm A flag
        EXTI->PR |= (1U << 17); // Clear EXTI line
        // Handle Alarm Event
    }
}
```

---

## 6. Low-Power Applications

RTC remains active during **Stop** and **Standby** modes if powered by the backup domain. This makes it ideal for wake-up timers or keeping time during low power.

### Tips:
- Use LSE for better accuracy
- Use RTC Wakeup timer for periodic interrupts
- Configure Alarm or Wakeup sources before entering low power mode

---

## Conclusion

The **RTC** peripheral on the **STM32F411RE** offers precise timekeeping and calendar functionalities with alarm and wakeup features. To use the RTC, enable the backup domain, configure the clock source (typically LSE), set prescalers, and use the `RTC_TR` and `RTC_DR` registers to set/read time and date. RTC is ideal for applications requiring time-based events, alarms, and low-power wake-ups.