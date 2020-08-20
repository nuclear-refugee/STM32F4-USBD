#include "../header/usb_hal.h"
#include "../../header/uart_print.h" 
#define USB_HAL_DBG(fmt,arg...) uart_print(fmt,##arg)

void usb_device_init(){    
    peripheral_core_init();
   if(!peripheral_device_init(&usb_device_descriptor)){
       USB_HAL_DBG("device init fail\r\n");
   }
}
void usb_host_init(){

}