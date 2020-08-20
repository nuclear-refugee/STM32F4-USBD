#include "../header/driver.h"
enum host_packet_status {
    RX_IN_DATA = 2,
    IN_TRANSFER_COMPLETE = 3,
    DATA_TOGGLE_ERROR = 5,
    CHANNEL_HALT = 7,
};
enum device_packet_status {
    GLOBAL_OUT_NAK = 1,
    RX_OUT_DATA = 2,
    OUT_TRANSFER_COMPLETE = 3,
    SETUP_TRANSACTION_COMPLETE = 4,
    RX_SETUP_DATA = 6,
};
typedef struct _HOST_RXSTSP {
    uint8_t packet_status;
    uint8_t pid;
    uint8_t byte_count;
    uint8_t channel_number;
} host_read_fifo_status_t;
typedef struct _DEVICE_RXSTSP {
    uint8_t frame_number;
    uint8_t packet_status;
    uint8_t pid;
    uint8_t byte_count;
    uint8_t endpoint_number;
} device_read_fifo_status_t;

/**
 * init sequence
 * core init
 * device init or host init
 * endpo
 *
 */

/**
 * @name peripheral_core_init
 * @brief
 */
void peripheral_core_init() {
    // GPIO & OTG FS peripheral enable
    // GPIOA pin PA8 FS_SOF PA10 FS_ID PA11 FS_DM PA12 FS_DP
    RCC_AHB1_ENR |= (1 << 31) | (1 << 0);
    // AHB2 bit7 OTGFS
    RCC_AHB2_ENR |= (1 << 7);
    // GPIOA alternate setting PA11 DM PA12 DP
    GPIO_MODER(GPIOA_BASE) = (2 << 22) | (2 << 24);
    GPIO_AFRH(GPIOA_BASE) = (10 << 11) | (10 << 12);
    FS_AHBCFG |= GINTMSK;
    FS_INTSTS |= RXFLVL | PTXFE;
    FS_USBCFG |= HNP | SRP | FS_TIMEOUT(6) | USB_TURNAROUND_TIME(4);
    FS_INTMSK |= OTG_INT_MSK | MODE_MISMATCH_INT_MSK;
}
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
// AHB need more than 14.2Mhz
uint8_t peripheral_device_init() {
    //  CMOD bit0 value 1: host mode 0: device mode
    if (FS_INTSTS & CMOD) {
        return 0;
    }
    FS_DCFG |= FULL_SPEED;
    FS_INTMSK |= USB_DEVICE_INT;
    FS_GCCFG = (1 << 19);
    while ((FS_INTSTS & USB_RST) == 0)
        ;
    return 1;
}

/**
 * @name peripheral_usb_reset_endpoint_init
 * @brief all initialization required to receive SETUP packets is done.
 */
void peripheral_usb_reset_endpoint_init(void* descriptor_ptr,
                                        void* endpoint1_ptr,
                                        void* endpoint2_ptr,
                                        void* endpoint3_ptr) {
    device_descriptor_t* device_ptr = (device_descriptor_t*)descriptor_ptr;
    endpoint_descriptor_t* ep1_ptr = (endpoint_descriptor_t*)endpoint1_ptr;
    endpoint_descriptor_t* ep2_ptr = (endpoint_descriptor_t*)endpoint2_ptr;
    endpoint_descriptor_t* ep3_ptr = (endpoint_descriptor_t*)endpoint3_ptr;
    // all out endpoint SNAK bit set 1 bit 27 set nak bit 26 write 1 clear nak
    FS_DOEPCTL0 = (1 << 27);
    if ((ep1_ptr->b_endpoint_address & (1 << 7)) == 0) {
        FS_DOEPCTL1 = (1 << 27);
    }
    if ((ep2_ptr->b_endpoint_address & (1 << 7)) == 0) {
        FS_DOEPCTL2 = (1 << 27);
    }
    if ((ep3_ptr->b_endpoint_address & (1 << 7)) == 0) {
        FS_DOEPCTL3 = (1 << 27);
    }
    //  unmask interrupt mask bit 0 - bit 3 mapping to in endpoint 0-3
    //  bit16-bit19 mapping to out endpoint 0-3
    FS_DAINTMSK = (1 << 0) | (1 << 16);
    // set data fifo ram for each fifo
    FS_RXFSIZ = device_ptr->b_max_packet_size;
    // set endpoint 0 to RX the number of back to back setup packet
    FS_DOEPTSIZ0 = (3 << 29);
}
/**
 *@name Endpoint initialization on enumeration completion
 *@brief At this point, the device is ready to receive SOF packets and is
 *configured to perform control transfers on control endpoint 0
 */
