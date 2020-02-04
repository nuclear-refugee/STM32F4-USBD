#include "usb_gpio.h"

int usbd_init(void) {
    /*
    PA11:OTG_FS_DM    PA12:OTG_FS_DP
    GPIO_SPEED:50mhz
    GPIO_MODE:AF
    GPIO_OTYPE:PUSH PULL
    GPIO_PUPD:NOPUPD
    */
   GPIO_MODER(GPIOA_BASE) |= 2<<22 | 2<<24;
   GPIO_OTYPER(GPIOA_BASE) |= 0<<11 | 0<<12;
   GPIO_OSPEEDR(GPIOA_BASE) |= 1<<22 | 1<24;
   GPIO_PUPDR(GPIOA_BASE) |= 0<<22 | 0<<24;
   GPIO_AFRH(GPIOA_BASE) |= 10<<12 | 10<<16;
   /*
    PA10:OTG_FS_ID
    GPIO_SPEED:50mhz
    GPIO_MODE:AF
    GPIO_OTYPE:OPEN DRAIN
    GPIO_PUPD:PU
   */
   GPIO_MODER(GPIOA_BASE) |= 2 << 20;
   GPIO_OTYPER(GPIOA_BASE) |= 1 << 10;
   GPIO_OSPEEDR(GPIOA_BASE) |= 1 << 20;
   GPIO_PUPDR(GPIOA_BASE) |= 1 << 20;
   GPIO_AFRH(GPIOA_BASE) |= 10 << 8;
   /*
    APB2 SYSCFG clock cn
    AHB2 OTG_FS clock en
   */
    RCC_APB2_ENR |= 1 << 14;
    RCC_AHB2_ENR |= 1 << 7;
    /*
    EXTI LINE 18 USB OTG WAKEUP
    */
    EXTI_RTSR |= 1 << 18;
    EXTI_FTSR |= 0 << 18;
    /*
    NVIC CHannel = 42
    */
}