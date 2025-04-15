#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"

/* Function Declarations */
void uart2_init(void);
void uart_write_character(int ch);
void uart_write_formatted_string(const char *format, ...);
int __io_putchar(int ch);

#endif /* INCLUDE_UART_H_ */
