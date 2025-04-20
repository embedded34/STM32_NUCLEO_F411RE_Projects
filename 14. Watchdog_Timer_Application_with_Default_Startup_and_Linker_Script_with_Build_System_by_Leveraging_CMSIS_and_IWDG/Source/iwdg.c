#include <stdint.h>
#include "iwdg.h"

// Macro to define the key that enables the IWDG
#define IWDG_KEY_ENABLE (0x0000CCCCU)

// Macro to define the key that enables write access to IWDG registers
#define IWDG_KEY_WR_ACCESS_ENABLE (0x00005555U)

// Macro to define the prescaler value for the IWDG clock
#define IWDG_PRESCALER_4 (0x00000000U)

// Macro to set the reload value to the maximum, 0xFFF, providing the longest timeout period
#define IWDG_RELOAD_VAL (0xFFFU)

static uint8_t is_iwdg_ready(void);

void iwdg_init(void) {
    // Enable the IWDG by writing 0xCCCC in the key register
    IWDG->KR = IWDG_KEY_ENABLE;

    // Enable write access to the prescaler and reload registers by writing 0x5555 in the key register
    IWDG->KR = IWDG_KEY_WR_ACCESS_ENABLE;

    // Set the prescaler register to divide the LSI clock by 4
    IWDG->PR = IWDG_PRESCALER_4;

    // Set the reload register to the largest value 0xFFF to get the longest possible timeout period
    IWDG->RLR = IWDG_RELOAD_VAL;

    // Wait until the IWDG registers are updated and ready to use
    while (is_iwdg_ready() != 1) {
    }

    // Reload the counter and prevent the IWDG from resetting the system by writing 0xAAAA in the key register
    IWDG->KR = IWDG_KEY_RELOAD;
}

static uint8_t is_iwdg_ready(void) {
	return ((READ_BIT(IWDG->SR, IWDG_SR_PVU | IWDG_SR_RVU) == 0U) ? (uint8_t)1U : (uint8_t)0U);
}
