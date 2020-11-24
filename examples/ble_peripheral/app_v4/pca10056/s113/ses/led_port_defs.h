/*
 * led_port_defs.h
 *
 *  Created on: Nov 10, 2020
 *      Author: jlnbo
 */

#ifndef AP_PORTS_DEFS_LED_PORT_DEFS_H_
#define AP_PORTS_DEFS_LED_PORT_DEFS_H_
/*==================[INCLUSIONS]=============================================*/
#include "stdint.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define	LED_PORT                        ((uint8_t)'L')
#define LED_PORT_ID_POS			0
#define LED_PORT_PAYLOAD_POS		1
#define LED_PORT_MAX_TX_LEN 		5 	//Bigger tx packet
#define LED_PORT_MAX_RX_LEN		5	//Bigger rx packet

/*==================[ACTION PACKET]==========================================*/
/* Request status */
#define LED_PORT_REQ_STATUS_ID		((uint8_t)'R')
#define LED_PORT_REQ_STATUS_LEN		1

typedef struct __attribute__((packed)) {
	/* Packet ID */
	uint8_t id;
}led_port_read_cmd_t;


/*==================[STATUS PACKET]==========================================*/
#define LED_PORT_STATUS_PKT_ID		((uint8_t)'S')
#define LED_PORT_STATUS_PKT_LEN		5

#define LED_PORT_STATUS_OFF  		((uint8_t)'0')
#define LED_PORT_STATUS_ON              ((uint8_t)'1')

typedef struct __attribute__((packed)) {
	/* Packet ID */
	uint8_t id;
	/* Payload */
	uint8_t l1;
	uint8_t l2;
	uint8_t l3;
	uint8_t l4;
}led_port_status_pkt_t;



#endif /* AP_PORTS_DEFS_LED_PORT_DEFS_H_ */
