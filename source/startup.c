#include <stdint.h>
extern uint32_t _endtext;
extern uint32_t _startdata;
extern uint32_t _enddata;
extern uint32_t _startbss;
extern uint32_t _endbss;
extern uint32_t _estack;

extern void main(void);
void reset_handler(void) {
    uint32_t* end_text = &_endtext;
    uint32_t* start_data = &_startdata;
    uint32_t* end_data = &_enddata;
    while(start_data<end_data) *start_data++=*end_text++;
    uint32_t* start_bss = &_startbss;
    uint32_t* endbss = &_endbss;
    while(start_bss<endbss)*start_bss++=0;

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