#include "..\header\main.h"
int main(){
   
    
    uart_init();

    int  data=1;
    char cc='e';
    char *str="abcde";
    RCC_AHB1_ENR |= 1<<3; //enable gpiod
    GPIO_MODER(GPIOD_BASE) |= 1<<24; // pd 12~15
    GPIO_OTYPER(GPIOD_BASE) = 0;
    GPIO_OSPEEDR(GPIOD_BASE) = 1<<24;
    GPIO_PUPDR(GPIOD_BASE) = 0;
    GPIO_ODR(GPIOD_BASE) |= 1<<12;

    while (1)
    {
    uart_print("data=%d",data);
    uart_print("char=%c",cc);
    uart_print("string=%s",str);
    uart_print("hex=%x",data);
    uart_print("data=%d char=%c string=%s hex=%x",data,cc,str,data);
    data=(data+1)%20;
    GPIO_ODR(GPIOD_BASE) ^= 1<<12;
    for (int i=0;i<1000000;i++) {;}
    GPIO_ODR(GPIOD_BASE) ^= 1<<12;
    for (int i=0;i<1000000;i++) {;}

    }
    

}

