#ifndef __UART_PRINT_H
#define __UART_PRINT_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"

#define high_bits(x) (x >> 4 % 16)
#define low_bits(x)  (x & 15 % 16)

int uart_print(const char *data, ...);

#endif