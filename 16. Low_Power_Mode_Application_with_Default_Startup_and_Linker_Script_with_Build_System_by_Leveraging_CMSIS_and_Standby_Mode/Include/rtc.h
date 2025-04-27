#ifndef INCLUDE_RTC_H_
#define INCLUDE_RTC_H_

#include <stdint.h>
#include "stm32f4xx.h"

/* Function Declarations */
void rtc_enable_initialization_mode(void);
void rtc_disable_initialization_mode(void);
uint8_t is_rtc_initialization_mode(void);
uint8_t is_rtc_synchronized(void);
void rtc_init(void);
uint8_t rtc_convert_dec2bcd(uint8_t decimal_value);
uint8_t rtc_convert_bcd2dec(uint8_t bcd_value);
uint8_t rtc_date_get_current_day(void);
uint8_t rtc_date_get_current_month(void);
uint8_t rtc_date_get_current_year(void);
uint8_t rtc_time_get_current_second(void);
uint8_t rtc_time_get_current_minute(void);
uint8_t rtc_time_get_current_hour(void);

#endif /* INCLUDE_RTC_H_ */
