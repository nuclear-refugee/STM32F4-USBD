#include "../header/usb_hal.h"

void usb_device_init(){    
    peripheral_device_init(&usb_device_descriptor);
}