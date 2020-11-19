/**
 * @file application_protocol.c
 * @brief Implementation of application protocol
 *
 * @date 	28/10/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */
 
/*==================[INCLUSIONS]=============================================*/
#include "application_protocol.h"
#include "stdio.h"

/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL DATA DECLARATION]==============================*/
extern const uint8_t AP_CallbackTableSize;
extern const AP_Callback_t AP_CallbackTable[];

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
void AP_LowerCallback(uint8_t * raw_pkt) {
  AP_Packet_t ap_pkt = {raw_pkt[AP_PORT_POS],&raw_pkt[AP_DATA_POS]};
  //FIXME: uncomment next 2 line
  //if(ap_pkt.port < AP_CallbackTableSize)
   //AP_CallbackTable[ap_pkt->port](arr);
    AP_CallbackTable[1](ap_pkt.data); //comment this hardcorded port
}

void AP_Init(){
}

void AP_SendPacket(uint8_t type, uint8_t * data, uint8_t length){
	
}

/*==================[END OF FILE]============================================*/