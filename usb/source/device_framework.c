#include "../header/device_framework.h"

void device_change_state(uint8_t state, uint8_t event) {
    uint8_t last_state;
    switch (state) {
        case ATTACHED:
            state = POWERED;
            break;
        case POWERED:
            if (event == RX_RESET) {
                state = DEFAULT;
            } else if (event == HUB_DECONFIGURED) {
                state = ATTACHED;
            } else if (event == BUS_INACTVE) {
                last_state = POWERED;
                state = SUSPENDED;
            }
            break;
        case DEFAULT:
            if (event == ADDRESS_ASSIGNED) {
                handle_device_descriptor();
                state = ADDRESS;
            } else if (event == BUS_INACTVE) {
                last_state = DEFAULT;
                state = SUSPENDED;
            }
            break;
        case ADDRESS:
            if (event == DEVICE_CONFIG) {
                handle_config_descriptor();
                state = CONFIGURED;
            } else if (event == BUS_INACTVE) {
                last_state = ADDRESS;
                state = SUSPENDED;
            }
            break;
        case CONFIGURED:
            if (event == DEVICE_DECONFIG) {
                state = ADDRESS;
            } else if (event == BUS_INACTVE) {
                last_state = CONFIGURED;
                state = SUSPENDED;
            }
            break;
        case SUSPENDED:
            if (event == BUS_ACTIVE) {
                state = last_state;
            }
            break;
        case RESET:
            if (event == RX_RESET) {
                handle_transit_default();
                state = DEFAULT;
            } else if (event == POWER_INTERRUPT) {
                handle_power_interrupt();
                state = POWERED;
            }
            break;
        default:
            break;
    }
}

void handle_device_descriptor(){

}

void handle_config_descriptor(){

}

void RX_resume(){
    device_change_state(RESET,RX_RESET);
}
void handle_transit_default(){

}
void handle_power_interrupt(){
    
}