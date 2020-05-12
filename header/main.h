#ifndef __MAIN_H_
#define __MAIN_H_
#include "reg.h"
#include "stdint.h"
#include "stdlib.h"
#include "uart.h"
#include "../usb/header/usb_hal.h"
#define HSE_CLK 8000000UL

void init_core_clk();
int main();
#endif