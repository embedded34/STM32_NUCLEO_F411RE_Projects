#include "rtc.h"

// Macro to enable the clock for PWR (power controller) (bit 28 in RCC_APB1ENR)
#define PWREN (1U << 28)

// Macro to enable write access to RTC and RTC backup registers (bit 8 in PWR_CR)
#define CR_DBP (1U << 8)

// Macro to enable internal low-speed oscillator (LSI) (bit 0 in RCC_CSR)
#define CSR_LSION (1U << 0)

// Macro to check whether the LSI is stable and ready to be used (bit 1 in RCC_CSR)
#define CSR_LSIRDY (1U << 1)

// Macro to force a software reset of the entire backup domain (bit 16 in RCC_BDCR)
#define BDCR_BDRST (1U << 16)

// Macro to enable RTC clock (bit 15 in RCC_BDCR)
#define BDCR_RTCEN (1U << 15)

// Macro to disable write protection on the RTC registers (the first key)
#define RTC_WRITE_PROTECTION_KEY_1 ((uint8_t)0xCAU)

// Macro to disable write protection on the RTC registers (the second key)
#define RTC_WRITE_PROTECTION_KEY_2 ((uint8_t)0x53U)

// Macro to enter initialization mode in the RTC peripheral (bit 7 in RTC_ISR)
#define ISR_INIT (1U << 7)

// Macro to indicate that the RTC peripheral is in initialization mode (bit 6 in RTC_ISR)
#define ISR_INITF (1U << 6)

// Macro to set the time format to PM in the 12-hour format
#define TIME_FORMAT_PM (1U << 22)

// Macro to set the hour format to 12-hour format (bit 6 in RTC_CR)
#define CR_FMT (1U << 6)

// Macro to check whether calendar shadow registers synchronized (bit 5 in RTC_ISR)
#define ISR_RSF (1U << 5)

// Macro to set the asynchronous prescaler for the RTC peripheral
#define RTC_ASYNCH_PREDIV ((uint32_t)0x007F) // 127

// Macro to set the synchronous prescaler for the RTC peripheral
#define RTC_SYNCH_PREDIV ((uint32_t)0x00F9)  // 249

static void rtc_set_asynchronous_prescaler(uint32_t Asynch_Prescaler);
static void rtc_set_synchronous_prescaler(uint32_t Synch_Prescaler);
static uint8_t rtc_initialization_sequence_enter(void);
static uint8_t wait_for_rtc_synchronization(void);
static uint8_t rtc_initialization_sequence_exit(void);
static void rtc_date_config(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year);
static void rtc_time_config(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds);

static void rtc_set_asynchronous_prescaler(uint32_t Asynch_Prescaler) {
    MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_A, Asynch_Prescaler << RTC_PRER_PREDIV_A_Pos);
}

static void rtc_set_synchronous_prescaler(uint32_t Synch_Prescaler) {
    MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_S, Synch_Prescaler << RTC_PRER_PREDIV_S_Pos);
}

static uint8_t rtc_initialization_sequence_enter(void) {
    // Put the RTC peripheral into initialization mode
	rtc_enable_initialization_mode();

    // Wait until the RTC peripheral is in initialization mode
    while (is_rtc_initialization_mode() != 1) {
    }

    return 1;
}

static uint8_t wait_for_rtc_synchronization(void) {
    // Clear the registers synchronization flag (RSF)
    RTC->ISR &= ~ISR_RSF;

    // Wait until the RTC registers are synchronized
    while (is_rtc_synchronized() != 1) {
    }

    return 1;
}

static uint8_t rtc_initialization_sequence_exit(void) {
    // Stop the RTC peripheral initialization mode
	rtc_disable_initialization_mode();

	// Wait until the RTC registers are synchronized
    return (wait_for_rtc_synchronization());
}

static void rtc_date_config(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year) {
	// A temporary variable to hold the date value
    register uint32_t temp = 0U;

    // Combine date components that are week day, day, month and year
    temp = (WeekDay << RTC_DR_WDU_Pos) |
           (((Year & 0xF0U) << (RTC_DR_YT_Pos - 4U)) | ((Year & 0x0FU) << RTC_DR_YU_Pos)) |
           (((Month & 0xF0U) << (RTC_DR_MT_Pos - 4U)) | ((Month & 0x0FU) << RTC_DR_MU_Pos)) |
           (((Day & 0xF0U) << (RTC_DR_DT_Pos - 4U)) | ((Day & 0x0FU) << RTC_DR_DU_Pos));

    // Update the RTC’s time register with the newly constructed date value
    MODIFY_REG(RTC->DR, (RTC_DR_WDU | RTC_DR_MT | RTC_DR_MU | RTC_DR_DT | RTC_DR_DU | RTC_DR_YT | RTC_DR_YU), temp);
}

static void rtc_time_config(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds) {
	// A temporary variable to hold the time value
    register uint32_t temp = 0U;

    // Combine time components that are time format, hours, minutes and seconds
    temp = Format12_24 | (((Hours & 0xF0U) << (RTC_TR_HT_Pos - 4U)) | ((Hours & 0x0FU) << RTC_TR_HU_Pos)) |
           (((Minutes & 0xF0U) << (RTC_TR_MNT_Pos - 4U)) | ((Minutes & 0x0FU) << RTC_TR_MNU_Pos)) |
           (((Seconds & 0xF0U) << (RTC_TR_ST_Pos - 4U)) | ((Seconds & 0x0FU) << RTC_TR_SU_Pos));

    // Update the RTC’s time register with the newly constructed time value
    MODIFY_REG(RTC->TR, (RTC_TR_PM | RTC_TR_HT | RTC_TR_HU | RTC_TR_MNT | RTC_TR_MNU | RTC_TR_ST | RTC_TR_SU), temp);
}