void peripheral_endpoint_init_on_enum_complete(void* descriptor_ptr) {
    device_descriptor_t* device_ptr = (device_descriptor_t*)descriptor_ptr;
    // Endpoint initialization on enumeration completion
    while ((FS_INTSTS & ENUM_DONE) == 0)
        ;
    FS_DIEPCTL0 =
        ((FS_DIEPCTL0 & (~0x7ff)) | (device_ptr->b_max_packet_size & 0x7ff));
    // At this point, the device is ready to receive SOF packets and is
    // configured to perform control
    // transfers on control endpoint 0.
}

/**
 * @name peripheral_endpoint_init_on_set_address_cmd
 * @brief he application must do  when receives a SetAddress command in a SETUP
 * packet.
 */
void peripheral_endpoint_init_on_set_address_cmd(uint8_t device_address) {
    FS_DCFG = device_address;
    // TODO: send status in packet
}
/**
 * @name peripheral_endpoint_init_on_set_config_or_interface_cmd
 * @brief
 * 1. When a SetConfiguration command is received, the application must program
 * the endpoint registers to configure them with the characteristics of the
 * valid endpoints in the new configuration.
 * 2. When a SetInterface command is received, the application must program the
 * endpoint registers of the endpoints affected by this command.
 * 3. Some endpoints that were active in the prior configuration or alternate
 * setting are not valid in the new configuration or alternate setting. These
 * invalid endpoints must be deactivated.
 * 4. Unmask the interrupt for each active endpoint and mask the interrupts for
 * all inactive endpoints in the OTG_FS_DAINTMSK register.
 * 5. Set up the Data FIFO RAM for each FIFO.
 * 6. After all required endpoints are configured; the application must program
 * the core to send a status IN packet. At this point, the device core is
 * configured to receive and transmit any type of data packet
 */
void peripheral_endpoint_init_on_set_config_or_interface_cmd() {
    // TODO: the brief describe
}
/**
 * @name endpoint_activation
 * @brief  the steps required to activate a device endpoint or to configure an
 * existing device endpoint to a
 *
 * "new type."
 *
 * - Program the characteristics of the required endpoint into the
 *   following fields of the OTG_FS_DIEPCTLx register (for IN or bidirectional
 *   endpoints) or the OTG_FS_DOEPCTLx register (for OUT or bidirectional
 * endpoints).
 *      - Maximum packet size
 *      - USB active endpoint = 1
 *      - Endpoint start data toggle (for interrupt and bulk endpoints)
 *      - Endpoint type
 *      - TxFIFO number
 * - Once the endpoint is activated, the core starts decoding the tokens
 * addressed to that endpoint and sends out a valid handshake for each valid
 * token received for the endpoint.
 *
 */
void endpoint_activation(uint8_t num_of_endpoint, void* endpoint1_ptr,
                         void* endpoint2_ptr, void* endpoint3_ptr) {
    endpoint_descriptor_t* ep1_ptr = (endpoint_descriptor_t*)endpoint1_ptr;
    endpoint_descriptor_t* ep2_ptr = (endpoint_descriptor_t*)endpoint2_ptr;
    endpoint_descriptor_t* ep3_ptr = (endpoint_descriptor_t*)endpoint3_ptr;
    // in out endpointX control register
    switch (num_of_endpoint) {
        case 3:
            if (ep3_ptr->b_endpoint_address & (1 << 7)) {
                FS_DIEPCTL3 = MPSIZ(ep3_ptr->w_max_packet_size & 0x7ff) |
                              TXFNUM(3) | USBAEP | DATA0PID;
            } else {
                FS_DOEPCTL3 = MPSIZ(ep3_ptr->w_max_packet_size & 0x7ff) |
                              USBAEP | EPTYP((ep3_ptr->bm_attributes & 3)) |
                              DATA0PID;
            }
        case 2:
            if (ep2_ptr->b_endpoint_address & (1 << 7)) {
                FS_DIEPCTL2 = MPSIZ(ep2_ptr->w_max_packet_size & 0x7ff) |
                              TXFNUM(3) | USBAEP | DATA0PID;
            } else {
                FS_DOEPCTL2 = MPSIZ(ep2_ptr->w_max_packet_size & 0x7ff) |
                              USBAEP | EPTYP((ep2_ptr->bm_attributes & 3)) |
                              DATA0PID;
            }
        case 1:
            if (ep1_ptr->b_endpoint_address & (1 << 7)) {
                FS_DIEPCTL1 = MPSIZ(ep1_ptr->w_max_packet_size & 0x7ff) |
                              TXFNUM(3) | USBAEP | DATA0PID;
            } else {
                FS_DOEPCTL1 = MPSIZ(ep1_ptr->w_max_packet_size & 0x7ff) |
                              USBAEP | EPTYP((ep1_ptr->bm_attributes & 3)) |
                              DATA0PID;
            }
            break;
        default:
            break;
    }
}

