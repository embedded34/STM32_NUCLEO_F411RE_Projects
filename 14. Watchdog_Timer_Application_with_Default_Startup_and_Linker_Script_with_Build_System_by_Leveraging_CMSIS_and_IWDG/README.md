# STM32F411RE IWDG Peripheral Registers

This document explains the usage of essential STM32F411RE IWDG (Independent Watchdog) registers, which are used to configure and control the IWDG peripheral to ensure system reliability by resetting the system in case of software malfunctions.

The **Independent Watchdog (IWDG)** is a separate timer clocked by its own low-speed internal oscillator (LSI), running independently from the main system clocks. Once started, it cannot be stopped except by a system reset.

### Key Features:
- Runs from LSI (~32 kHz)
- Once enabled, **cannot be disabled** except by a reset
- Requires periodic refresh to prevent a system reset

### Typical Use Case:
Used in fault-tolerant applications to reset the MCU if the software hangs or enters an unexpected state.

---

## 1. IWDG_KR (Key Register)

This register is used to unlock the IWDG for configuration, start it, and refresh the counter.

### Register Address:
- **IWDG_KR**: `0x40003000 + 0x00`

### Usage:
- Write `0x5555` to unlock access to PR and RLR
- Write `0xCCCC` to start the watchdog
- Write `0xAAAA` to refresh (reload) the counter

### Example:
```
// Enable write access to PR and RLR
IWDG->KR = 0x5555U;

// Start the IWDG
IWDG->KR = 0xCCCCU;
```

---

## 2. IWDG_PR (Prescaler Register)

Sets the prescaler divider for the LSI clock to define the watchdog timeout period.

### Register Address:
- **IWDG_PR**: `0x40003000 + 0x04`

### Usage:
Select prescaler divider from 4 to 256.

### Prescaler Values:
- `000`: /4  
- `001`: /8  
- `010`: /16  
- `011`: /32  
- `100`: /64  
- `101`: /128  
- `110` or `111`: /256

### Example (Prescaler = /32):
```
// Set prescaler to divide by 32
IWDG->PR = 0x03; // PR[2:0] = 011
```

---

## 3. IWDG_RLR (Reload Register)

Defines the value loaded into the watchdog counter. The timeout period is a function of the `RL` value and the clock prescaler.

### Register Address:
- **IWDG_RLR**: `0x40003000 + 0x08`

### Usage:
Set the counter reload value (maximum 0xFFF).

### Example (RLR = 1000):
```
// Set reload value
IWDG->RLR = 1000;
```

---

## 4. IWDG_SR (Status Register)

Indicates the status of the IWDG registers (whether PR and RLR updates are ongoing).

### Register Address:
- **IWDG_SR**: `0x40003000 + 0x0C`

### Usage:
Check if PR/RLR updates are completed (wait for 0).

### Bits:
- **PVU** (Bit 0): Prescaler Value Update
- **RVU** (Bit 1): Reload Value Update

### Example:
```
// Wait for PR and RLR updates to complete
while (IWDG->SR != 0);
```

---

## 5. Refresh the IWDG Counter

Once started, the IWDG must be refreshed periodically to prevent a system reset. Refresh by writing `0xAAAA` to `IWDG_KR`.

### Example:
```
// Refresh IWDG to prevent reset
IWDG->KR = 0xAAAA;
```

---

## Conclusion

To use the **IWDG** on the **STM32F411RE**, start by enabling the **LSI** oscillator. Then, unlock the watchdog registers using `IWDG_KR`, configure the prescaler and reload value using `IWDG_PR` and `IWDG_RLR`, and start the watchdog with `0xCCCC`. The software must then regularly write `0xAAAA` to refresh the counter, otherwise the MCU will reset. This mechanism provides a vital safety net against software faults and system hangs.