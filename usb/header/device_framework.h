#ifndef __DEVICE_FRAMEWORK__
#define __DEVICE_FRAMEWORK__

#include <stdint.h>
/************bmRequestType*****************/
#define DIRECTION_OFFSET 7
#define HOST_TO_DEVICE   (0 << DIRECTION_OFFSET)
#define DEVICE_TO_HOST   (1 << DIRECTION_OFFSET)

#define TYPE_OFFSET   5
#define TYPE_STANDARD (0 << TYPE_OFFSET)
#define TYPE_CLASS    (1 << TYPE_OFFSET)
#define TYPE_VENDOR   (2 << TYPE_OFFSET)

#define RECIPIENT_OFFSET 0
#define RCPNT_DEVICE     (0 << RECIPIENT_OFFSET)
#define RCPNT_INTERFACE  (1 << RECIPIENT_OFFSET)
#define RCPNT_ENDPOINT   (2 << RECIPIENT_OFFSET)
#define RCPNT_OTHER      (3 << RECIPIENT_OFFSET)

/*******************Request *******************/
#define GET_STATUS     0
#define CLEAR_FEATURE  1
#define SET_FEATURE    3
#define SET_ADDRESS    5
#define GET_DESCRIPTOR 6
#define SET_DESCRPOTR  7
#define GET_CONFIG     8
#define SET_CONFIG     9
#define GET_INTERFACE  10
#define SET_INTERFACE  11
#define SYNC_FRAME     12

/*****************Descriptor types**************/
#define DEVICE          1
#define CONFIG          2
#define STRING          3
#define INTERFACE       4
#define ENDPOINT        5
#define DEVIE_QUALIFIER 6
#define OHTER_SPEED     7
#define INTERFACE_POWER 8

#pragma pack(push)
#pragma pack(1)
typedef struct __interface_descriptor_t {
    uint8_t b_length;
    uint8_t b_descriptor_type;
    uint8_t b_interface_num;
    uint8_t b_alt_setting;
    uint8_t b_num_endpoints;
    uint8_t b_interface_class;
    uint8_t b_interface_sub_class;
    uint8_t b_interface_protocol;
    uint8_t i_interface;
} interface_descriptor_t;

#pragma pack(pop)
#endif