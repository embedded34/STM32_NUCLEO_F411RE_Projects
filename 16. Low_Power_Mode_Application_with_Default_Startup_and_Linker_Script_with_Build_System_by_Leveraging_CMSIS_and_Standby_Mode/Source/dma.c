#include "dma.h"

// Macro to enable clock for DMA2 controller (bit 22 in RCC_AHB1ENR register)
#define DMA2EN (1U << 22)

// Macro to enable the DMA stream (bit 0 in DMA_SxCR register)
#define DMA_SCR_EN (1U << 0)

// Macro to enable memory increment mode (increments memory address pointer after each data transfer) (bit 10 in DMA_SxCR register)
#define DMA_SCR_MINC (1U << 10)

// Macro to enable peripheral increment mode (increments peripheral address pointer after each data transfer) (bit 9 in DMA_SxCR register)
#define DMA_SCR_PINC (1U << 9)

// Macro to enable transfer complete interrupt (bit 4 in DMA_SxCR register)
#define DMA_SCR_TCIE (1U << 4)

// Macro to enable transfer error interrupt (bit 2 in DMA_SxCR register)
#define DMA_SCR_TEIE (1U << 2)

// Macro to disable DMA direct mode (bit 2 in DMA_SxFCR register)
#define DMA_SFCR_DMDIS (1U << 2)

void dma2_stream0_mem2mem_config(void) {
	// Enable the clock access to DMA2
    RCC->AHB1ENR |= DMA2EN;

    // Disable DMA2 stream before making any configurations
    DMA2_Stream0->CR &= ~DMA_SCR_EN;

    // Wait until DMA2 Stream0 is disabled before configuring it
    while ((DMA2_Stream0->CR & (DMA_SCR_EN))) {
    }

    // Set memory data size to half-word (16-bit)
    DMA2_Stream0->CR |= (1U << 13);
    DMA2_Stream0->CR &= ~(1U << 14);

    // Set peripheral data size to half-word (16-bit)
    DMA2_Stream0->CR |= (1U << 11);
    DMA2_Stream0->CR &= ~(1U << 12);

    // Enable memory address increment mode
    DMA2_Stream0->CR |= DMA_SCR_MINC;

    // Enable peripheral address increment mode
    DMA2_Stream0->CR |= DMA_SCR_PINC;

    // Set data transfer direction as memory-to-memory
    DMA2_Stream0->CR &= ~(1U << 6);
    DMA2_Stream0->CR |= (1U << 7);

    // Enable the transfer complete interrupt
    DMA2_Stream0->CR |= DMA_SCR_TCIE;

    // Enable the transfer error interrupt
    DMA2_Stream0->CR |= DMA_SCR_TEIE;

    // Disable direct mode to use FIFO mode
    DMA2_Stream0->FCR |= DMA_SFCR_DMDIS;

    // Set DMA2 FIFO threshold to full FIFO, i.e. transfer only when FIFO is completely full
    DMA2_Stream0->FCR |= (1U << 0);
    DMA2_Stream0->FCR |= (1U << 1);

    // Enable the DMA2_Stream0 interrupt line in the NVIC to handle DMA2-related interrupts
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void dma2_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len) {
	// Set peripheral address to the value passed in src_buff
    DMA2_Stream0->PAR = src_buff;

    // Set memory address to the value passed in dest_buff
    DMA2_Stream0->M0AR = dest_buff;

    // Set the number of data items to transfer
    DMA2_Stream0->NDTR = len;

    // Enable DMA2 stream to start the data transfer from the source to the destination
    DMA2_Stream0->CR |= DMA_SCR_EN;
}
