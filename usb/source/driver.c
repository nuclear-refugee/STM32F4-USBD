#include "../header/driver.h"
/**
 * @name peripheral_device_init
 * @brief
 * init device flow in reference manual 34.17.3
 * - Program the following fields in the OTG_FS_DCFG (0x800)
 *   register:
 *      - device speed
 *      - non zero length status out handshake
 * - Gintmsk unmask interrupt register (0x018):
 *      - USB reset
 *      - Enumeration done
 *      - Early suspend
 *      - USB suspend
 *      - SOF
 *
 * - program vbusen bit gcfg enable Vbus sensing in B
 *   device mode and supply 5V across the pull up R on the DP line
 *
 * - wait for USBRST interrupt bit in OTG_FS_GINTSTS.
 *   This bit indicates reset has been detected on the
 *   USB at least 10ms on receiving this interrupt.
 *
 * - Wait for the ENUMDNE interrupt in OTG_FS_GINTSTS.
 *   This interrupt indicates the end ofreset on the USB.
 *   On receiving this interrupt, the application must read
 *   the OTG_FS_DSTS register to determine the enumeration
 *   speed and perform the steps listed in Endpoint
 *   initialization on enumeration completion on page 1353.
 *      - Program the MPSIZ field in OTG_FS_DIEPCTL0 to set the maximum
 *        packet size. This step configures control endpoint 0.
 *        The maximum packet size for a control endpoint depends
 *        on the enumeration speed.

 * - At this point, the device is ready to accept SOF packets
 *   and perform control transfers on control endpoint 0.
 */

void peripheral_device_init(void *descriptor_ptr) {
    device_descriptor_t *device_descriptor_ptr =
        (device_descriptor_t *)descriptor_ptr;
    FS_DCFG |= FULL_SPEED;
    FS_INTMSK |= USB_DEVICE_INT;
    FS_GCCFG = (1 << 19);
    while (FS_INTSTS & (1 << 12) == 0)
        ;
    while (FS_INTSTS & (1 << 13) == 0)
        ;
    FS_DIEPCTL0 = ((FS_DIEPCTL0 & (~0x7ff)) |
                   (device_descriptor_ptr->b_max_packet_size & 0x7ff));
}
/**
 * @name endpoint_activation  
 * @brief
 * - Program the characteristics of the required endpoint into the 
 *   following fields of the OTG_FS_DIEPCTLx register (for IN or bidirectional 
 *   endpoints) or the OTG_FS_DOEPCTLx register (for OUT or bidirectional endpoints).
 *      - Maximum packet size
 *      - USB active endpoint = 1
 *      - Endpoint start data toggle (for interrupt and bulk endpoints)
 *      - Endpoint type
 *      - TxFIFO number
 * - Once the endpoint is activated, the core starts decoding the tokens addressed 
 *   to that endpoint and sends out a valid handshake for each valid token received 
 *   for the endpoint.
 * 
 */
void endpoint_activation(uint8_t endpoint_num,void *device_ptr,void *config_ptr){
    

}
void in_tran() {}
void out_packet() {}

void rx_packet_in_fifo(){

}

