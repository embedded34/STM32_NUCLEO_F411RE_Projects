#include <stdint.h>
#include "adc_dma.h"

// Macro to enable the clock for GPIOA (bit 0 in RCC_AHB1ENR)
#define GPIOAEN (1U << 0)

// Macro to enable the clock for ADC1 (bit 8 in RCC_APB2ENR)
#define ADC1EN (1U << 8)

// Macro to enable the scan mode (bit 8 in ADC_CR1)
#define CR1_SCAN (1U << 8)

// Macro to enable the ADC module (bit 0 in ADC_CR2)
#define CR2_ADCON (1U << 0)

// Macro to enable continuous conversion mode (bit 1 in ADC_CR2)
#define CR2_CONT (1U << 1)

// Macro to enable direct memory access mode for single ADC mode to transfer data automatically (bit 8 in ADC_CR2)
#define CR2_DMA (1U << 8)

// Macro to issue direct memory access requests as long as data are converted and DMA=1 (bit 9 in ADC_CR2)
#define CR2_DDS (1U << 9)

// Macro to start the ADC conversion of regular channels (bit 30 in ADC_CR2)
#define CR2_SWSTART (1U << 30)

// Macro to enable clock for DMA2 controller (bit 22 in RCC_AHB1ENR register)
#define DMA2EN (1U << 22)

// Macro to enable the DMA stream (bit 0 in DMA_SxCR register)
#define DMA_SCR_EN (1U << 0)

// Macro to enable memory increment mode (increments memory address pointer after each data transfer) (bit 10 in DMA_SxCR register)
#define DMA_SCR_MINC (1U << 10)

// Macro to enable peripheral increment mode (increments peripheral address pointer after each data transfer) (bit 9 in DMA_SxCR register)
#define DMA_SCR_PINC (1U << 9)

// Macro to enable DMA circular mode (bit 8 in DMA_SxCR register)
#define DMA_SCR_CIRC (1U << 8)

// Macro to enable transfer complete interrupt (bit 4 in DMA_SxCR register)
#define DMA_SCR_TCIE (1U << 4)

// Macro to enable transfer error interrupt (bit 2 in DMA_SxCR register)
#define DMA_SCR_TEIE (1U << 2)

// Macro to disable DMA direct mode (bit 2 in DMA_SxFCR register)
#define DMA_SFCR_DMDIS (1U << 2)

// Array to store the raw ADC1 data
uint16_t adc1_raw_data[NUM_OF_CHANNELS];

void adc1_dma2_init(void) {
    /************GPIOA Configuration**********/
	// Enable the clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Configure GPIOA Pin 0 and Pin 1 to operate in analog input mode (MODERx[1:0] = 11)
    GPIOA->MODER |= (1U << 0);
    GPIOA->MODER |= (1U << 1);
    GPIOA->MODER |= (1U << 2);
    GPIOA->MODER |= (1U << 3);

    /************ADC1 Configuration**********/
    // Enable clock access to the ADC1 module
    RCC->APB2ENR |= ADC1EN;

    // Set conversion sequence length to 2, meaning only two channels will be converted
    ADC1->SQR1 |= (1U << 20);
    ADC1->SQR1 &= ~(1U << 21);
    ADC1->SQR1 &= ~(1U << 22);
    ADC1->SQR1 &= ~(1U << 23);

    // Set channel 0 as the 1st conversion and channel 1 as the 2nd in the regular sequence
    ADC1->SQR3 = (0U << 0) | (1U << 5);

    // Enable scan mode to convert multiple channels sequentially
    ADC1->CR1 |= CR1_SCAN;

    // Enable continuous conversion mode, DMA mode, and DMA request after last transfer
    ADC1->CR2 |= CR2_CONT | CR2_DMA | CR2_DDS;

    /************DMA2 Configuration**********/
    // Enable the clock access to DMA2
    RCC->AHB1ENR |= DMA2EN;

    // Disable DMA2 stream before making any configurations
    DMA2_Stream0->CR &= ~DMA_SCR_EN;

    // Wait until DMA2 Stream0 is disabled before configuring it
    while ((DMA2_Stream0->CR & (DMA_SCR_EN))) {
    }

    // Enable circular mode for continuous data transfer
    DMA2_Stream0->CR |= DMA_SCR_CIRC;

    // Set memory data size to half-word (16-bit)
    DMA2_Stream0->CR |= (1U << 13);
    DMA2_Stream0->CR &= ~(1U << 14);

    // Set peripheral data size to half-word (16-bit)
    DMA2_Stream0->CR |= (1U << 11);
    DMA2_Stream0->CR &= ~(1U << 12);

    // Enable memory address increment mode
    DMA2_Stream0->CR |= DMA_SCR_MINC;

    // Set peripheral address to the ADC1 data register
    DMA2_Stream0->PAR = (uint32_t)(&(ADC1->DR));

    // Set memory address to the adc1_raw_data array
    DMA2_Stream0->M0AR = (uint32_t)(&adc1_raw_data);

    // Set the number of data items to transfer
    DMA2_Stream0->NDTR = (uint16_t)NUM_OF_CHANNELS;

    // Enable DMA2 stream
    DMA2_Stream0->CR |= DMA_SCR_EN;

    /************ADC1 Configuration**********/
    // Enable ADC1 module
    ADC1->CR2 |= CR2_ADCON;

    // Start ADC1 conversion
    ADC1->CR2 |= CR2_SWSTART;
}
