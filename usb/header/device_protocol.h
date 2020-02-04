#ifndef __DEVICE_PROTOCOL_H_
#define __DEVICE_PROTOCOL_H_

/**************PID TYPES****************/
/****************Token******************/
#define OUT   0b1000
#define IN    0b1001
#define SOF   0b1010
#define SETUP 0b1011

#define token_out   (OUT << 4 | (~OUT & 0b00001111))
#define token_in    (IN << 4 | (~IN & 0b00001111))
#define token_sof   (SOF << 4 | (~SOF & 0b00001111))
#define token_setup (SETUP << 4 | (~SETUP & 0b00001111))
/****************DATA********************/
#define DATA0 0b1100
#define DATA1 0b1101
#define DATA2 0b1110
#define MDATA 0b1111

#define data_data0 (DATA0 << 4 | (~DATA0 & 0b00001111))
#define data_data1 (DATA1 << 4 | (~DATA1 & 0b00001111))
#define data_data2 (DATA2 << 4 | (~DATA2 & 0b00001111))
#define data_mdata (MDATA << 4 | (~MDATA & 0b00001111))
/****************HANDSHAKE***************/
#define ACK   0b0100
#define NAK   0b0101
#define STALL 0b0111
#define NYET  0110

#define handshake_ack   (ACK << 4 | (~ACK & 0b00001111))
#define handshake_nck   (NAK << 4 | (~NAK & 0b00001111))
#define handshake_stall (STALL << 4 | (~STALL & 0b00001111))
#define handshake_nyet  (NYET << 4 | (~NYET & 0b00001111))
/****************SPECIAL***************/
#define PRE      0b0011
#define ERR      0b0011
#define SPLIT    0b0001
#define PING     0b0010
#define RESERVED 0b0000

#define special_pre (PRE << 4 | (~PRE & 0b00001111))
#define special_err (ERR << 4 | (~ERR & 0b00001111))
#define special_split (SPLIT << 4 | (~SPLIT & 0b00001111))
#define special_ping  (PING << 4 | (~PING & 0b00001111))
#define special_reserved (PRE << 4 | (~PRE & 0b00001111))

#endif