/**
 * @name endpoint_deactivation
 * @brief  the steps required to deactivate an existing endpoint.
 * - Program the characteristics of the required endpoint into the
 *   following fields of the OTG_FS_DIEPCTLx register (for IN or bidirectional
 *   endpoints) or the OTG_FS_DOEPCTLx register (for OUT or bidirectional
 * endpoints).
 *      - Maximum packet size
 *      - USB active endpoint = 1
 *      - Endpoint start data toggle (for interrupt and bulk endpoints)
 *      - Endpoint type
 *      - TxFIFO number
 * - Once the endpoint is activated, the core starts decoding the tokens
 * addressed to that endpoint and sends out a valid handshake for each valid
 * token received for the endpoint.
 */
void endpoint_deactivation(uint8_t num_of_endpoint, void* endpoint1_ptr,
                           void* endpoint2_ptr, void* endpoint3_ptr) {
    FS_INTMSK &= ~(NPTXFEM | RXFLVLM);

    endpoint_descriptor_t* ep1_ptr = (endpoint_descriptor_t*)endpoint1_ptr;
    endpoint_descriptor_t* ep2_ptr = (endpoint_descriptor_t*)endpoint2_ptr;
    endpoint_descriptor_t* ep3_ptr = (endpoint_descriptor_t*)endpoint3_ptr;
    // in out endpointX control register
    switch (num_of_endpoint) {
        case 3:
            if (ep3_ptr->b_endpoint_address & (1 << 7)) {
                FS_DIEPCTL3 = 0;
            } else {
                FS_DOEPCTL3 = 0;
            }
        case 2:
            if (ep2_ptr->b_endpoint_address & (1 << 7)) {
                FS_DIEPCTL2 = 0;
            } else {
                FS_DOEPCTL2 = 0;
            }
        case 1:
            if (ep1_ptr->b_endpoint_address & (1 << 7)) {
                FS_DIEPCTL1 = 0;
            } else {
                FS_DOEPCTL1 = 0;
            }
            break;
        default:
            break;
    }
}
/**
 * OPERATIONAL　MODEL
 */
/**
 * @name device_transaction_data_handle
 * @brief in reference manual 34.17.6 operational model
 * TODO: hook to isr
 */

void device_transaction_data_handle(void* buffer_ptr) {
    uint32_t temp;
    device_read_fifo_status_t status;
    while (!(FS_INTSTS & RXFLVL))
        ;
    temp = FS_RXSTSP;
    status.endpoint_number = (temp & 0xf);
    status.byte_count = (temp >> 4) & 0x7ff;
    status.pid = (temp >> 16) & 0x3;
    status.packet_status = (temp >> 17) & 0xf;
    status.frame_number = (temp >> 21) & 0xf;
    FS_INTMSK &= ~RXFLVLM;
    switch (status.packet_status) {
        case GLOBAL_OUT_NAK:
            if (status.byte_count == 0) {
                // TODO: out nak has RXed
            } else {
            }
            break;
        case RX_OUT_DATA:
            if (status.byte_count != 0) {
                // TODO: handle endpointx out data
            } else {
            }
            break;
        case OUT_TRANSFER_COMPLETE:
            if (status.byte_count == 0) {
                // TODO: return data complete value
            } else {
            }
            break;
        case SETUP_TRANSACTION_COMPLETE:
            if ((status.byte_count == 0) && (status.endpoint_number == 0)) {
            } else {
            }
            break;
        case RX_SETUP_DATA:
            if ((status.byte_count == 8) && (status.endpoint_number == 0)) {
                // TODO:　store setup data
            } else {
            }
            break;
        default:
            break;
    }
    FS_INTMSK |= RXFLVLM;
}
void setup_transaction() {}
void out_packet() {}

void in_tran() {}

void rx_packet_in_fifo() {}