/*
 * application_protocol.c
 *
 *  Created on: Nov 9, 2020
 *      Author: jlnbo
 */

/*==================[INCLUSIONS]=============================================*/
#include "application_protocol.h"
#include "error_handling.h"


/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static bool find_port_index(AP_Instance_t * api, uint8_t port, uint8_t *i){
	bool ret_val = false;
	for((*i)=0; (*i) < api->lut_length; (*i)++)
		if(api->lut[(*i)].port == port)
			break;
	if((*i) != api->lut_length) //port found
		ret_val = true;
	return ret_val;
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/

void AP_Init(AP_Instance_t * api){
	ASSERT(api != NULL);
	ASSERT(api->lut != NULL);
	ASSERT(api->tx_pkt_queue != NULL);
	bufferInit(api->tx_pkt_queue,AP_TX_QUEUE_LEN, AP_Packet_t);
	bufferInit(api->port_queue,AP_TX_QUEUE_LEN, uint8_t);
}


bool AP_AddCallbacks(AP_Instance_t * api, AP_PortCallbacks_t port_cbs){
	bool ret_val = false;
	if(api->lut_index < api->lut_length){
		api->lut[api->lut_index] = port_cbs;
		api->lut_index++;
		ret_val = true;
	}
	return ret_val;
}


void AP_SendPacket(AP_Instance_t * api, uint8_t port, uint8_t * payload, uint8_t length){
	if(length <= AP_PAYLOAD_MAX){
		if(!isBufferFull(api->tx_pkt_queue)){
			bufferWritePeek(api->tx_pkt_queue).port = port;
			bufferWritePeek(api->tx_pkt_queue).data = payload;
			bufferWritePeek(api->tx_pkt_queue).length = length;
			bufferWriteSkip(api->tx_pkt_queue);
		}
	}else{
		ASSERT(0);
	}
}

void AP_OnTxEvent(AP_Instance_t * api, AP_RetVal_t ret){
	uint8_t port,index;
	if(ret == AP_SUCCESS){
		api->tx_status = AP_FREE;
	}else{
		api->tx_status = AP_FREE; //FIXME
	}

	while(!isBufferEmpty(api->port_queue)){
		bufferRead(api->port_queue,&port);
		if(find_port_index(api, port, &index)){
			api->lut[index].tx_cb(ret);
		}
	}
}

void AP_OnRxData (AP_Instance_t * api, uint8_t * data, uint8_t length) {
	uint8_t index,offset = 0;

	while(offset < length){
		if(find_port_index(api, data[AP_PORT_POS+offset], &index)){
			api->lut[index].rx_cb(data[AP_PORT_POS+offset], &data[AP_DATA_POS+offset], data[AP_LENGTH_POS+offset]);
		}
		offset += data[AP_LENGTH_POS+offset] + AP_EXTRAS_LEN;
	}
}

void AP_TxHandler(AP_Instance_t * api){
	/* Tx */
	while(!isBufferEmpty(api->tx_pkt_queue) && api->tx_status == AP_FREE){ // one send per cycle?
		AP_Packet_t * pkt = &bufferReadPeek(api->tx_pkt_queue);
		if(api->is_there_space(AP_EXTRAS_LEN + pkt->length)){
			// This order is important!
			api->allocate(&pkt->port,AP_PORT_LEN);
			api->allocate(&pkt->length,AP_LENGTH_LEN);
			api->allocate(pkt->data,pkt->length);
			//saves ports waiting tx
			bufferWrite(api->port_queue,pkt->port);
			bufferReadSkip(api->tx_pkt_queue);
			if(isBufferEmpty(api->tx_pkt_queue))
				api->tx_status = AP_BUSY;
		}
		else{
			api->tx_status = AP_BUSY;
		}
	}
}

/*==================[END OF FILE]============================================*/
