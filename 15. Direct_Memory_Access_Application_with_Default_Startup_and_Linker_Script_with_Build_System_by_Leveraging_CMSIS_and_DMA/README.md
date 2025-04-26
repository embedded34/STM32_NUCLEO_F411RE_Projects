# STM32F411RE DMA Peripheral Registers

This document explains the usage of essential STM32F411RE DMA (Direct Memory Access) registers, which are used to configure and control DMA operations for efficient memory-to-memory or peripheral-to-memory data transfers without CPU intervention.

The **Direct Memory Access (DMA)** controller enables high-speed data transfer between memory and peripherals or between memory regions, significantly reducing CPU load and improving performance in data-intensive operations.

---

### Key Features:
- Supports **memory-to-memory**, **peripheral-to-memory**, and **memory-to-peripheral** transfers
- Up to **8 DMA streams** per controller, each with **multiple channels**
- **Circular**, **normal**, and **double buffer** modes supported
- Flexible **interrupt generation** and **priority levels**
- Works with various peripherals: USART, SPI, ADC, etc.

---

### Typical Use Case:
Used in scenarios requiring fast, efficient data transfer without burdening the CPU — such as reading ADC values, sending UART data, or transferring large memory blocks.

---

## 1. DMA_SxCR (Stream x Configuration Register)

Controls the configuration of each DMA stream (x = 0 to 7).

### Key Bits:
- **EN** (Bit 0): Enable stream  
- **DIR** (Bits 7–6): Data transfer direction  
  - `00`: Peripheral-to-memory  
  - `01`: Memory-to-peripheral  
  - `10`: Memory-to-memory  
- **CIRC** (Bit 8): Circular mode enable  
- **PL** (Bits 17–16): Priority level  
- **MINC**/**PINC** (Bits 10/9): Memory/Peripheral increment mode  

### Example (Memory-to-peripheral):
```
DMA2_Stream0->CR &= ~DMA_SxCR_EN; // Disable stream before any configuration
DMA2_Stream0->CR = (1 << 6) | (1 << 10) | (1 << 9); // DIR = 01, MINC = 1, PINC = 1
```

---

## 2. DMA_SxNDTR (Stream x Number of Data Register)

Specifies the number of data items to be transferred.

### Example:
```
DMA2_Stream0->NDTR = 256U; // Transfer 256 items
```

---

## 3. DMA_SxPAR (Stream x Peripheral Address Register)

Holds the address of the peripheral data register.

### Example:
```
DMA2_Stream0->PAR = (uint32_t)(&(USART2->DR)); // Set peripheral address
```

---

## 4. DMA_SxM0AR (Stream x Memory 0 Address Register)

Defines the base address of the memory buffer (Memory 0).

### Example:
```
DMA2_Stream0->M0AR = (uint32_t)txBuffer; // Set memory address
```

---

## 5. DMA_LISR / DMA_HISR (Low and High Interrupt Status Registers)

Report interrupt events for lower (0–3) and higher (4–7) streams respectively.

### Common Flags (per stream):
- **TCIFx**: Transfer complete  
- **HTIFx**: Half transfer  
- **TEIFx**: Transfer error

---

## 6. DMA_LIFCR / DMA_HIFCR (Low and High Interrupt Flag Clear Registers)

Used to clear corresponding interrupt flags.

### Example:
```
DMA2->LIFCR = DMA_LIFCR_CTCIF0; // Clear transfer complete flag for Stream 0
```

---

## 7. Enabling and Starting a DMA Transfer

After configuring the control, data size, peripheral, and memory address registers:

### Example:
```
// Disable stream before reconfiguring
DMA2_Stream0->CR &= ~DMA_SxCR_EN;
while (DMA2_Stream0->CR & DMA_SxCR_EN); // Wait until disabled

// Set peripheral/memory addresses and data size
DMA2_Stream0->PAR  = (uint32_t)&USART1->DR;
DMA2_Stream0->M0AR = (uint32_t)txBuffer;
DMA2_Stream0->NDTR = buffer_size;

// Enable DMA Stream
DMA2_Stream0->CR |= DMA_SxCR_EN;
```

---

## Conclusion

To use the **DMA peripheral** on the **STM32F411RE**, configure the stream control register (`DMA_SxCR`) with appropriate settings, set the source and destination addresses (`DMA_SxPAR`, `DMA_SxM0AR`), the number of data items (`DMA_SxNDTR`), and enable the stream. DMA improves system performance by offloading repetitive memory/peripheral transfer tasks, allowing the CPU to focus on other operations.