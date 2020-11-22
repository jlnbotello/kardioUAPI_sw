/*
 * transport_protocol.h
 *
 *  Created on: Nov 9, 2020
 *      Author: jlnbo
 */

/*
	+------------------------------------------------------------+
	|                          TP Packet                         |
	+-----------------------------------+-------------+----------+
	|                Head               |   Payload   |   Tail   |
	+--------+--------+--------+--------+-------------+----------+
	| Header |   Id   |  Flag  | Length |   Payload   | Checksum |
	+--------+--------+--------+--------+-------------+----------+
	| 1 Byte | 1 Byte | 1 Byte | 1 Byte | 0-255 Bytes |  4 Bytes |
	+--------+--------+--------+--------+-------------+----------+
    Made with https://www.tablesgenerator.com/text_tables#

    Header: 	Fixed for TP (0xAA)
    Id: 		Cyclic incremental counter
    Flag: 		Reserved. Internal use
    Length: 	Payload length
    Payload:	Upper communication layer data
    Checksum:	Calculated over head and payload

    Extras: Header, Id, Flag, Length & Checksum Fields
*/

#ifndef TRANSPORT_PROTOCOL_H_
#define TRANSPORT_PROTOCOL_H_
/*==================[INCLUSIONS]=============================================*/
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "ringbuffer.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define TP_HEADER       				'T'

#define TP_HEADER_POS   				0
#define TP_ID_POS       				1
#define TP_FLAG_POS     				2
#define TP_LENGTH_POS   				3
#define TP_PAYLOAD_POS                                  4

#define TP_HEADER_LEN					1
#define TP_ID_LEN					1
#define TP_FLAG_LEN					1
#define TP_LENGTH_LEN					1
#define TP_CHECKSUM_LEN 				4
#define TP_PAYLOAD_MAX					50

#define TP_HEAD_LEN					TP_HEADER_LEN	+\
							TP_ID_LEN 	+\
							TP_FLAG_LEN 	+\
							TP_LENGTH_LEN

#define TP_TAIL_LEN                                     TP_CHECKSUM_LEN

#define TP_EXTRAS_LEN					TP_HEAD_LEN     +\
                                                        TP_TAIL_LEN

#define TP_MAX_PKT_LEN 					TP_HEAD_LEN	+\
							TP_PAYLOAD_MAX	+\
							TP_TAIL_LEN

#define TP_MAX_LEN_PROCESSED_PER_CYCLE	100

#define TP_MAX_ID 						255

/*==================[TYPEDEF]================================================*/
typedef enum {TP_SUCCESS, TP_FAIL} TP_RetVal_t;
typedef enum {TP_FREE,TP_BUSY} TP_BufferStatus_t;
//typedef struct{TP_RetVal_t status;} TP_TxEvent_t;
//typedef struct{TP_RetVal_t status;} TP_RxEvent_t;
typedef void (*TP_TxCallback_t) (TP_RetVal_t ev);
typedef void (*TP_RxCallback_t) (uint8_t * data, uint8_t length);

typedef struct __attribute__((packed)){
  uint8_t header;
  uint8_t id;
  uint8_t flag; //reserved
  uint8_t payload_len;
  uint8_t data[TP_PAYLOAD_MAX + TP_CHECKSUM_LEN]; // Payload + Checksum
} TP_Packet_t;

ringBuffer_typedef(uint8_t,TP_RxRingBuffer_t);

typedef struct{
	/*Tx*/
	TP_Packet_t * tx_packet;
	volatile TP_BufferStatus_t tx_status;
	bool async_tx_mode;
	void (*send)(uint8_t * data, uint8_t length);
	TP_TxCallback_t tx_cb;


	/*Rx*/
        TP_RxRingBuffer_t * rx_buffer;
	TP_Packet_t * rx_packet;
	volatile TP_BufferStatus_t rx_status;
	TP_RxCallback_t rx_cb;
	//uint16_t (*get_rx_data_length)(void);
	//uint8_t (*get_rx_data)(void);
	bool skip_checksum;
	bool skip_id_check;
	uint8_t expected_rx_id;
	uint8_t rx_state;
	uint8_t offset;
}TP_Instance_t;



/*==================[EXTERNAL FUNCTIONS DECLARATION]=========================*/
bool TP_Init(TP_Instance_t * tpi);

bool TP_IsThereSpace(TP_Instance_t * tpi, uint8_t n);

uint8_t TP_ProcessRxMultiByte(TP_Instance_t * tpi, uint8_t * data, uint8_t length);

void TP_ProcessRxByte(TP_Instance_t * tpi, uint8_t  data);

uint8_t TP_BufferizeRx(TP_Instance_t * tpi, uint8_t const * const data, uint8_t length);

void TP_OnTxEvent(TP_Instance_t * tpi, TP_RetVal_t ev);

void TP_Allocate(TP_Instance_t * tpi, uint8_t *data, uint8_t length);

void TP_RxHandler(TP_Instance_t *tpi);

void TP_TxHandler(TP_Instance_t *tpi);


/*==================[END OF FILE]============================================*/
#endif /* TRANSPORT_PROTOCOL_H_ */
