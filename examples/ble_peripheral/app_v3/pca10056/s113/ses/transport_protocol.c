/*
 * transport_protocol.c
 *
 *  Created on: Nov 9, 2020
 *      Author: jlnbo
 */

/*==================[INCLUSIONS]=============================================*/
#include "transport_protocol.h"
#include "crc.h"
#include "string.h"
#include "error_handling.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
/* --- Rx states --- */
#define ST_WAIT_HEADER 		0xA1
#define ST_WAIT_ID 		0xA2
#define ST_WAIT_FLAG 		0xA3
#define ST_WAIT_PAYLOAD_LEN     0xA4
#define ST_WAIT_CHECKSUM 	0xA5
#define ST_WAIT_PAYLOAD 	0xA6


/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static void update_id(uint8_t * id) {
	if ((*id) < TP_MAX_ID)
		(*id)++;
	else
		(*id) = 0;
}

static bool id_ok(TP_Instance_t * tpi) { //Could be used on resend logic
	bool ret_val = true;
	if(tpi->expected_rx_id != tpi->rx_packet->id )
		ret_val = false; //losing data

	tpi->expected_rx_id = tpi->rx_packet->id;
	update_id(&tpi->expected_rx_id);

	return ret_val;
}

static uint32_t calculate_checksum(uint8_t * data, uint8_t length){
	return crcFast(data, length);
}

static void insert_checksum(TP_Packet_t * pkt){
	uint8_t * raw_pkt = (uint8_t *) pkt;
	uint32_t * p_checksum = (uint32_t *) &raw_pkt[pkt->payload_len + TP_HEAD_LEN];
	* p_checksum = calculate_checksum(raw_pkt, pkt->payload_len + TP_HEAD_LEN);
}

static bool verify_checksum(TP_Packet_t * pkt) {
	bool ret_val = false;
	uint8_t * raw_pkt = (uint8_t *) pkt;
	uint32_t calculated_checksum = calculate_checksum(raw_pkt,pkt->payload_len + TP_HEAD_LEN);
	uint32_t * p_checksum = (uint32_t *) &raw_pkt[pkt->payload_len + TP_HEAD_LEN];
		if(calculated_checksum == * p_checksum)
		ret_val = true;
	return ret_val;
}

static bool valid_rx_pkt(TP_Instance_t * tpi) {
	bool ret_val = false;
	if((verify_checksum(tpi->rx_packet) || tpi->skip_checksum) && (id_ok(tpi) || tpi->skip_id_check))
		ret_val = true;
	return ret_val;
}

