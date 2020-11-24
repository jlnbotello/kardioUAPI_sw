/**
 * @file afe_port_defs.h
 * @brief 
 *
 * @date 	22/11/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

#ifndef AFE_PORT_DEFS_H
#define AFE_PORT_DEFS_H
/*==================[INCLUSIONS]=============================================*/
#include "stdint.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define AFE_PORT                        ((uint8_t)'A')
#define AFE_PORT_ID_POS                 0
#define AFE_PORT_PAYLOAD_POS		1

/*==================[ACTION PACKET]==========================================*/
/* Commands */
#define AFE_PORT_CMD_LEN                1
#define AFE_PORT_CMD_START_ID		((uint8_t)'O')
#define AFE_PORT_CMD_STOP_ID		((uint8_t)'X')

typedef struct __attribute__((packed)) {
	/* Packet ID */
	uint8_t id;
	/* Payload */
	uint8_t sampling_rate;
	uint8_t channel_1_gain;
	uint8_t channel_2_gain;
}afe_port_cfg_t;
    
/* Data Samples Packet */    
#define AFE_PORT_DATA_PKT_LEN           9
#define AFE_PORT_DATA_PKT_ID		((uint8_t)'D')

typedef struct __attribute__((packed)) {
    /* Packet ID */
    const uint8_t id;
    /* Payload */
    int32_t sample_ch1;
    int32_t sample_ch2;   
}afe_port_32bit_data_t;

typedef struct __attribute__((packed)) {
    /* Packet ID */
    const uint8_t id;
    /* Payload */
    int16_t sample_ch1;
    int16_t sample_ch2;   
}afe_port_16bit_data_t;
    
/*==================[TYPEDEF]================================================*/

/*==================[EXTERNAL FUNCTIONS DECLARATION]=========================*/


/*==================[END OF FILE]============================================*/
#endif /* AFE_PORT_DEFS_H */