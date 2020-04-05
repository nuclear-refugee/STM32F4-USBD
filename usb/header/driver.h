#include "../../header/reg.h"
#include "../header/device_framework.h"
#define FULL_SPEED        (1 << 1) | (1 << 0)
#define INT_ENUM_DONE     (1 << 13)
#define INT_USB_RESET     (1 << 12)
#define INT_EARLY_SUSPEND (1 << 10)
#define INT_USB_SUSPEND   (1 << 11)
#define INT_SOF           (1 << 3)
#define USB_DEVICE_INT \
    (INT_ENUM_DONE | INT_USB_RESET | INT_EARLY_SUSPEND | INT_USB_SUSPEND |\
     INT_SOF)
void peripheral_device_init();
