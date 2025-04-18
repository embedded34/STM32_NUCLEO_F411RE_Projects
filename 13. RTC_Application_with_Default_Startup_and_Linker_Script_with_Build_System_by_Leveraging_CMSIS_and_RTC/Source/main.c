/**
 * This code demonstrates how to initialize UART2 for debugging, configure
 * the RTC (Real-Time Clock), and continuously display the current time and
 * date using the CMSIS framework on an STM32F4 microcontroller.
 */
#include <stdio.h>
#include "uart.h"
#include "rtc.h"

static void display_rtc_calendar(void);

// The length of the buffer for storing the time and date string
#define BUFF_LEN 16

// Arrays that hold the formatted time and date strings
uint8_t time_buff[BUFF_LEN] = {0};
uint8_t date_buff[BUFF_LEN] = {0};

/**
 * Main function: Initializes UART2 and RTC, then continuously displays time and date.
 */
int main(void) {
    // Initialize UART 2 peripheral for debugging
    uart2_init();

    // Initialize RTC peripheral
    rtc_init();

    while (1) {
    	// Display the current time and date
    	display_rtc_calendar();
    }

    return 0;
}

static void display_rtc_calendar(void) {
    // Time display format --> HH:MM:SS
	sprintf((char *)time_buff, "%02d:%02d:%02d",
	         rtc_time_get_current_hour(),
	         rtc_time_get_current_minute(),
	         rtc_time_get_current_second());

    // Date display format --> YYYY-MM-DD
    sprintf((char *)date_buff, "20%02d-%02d-%02d",
             rtc_date_get_current_year(),
             rtc_date_get_current_month(),
             rtc_date_get_current_day());

    // Print compact combined calendar format --> YYYY-MM-DD HH:MM:SS
    printf("%s %s\n\r", date_buff, time_buff);
}
