# STM32F411RE Timer 2 Peripheral Registers

This document explains the usage of essential STM32F411RE Timer 2 (TIM2) registers, which are used to configure and control the Timer 2 peripheral for generating time delays, PWM signals, and other timer-related functions.

## 1. RCC_APB1ENR (APB1 Peripheral Clock Enable Register)
The `RCC_APB1ENR` register enables or disables the clocks for peripherals on the APB1 bus, including Timer 2.

### Register Address:
- **RCC_APB1ENR**: `0x40023800 + 0x40`

### Usage:
To enable the clock for Timer 2, modify the corresponding bit in the `RCC_APB1ENR` register.

### Example:
```
// Enable Timer 2 clock
RCC->APB1ENR |= (1U << 0);
```

## 2. TIM2_CR1 (Control Register 1)
The `TIM2_CR1` register is used to configure the operation of Timer 2, including enabling the timer and setting its direction.

### Register Address:
- **TIM2_CR1**: `0x40000000 + 0x00`

### Usage:
To enable Timer 2, set the `CEN` bit in the `TIM2_CR1` register.

### Example:
```
// Enable Timer 2
TIM2->CR1 |= (1U << 0); // Set CEN bit to 1
```

## 3. TIM2_PSC (Prescaler Register)
The `TIM2_PSC` register sets the prescaler value, which divides the system clock to generate the timer's clock.

### Register Address:
- **TIM2_PSC**: `0x40000000 + 0x28`

### Usage:
To configure the prescaler value for Timer 2, set the value of the `TIM2_PSC` register.

### Example:
```
// Set Timer 2 prescaler to divide the system clock by 1000
TIM2->PSC = 1000 - 1;
```

## 4. TIM2_ARR (Auto-Reload Register)
The `TIM2_ARR` register is used to set the period for Timer 2, determining how often the timer overflows and triggers an interrupt.

### Register Address:
- **TIM2_ARR**: `0x40000000 + 0x2C`

### Usage:
To configure the auto-reload value for Timer 2, set the value of the `TIM2_ARR` register.

### Example:
```
// Set Timer 2 period to 10000 ticks
TIM2->ARR = 10000 - 1;
```

## 5. TIM2_SR (Status Register)
The `TIM2_SR` register provides the status flags for Timer 2, such as whether the timer overflowed or if an interrupt is pending.

### Register Address:
- **TIM2_SR**: `0x40000000 + 0x10`

### Usage:
To check if the timer has overflowed, check the `UIF` (Update Interrupt Flag) bit in the `TIM2_SR` register.

### Example:
```
// Check if Timer 2 has overflowed
if (TIM2->SR & (1U << 0)) {
    // Timer 2 overflowed
    TIM2->SR &= ~(1U << 0); // Clear UIF flag
}
```

## 6. TIM2_EGR (Event Generation Register)
The `TIM2_EGR` register is used to trigger events, such as generating an update event (UEV) to reload the timer's registers.

### Register Address:
- **TIM2_EGR**: `0x40000000 + 0x14`

### Usage:
To generate an update event (which reloads the `ARR` and `PSC` values), set the `UG` bit in the `TIM2_EGR` register.

### Example:
```
// Generate an update event for Timer 2
TIM2->EGR |= (1U << 0); // Set UG bit to 1
```

## Conclusion
The STM32F411RE Timer 2 peripheral provides powerful timing and PWM capabilities. By manipulating the `RCC_APB1ENR`, `TIM2_CR1`, `TIM2_PSC`, `TIM2_ARR`, `TIM2_SR`, and `TIM2_EGR` registers, you can configure the timer for various applications such as generating precise time delays, PWM signals, and handling interrupts. By using these registers effectively, you can control Timer 2 in an efficient and flexible way.