static bool process_rx(TP_Instance_t * tpi, uint8_t in) {
	//uint8_t * p_checksum;
	bool ret_val = false;
	switch (tpi->rx_state) {

	case ST_WAIT_HEADER:
		if (in == TP_HEADER) {
			tpi->rx_packet->header = in;
			tpi->rx_state = ST_WAIT_ID;
		}
		break;

	case ST_WAIT_ID:
		tpi->rx_packet->id = in;
		tpi->rx_state = ST_WAIT_FLAG;
		break;

	case ST_WAIT_FLAG:
		tpi->rx_packet->flag = in;
		tpi->rx_state = ST_WAIT_PAYLOAD_LEN;
		break;

	case ST_WAIT_PAYLOAD_LEN:
		tpi->rx_packet->payload_len = in;
		tpi->rx_state = ST_WAIT_PAYLOAD;
		tpi->offset = 0;
		break;

	case ST_WAIT_PAYLOAD:
		tpi->rx_packet->data[tpi->offset] = in;
		tpi->offset++;
		if (tpi->offset == tpi->rx_packet->payload_len) {
			tpi->offset = tpi->rx_packet->payload_len;
			tpi->rx_state = ST_WAIT_CHECKSUM;
		}
		break;
	case ST_WAIT_CHECKSUM:
		//(uint8_t *) pkt = (uint8_t *) &(tpi->rx_packet->checksum);
		//pkt[tpi->offset] = in; // WARNING: ENDIANESS!
		((uint8_t *) tpi->rx_packet->data)[tpi->offset] = in;
		tpi->offset++;
		if (tpi->offset >= tpi->rx_packet->payload_len + TP_CHECKSUM_LEN) {
			ret_val = true;
			tpi->offset = 0;
			tpi->rx_state = ST_WAIT_HEADER;
		}
		break;
	default:
		break;
	}
	return ret_val;
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
bool TP_Init(TP_Instance_t * tpi) {
	ASSERT(tpi->tx_packet != NULL);
	ASSERT(tpi->send != NULL);
	ASSERT(tpi->tx_cb != NULL);
        ASSERT(tpi->rx_buffer != NULL);
	ASSERT(tpi->rx_packet != NULL);
	ASSERT(tpi->rx_cb != NULL);

	tpi->tx_packet->header = TP_HEADER; //Fixed header for el TP packets
	tpi->tx_packet->payload_len = 0;
	tpi->tx_status = TP_FREE;
	tpi->rx_status = TP_FREE;
        bufferInit(tpi->rx_buffer,TP_MAX_PKT_LEN, uint8_t);
	tpi->rx_state = ST_WAIT_HEADER;
	tpi->offset = 0;
	tpi->expected_rx_id = 1;

	crcInit();

	return true;
}

bool TP_IsThereSpace(TP_Instance_t * tpi, uint8_t n){
	bool ret_val = false;
	uint16_t next_tx_w_pos = tpi->tx_packet->payload_len + n;
	if(next_tx_w_pos <= TP_MAX_PKT_LEN)
		ret_val = true;
	return ret_val;
}

/**@brief Does not requiere a ring buffer. Process rx data on callback (an array).
*
*/
uint8_t TP_ProcessRxMultiByte(TP_Instance_t * tpi, uint8_t * data, uint8_t length){
	uint8_t i = 0;
	uint8_t processed_bytes = length;
	while(i < length){
		if(process_rx(tpi,data[i]) && tpi->rx_status == TP_FREE){
			tpi->rx_status = TP_BUSY;
			processed_bytes = i+1;
			break;
		}
		i++;
	}
	return processed_bytes;
}

/**@brief Does not requiere a ring buffer. Process rx data on callback (byte by byte).
*
*/
void TP_ProcessRxByte(TP_Instance_t *tpi, uint8_t data) {
    if (process_rx(tpi, data) && tpi->rx_status == TP_FREE) {
        tpi->rx_status = TP_BUSY;
    }
}

/* Require a ring buffer. differed processing*/    

uint8_t TP_BufferizeRx(TP_Instance_t * tpi, uint8_t const * const data, uint8_t length){
    uint8_t pos = 0;
    while(!isBufferFull(tpi->rx_buffer) && pos < length){
      bufferWrite(tpi->rx_buffer,data[pos])
      pos++;
    }
    if(isBufferFull(tpi->rx_buffer) && pos < length){
      /* Incoming data size is greater than buffer */
    }
  return pos;
}

/**
 * Call after lower layer has sent the data
 */
void TP_OnTxEvent(TP_Instance_t * tpi, TP_RetVal_t ret){
	if(ret == TP_SUCCESS){
		tpi->tx_status = TP_FREE;
		tpi->tx_packet->payload_len = 0;
	}
	tpi->tx_cb(ret);
}

void TP_Allocate(TP_Instance_t * tpi, uint8_t *data, uint8_t length) {
	uint8_t * p_pay = tpi->tx_packet->data;
	uint8_t tx_w_pos = tpi->tx_packet->payload_len;
	uint16_t next_tx_w_pos = tx_w_pos + length;

	if(next_tx_w_pos <= TP_MAX_PKT_LEN && tpi->tx_status == TP_FREE){ // available space to allocate payload
		memcpy(&p_pay[tx_w_pos], data, length);
		tpi->tx_packet->payload_len += length;
	}else{

	}
}

static void tp_rx_handler(TP_Instance_t *tpi) {
    if (tpi->rx_status == TP_BUSY) {
        if (valid_rx_pkt(tpi)) {
            tpi->rx_cb(tpi->rx_packet->data, tpi->rx_packet->payload_len);
            tpi->rx_status = TP_FREE;
        }
    }
}

void TP_TxHandler(TP_Instance_t *tpi) {
    if (tpi->tx_packet->payload_len > 0 && tpi->tx_status == TP_FREE) {
        tpi->tx_status = TP_BUSY;
        update_id(&tpi->tx_packet->id);
        insert_checksum(tpi->tx_packet);

        tpi->send((uint8_t *)tpi->tx_packet, tpi->tx_packet->payload_len + TP_EXTRAS_LEN);

        if (tpi->async_tx_mode == false) {
            TP_OnTxEvent(tpi, TP_SUCCESS);
        }
    }
}

/**@brief For superloop implementation. Flags pulling
*
*/
void TP_Run(TP_Instance_t *tpi) {
    /* Rx */
    tp_rx_handler(tpi);
    /* Tx */
    TP_TxHandler(tpi);
}


void TP_RxHandler(TP_Instance_t *tpi){
    uint8_t data;
    while(!isBufferEmpty(tpi->rx_buffer)){
        bufferRead(tpi->rx_buffer,&data);
        TP_ProcessRxByte(tpi,data);
        tp_rx_handler(tpi);
    }    
}