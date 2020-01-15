#include <stdint.h>
#include "../header/reg.h"
extern uint32_t _endtext;
extern uint32_t _startdata;
extern uint32_t _enddata;
extern uint32_t _startbss;
extern uint32_t _endbss;
extern uint32_t _estack;
void init_core_clk(void){
    RCC_CR |= PLL_ON|(1<<16)|(1<<19);
    while((!(RCC_CR&(1<<17)))||(!(RCC_CR&(1<<25))));
    RCC_PLLCFGR =(1<<22)|(PLLM(5)) | (PLLN(60)) | (PLLQ(2))|(PLLP(2));
    RCC_CFGR = 1;
}

extern int main(void);
void reset_handler(void) {
    uint32_t* end_text = &_endtext;
    uint32_t* start_data = &_startdata;
    uint32_t* end_data = &_enddata;
    while(start_data<end_data) *start_data++=*end_text++;
    uint32_t* start_bss = &_startbss;
    uint32_t* endbss = &_endbss;
    while(start_bss<endbss)*start_bss++=0;
    init_core_clk();
    main();
}
void nmi_handler (void){
    while(1);
}
void hard_fault_handler(void){
    while(1);
}
void mem_manage_handler(void){
    while(1);
}
void bus_fault_handler(void){
    while (1);       
}
__attribute((section(".isr_vector"))) uint32_t *isr_vectors[] = {
    (uint32_t *)&_estack, 
    (uint32_t *)reset_handler, /* code entry point */
    (uint32_t *)nmi_handler,
    (uint32_t *)hard_fault_handler,
    (uint32_t *)mem_manage_handler,
    (uint32_t *)bus_fault_handler,
};
