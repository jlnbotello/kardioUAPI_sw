/**
 * @file afe_port.c
 * @brief
 *
 * @date 	22/11/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*==================[INCLUSIONS]=============================================*/
 #include "afe_port.h"
 #include "afe_port_defs.h"
 //#include "ads1292r.h"
 #include "error_handling.h"

/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/
static void on_tx_event(AP_RetVal_t  ret);

static void send(uint8_t * data, uint8_t length);

static void process_rx_multi_byte(uint8_t port, uint8_t *data, uint8_t length);

static void rx_handler();

/*==================[INTERNAL DATA DEFINITION]===============================*/
static AP_PortCallbacks_t afe_port_cbs = {      
        .port = AFE_PORT,                       
        .rx_cb = process_rx_multi_byte,         
        .tx_cb = on_tx_event,                   
};
    
static AP_TransferStatus_t rx_status;
static AP_TransferStatus_t tx_status;

static AP_Instance_t * api;

static afe_port_data_t samples;
static afe_port_cfg_t  config;


static uint8_t * rx_buffer = NULL;
static uint8_t rx_data_len = 0;

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static void on_tx_event(AP_RetVal_t  ret){
	if(ret == AP_SUCCESS)
		tx_status = AP_FREE;
}
            
static void send(uint8_t * data, uint8_t length){
	if(tx_status == AP_FREE){
		AP_SendPacket(api, AFE_PORT, data, length);
		tx_status = AP_BUSY;
	}
}

static void process_rx_multi_byte(uint8_t port, uint8_t *data, uint8_t length) {
    if (rx_status == AP_FREE) {
        rx_status = AP_BUSY;
        rx_data_len = length;
        rx_buffer = data;        
        rx_handler();
    }
}

static void rx_handler() {
    if (rx_status == AP_BUSY) {
        switch (rx_buffer[AFE_PORT_ID_POS]) {
        case AFE_PORT_CMD_START_ID:
            if (rx_data_len == AFE_PORT_CMD_LEN) {

            } else {
                // Wrong length!
            }
            break;
        case AFE_PORT_CMD_STOP_ID:
            if (rx_data_len == AFE_PORT_CMD_LEN) {

            } else {
                // Wrong length!
            }
            break;

        default:
            // Not recognized packet
            break;
        }
        rx_status = AP_FREE;
    }
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/

/*==================[END OF FILE]============================================*/