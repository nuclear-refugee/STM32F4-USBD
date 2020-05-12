#ifndef __USB_HAL__
#define __USB_HAL__
#include "device_framework.h"
#include "driver.h"

#pragma pack(push)
#pragma pack(1)
typedef struct  __usb_device{
    device_descriptor_t  device_descriptor;
    configuration_descriptor_t config_descriptor;
    void* interface_descriptor;
} usb_device_t;
#pragma pack(pop)
#endif