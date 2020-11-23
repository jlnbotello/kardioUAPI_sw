/**
 * @file led_port.c
 * @brief 
 *
 * @date 	21/10/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*==================[INCLUSIONS]=============================================*/
 #include "led_port.h"
 #include "led_port_defs.h"
 #include "led.h"
 #include "error_handling.h"
 #include "string.h"
 
/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/
static void on_tx_event(AP_RetVal_t  ret);

static void update_status(led_port_status_pkt_t * pkt);

static void send(uint8_t * data, uint8_t length);

static void process_rx_multi_byte(uint8_t port, uint8_t *data, uint8_t length);

static void bufferize_rx_multi_byte(uint8_t port, uint8_t * data, uint8_t length);

static void rx_handler();

/*==================[INTERNAL DATA DEFINITION]===============================*/
#if (LED_PORT_IMPLEMENTATION_TYPE == LED_PORT_SUPERLOOP_IMPLEMENTATION)     
static AP_PortCallbacks_t led_port_cbs = {     \
        .port = LED_PORT,                      \
        .rx_cb = bufferize_rx,                 \
        .tx_cb = on_tx_event,                  \
}; 
#elif (LED_PORT_IMPLEMENTATION_TYPE == LED_PORT_SCHEDULER_IMPLEMENTATION)
static AP_PortCallbacks_t led_port_cbs = {      \
        .port = LED_PORT,                       \
        .rx_cb = process_rx_multi_byte,         \
        .tx_cb = on_tx_event,                   \
};
#else
#error "Implementation not defined"
#endif

static AP_TransferStatus_t rx_status;
static AP_TransferStatus_t tx_status;

static AP_Instance_t * api;

static led_port_status_pkt_t status;
#ifdef SUPERLOOP_IMPLEMENTATION
static uint8_t rx_buffer[LED_PORT_MAX_RX_LEN];
#else
static uint8_t * rx_buffer = NULL;
#endif
static uint8_t rx_data_len = 0;

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static void on_tx_event(AP_RetVal_t  ret){
	if(ret == AP_SUCCESS)
		tx_status = AP_FREE;
}

static void update_status(led_port_status_pkt_t * pkt){
	if((status.l1 = pkt->l1) == LED_PORT_STATUS_ON)
		Led_On(L1);
	else
		Led_Off(L1);

	if((status.l2 = pkt->l2) == LED_PORT_STATUS_ON)
		Led_On(L2);
	else
		Led_Off(L2);

	if((status.l3 = pkt->l3) == LED_PORT_STATUS_ON)
		Led_On(L3);
	else
		Led_Off(L3);

	if((status.l4 = pkt->l4) == LED_PORT_STATUS_ON)
		Led_On(L4);
	else
		Led_Off(L4);
}

static void send(uint8_t * data, uint8_t length){
	if(tx_status == AP_FREE){
		AP_SendPacket(api,LED_PORT, data, length);
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

static void bufferize_rx_multi_byte(uint8_t port, uint8_t * data, uint8_t length){
    if(rx_status == AP_FREE){
            if(length <= LED_PORT_MAX_RX_LEN){
                rx_data_len = length;
                memcpy(rx_buffer,data,length);
                rx_status = AP_BUSY; // New data to process!
            }
    }
}

static void rx_handler() {
    if (rx_status == AP_BUSY) {
        switch (rx_buffer[LED_PORT_ID_POS]) {
        case LED_PORT_STATUS_PKT_ID:
            if (1) { // FIXME rx_data_len == LED_PORT_STATUS_PKT_LEN){
                update_status((led_port_status_pkt_t *)rx_buffer);
            } else {
                // Wrong length!
            }

            break;

        case LED_PORT_REQ_STATUS_ID:
            if (1) { // FIXME rx_data_len == LED_PORT_REQ_STATUS_LEN){
                send((uint8_t *)&status, LED_PORT_STATUS_PKT_LEN);
            } else {
                // Wrong length!
            }

            break;

        default:
            // Not recognized led packet
            break;
        }
        rx_status = AP_FREE;
    }
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
void LedPort_Init(AP_Instance_t * ap_instance){
	api = ap_instance;
	rx_status = AP_FREE;
	tx_status = AP_FREE;
        ASSERT(true == AP_AddCallbacks(api, led_port_cbs));
}
    

/**@brief For superloop implementation. Flags pulling
*
*/
void LedPort_Run(){
        rx_handler();
}
    
/**@brief For superloop implementation. Call from interrupt
*
*/


/*==================[END OF FILE]============================================*/