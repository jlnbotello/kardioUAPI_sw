/*
 * application_protocol.h
 *
 *  Created on: Nov 9, 2020
 *      Author: jlnbo
 */

/*
    +-------------------------------+
    |           AP Packet           |
    +--------+--------+-------------+
    |  Port  | Length |     Data    |
    +--------+--------+-------------+
    | 1 Byte | 1 Byte | 0-253 Bytes |
     +--------+--------+-------------+
    Made with https://www.tablesgenerator.com/text_tables#

    Port:     One port per peripheral
    Length:   Data length
    Data:     Data for specified port

    Extras: Port & Length Fields
*/

#ifndef APPLICATION_PROTOCOL_H_
#define APPLICATION_PROTOCOL_H_
/*==================[INCLUSIONS]=============================================*/
#include "stdint.h"
#include "stdbool.h"
#include "ringbuffer.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define AP_PAYLOAD_MAX    23

#define AP_PORT_POS       0
#define AP_PORT_LEN       1

#define AP_LENGTH_POS     1
#define AP_LENGTH_LEN     1

#define AP_DATA_POS       2

#define AP_EXTRAS_LEN     AP_PORT_LEN + AP_PORT_LEN

#define AP_TX_QUEUE_LEN   10

/*==================[TYPEDEF]================================================*/

typedef enum {AP_FREE,AP_BUSY,AP_TIMEOUT} AP_TransferStatus_t;
typedef enum {AP_SUCCESS, AP_FAIL} AP_RetVal_t;
//typedef struct{AP_RetVal_t status;} AP_TxEvent_t;
typedef void (*AP_RxCallback_t)(uint8_t port, uint8_t * data, uint8_t length);
typedef void (*AP_TxCallback_t) (AP_RetVal_t ret);

typedef struct{
  uint8_t port;
  uint8_t length;
  uint8_t * data;
} AP_Packet_t;

ringBuffer_typedef(AP_Packet_t, AP_TxPktQueue_t);
ringBuffer_typedef(uint8_t, AP_PortQueue_t);

typedef struct{
	uint8_t port;
	AP_RxCallback_t rx_cb;
	AP_TxCallback_t tx_cb;
} AP_PortCallbacks_t;

typedef struct{
	AP_TxPktQueue_t * tx_pkt_queue;
	AP_PortQueue_t * port_queue;
	//uint8_t tx_queue_length;
	volatile AP_TransferStatus_t tx_status;
	AP_PortCallbacks_t * lut;
	uint8_t lut_length;
	uint8_t lut_index;
	void (*allocate)(uint8_t * data, uint8_t length); /**< To allocate data to send */
	bool (*is_there_space)(uint8_t n);
} AP_Instance_t;

/*
typedef struct{
	AP_Instance_t * api;
	uint8_t port;
	volatile AP_TransferStatus_t tx_status;
	volatile AP_TransferStatus_t rx_status;
} AP_Handler_t;
*/

void AP_Init(AP_Instance_t * api);

bool AP_AddCallbacks(AP_Instance_t * api, AP_PortCallbacks_t port_cbs);

void AP_SendPacket(AP_Instance_t * api, uint8_t port, uint8_t * payload, uint8_t length);

void AP_OnTxEvent(AP_Instance_t * api, AP_RetVal_t ev);

void AP_OnRxData(AP_Instance_t * api, uint8_t * data, uint8_t length);

void AP_TxHandler(AP_Instance_t * api);



#endif /* APPLICATION_PROTOCOL_H_ */