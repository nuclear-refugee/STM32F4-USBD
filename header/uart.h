#ifndef __UART_H_
#define __UART_H_
#include "main.h"
#include "reg.h"


#define BAUDRATE(x) (HSE_CLK / x)

#define APB1_UART2 (17)
#define APB1_UART3 (18)
#define APB1_UART4 (19)
#define APB1_UART5 (20)

void uart_init();
void uart_put_c(uint8_t data);
void uart_get_c(uint8_t* data);
#endif