void rtc_enable_initialization_mode(void) {
	RTC->ISR |= ISR_INIT;
}

void rtc_disable_initialization_mode(void) {
	RTC->ISR &= ~ISR_INIT;
}

uint8_t is_rtc_initialization_mode(void) {
	return ((RTC->ISR & (ISR_INITF)) == ISR_INITF);
}

uint8_t is_rtc_synchronized(void) {
	return ((RTC->ISR & (ISR_RSF)) == ISR_RSF);
}

void rtc_init(void) {
	// Enable the clock access to PWR
    RCC->APB1ENR |= PWREN;

    // Enable write access to RTC and RTC backup registers
    PWR->CR |= CR_DBP;

    // Enable the low-speed internal oscillator (LSI)
    RCC->CSR |= CSR_LSION;

    // Wait until the LSI oscillator is stable and ready to use
    while ((RCC->CSR & (CSR_LSIRDY)) != CSR_LSIRDY) {
    }

    // Force a software reset of the backup domain to ensure a clean configuration
    RCC->BDCR |= BDCR_BDRST;

    // Release the reset to allow the backup domain to function normally
    RCC->BDCR &= ~BDCR_BDRST;

    // Set the clock source for the RTC peripheral to LSI
    RCC->BDCR &= ~(1U << 8);
    RCC->BDCR |= (1U << 9);

    // Enable the RTC peripheral
    RCC->BDCR |= BDCR_RTCEN;

    // Disable the write protection of RTC registers
    RTC->WPR = RTC_WRITE_PROTECTION_KEY_1;
    RTC->WPR = RTC_WRITE_PROTECTION_KEY_2;

    // Enter the initialization mode
    if (rtc_initialization_sequence_enter() != 1) {
        // Handle initialization failure if necessary
    }

    // Configure the RTC peripheral to the desired date: Friday, April 18th, 2025
    // Format --> Weekday, Day, Month, Year
    rtc_date_config(0x05U, 0x18U, 0x04U, 0x25U);

    // Configure the RTC peripheral to the desired time: 05:59:55 PM
    // Format --> Time Format, Hour, Minute, Second
    rtc_time_config(TIME_FORMAT_PM, 0x05U, 0x59U, 0x55U);

    // Configure the RTC peripheral so that it uses a 12-hour format
    RTC->CR |= CR_FMT;

    // Set the asynchronous and synchronous prescaler values
    rtc_set_asynchronous_prescaler(RTC_ASYNCH_PREDIV);
    rtc_set_synchronous_prescaler(RTC_SYNCH_PREDIV);

    // Exit the initialization mode
    rtc_initialization_sequence_exit();

    // Re-enable write protection on the RTC registers to prevent accidental changes
    RTC->WPR = (uint8_t)0xFF;
}

uint8_t rtc_convert_dec2bcd(uint8_t decimal_value) {
	return (uint8_t)((((decimal_value) / 10U) << 4U) | ((decimal_value) % 10U));
}

uint8_t rtc_convert_bcd2dec(uint8_t bcd_value) {
    return (uint8_t)(((uint8_t)((bcd_value) & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U + ((bcd_value) & (uint8_t)0x0FU));
}

uint8_t rtc_date_get_current_day(void) {
	uint32_t bcd_day = (READ_BIT(RTC->DR, RTC_DR_DT | RTC_DR_DU)) >> RTC_DR_DU_Pos;
    return rtc_convert_bcd2dec((uint8_t)bcd_day);
}

uint8_t rtc_date_get_current_month(void) {
	uint32_t bcd_month = (READ_BIT(RTC->DR, RTC_DR_MT | RTC_DR_MU)) >> RTC_DR_MU_Pos;
    return rtc_convert_bcd2dec((uint8_t)bcd_month);
}

uint8_t rtc_date_get_current_year(void) {
	uint32_t bcd_year = (READ_BIT(RTC->DR, RTC_DR_YT | RTC_DR_YU)) >> RTC_DR_YU_Pos;
    return rtc_convert_bcd2dec((uint8_t)bcd_year);
}

uint8_t rtc_time_get_current_second(void) {
	uint32_t bcd_second = (READ_BIT(RTC->TR, RTC_TR_ST | RTC_TR_SU)) >> RTC_TR_SU_Pos;
    return rtc_convert_bcd2dec((uint8_t)bcd_second);
}

uint8_t rtc_time_get_current_minute(void) {
	uint32_t bcd_minute = (READ_BIT(RTC->TR, RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos;
    return rtc_convert_bcd2dec((uint8_t)bcd_minute);
}

uint8_t rtc_time_get_current_hour(void) {
    uint32_t bcd_hour = (READ_BIT(RTC->TR, (RTC_TR_HT | RTC_TR_HU))) >> RTC_TR_HU_Pos;
    uint32_t hour = rtc_convert_bcd2dec((uint8_t)bcd_hour);

    if (RTC->CR & CR_FMT) {
        // 12-hour mode: check PM bit
        if (RTC->TR & TIME_FORMAT_PM) {
            if (hour == 12) {
                return 12;  // 12 PM should remain 12
            }
            return (hour + 12);
        } else {
            if (hour == 12) {
                return 0;  // 12 AM should be 00 (midnight)
            }
            return hour;
        }
    } else {
        // 24-hour mode: return as is
        return hour;
    }
}
