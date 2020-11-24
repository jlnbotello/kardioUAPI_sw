/*==================[INCLUSIONS]=============================================*/
#include "tp_cfg.h"
#include "ble_nus.h"
#include "application_protocol.h"
#include "error_handling.h"


/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/
static void send_data(uint8_t *data, uint8_t length);
static void tp_onRx(uint8_t * data, uint8_t length);
static void tp_onTx(TP_RetVal_t ret);

/*==================[INTERNAL DATA DEFINITION]===============================*/

static ble_nus_t *p_nus = NULL;
static uint16_t *p_conn_handle = NULL;
static AP_Instance_t *p_api = NULL;

static TP_Packet_t rx_packet;
static TP_Packet_t tx_packet;
static TP_RxRingBuffer_t rx_buffer;

static TP_Instance_t tpi = {
    .tx_packet = &tx_packet,
    .async_tx_mode = false, // send implementation: blocking
    .send = send_data,
    .tx_cb = tp_onTx,
    .rx_buffer = &rx_buffer,
    .rx_packet = &rx_packet,
    .rx_cb = tp_onRx,
    .skip_checksum = true,
    .skip_id_check = true,

};

static TpCfg_RxEvent_t rx_event = {
    .tpi = &tpi,
    .rx_handler = TP_RxHandler
};

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static void send_data(uint8_t * data, uint8_t len) {
    uint16_t length = len;
    uint32_t err_code;
    //ENHANCEMENT Blocking  implementation.
    do {
        err_code = ble_nus_data_send(p_nus, data, &length, *p_conn_handle);
        if ((err_code != NRF_ERROR_INVALID_STATE) &&
            (err_code != NRF_ERROR_RESOURCES) &&
            (err_code != NRF_ERROR_NOT_FOUND)) {
            APP_ERROR_CHECK(err_code);
        }
    } while (err_code == NRF_ERROR_RESOURCES);
}

static void tp_onRx(uint8_t * data, uint8_t length){
	AP_OnRxData(p_api, data, length);
}
    
static void tp_onTx(TP_RetVal_t ret){
	if(ret == TP_SUCCESS){
		AP_OnTxEvent(p_api,AP_SUCCESS);
	}else{
		AP_OnTxEvent(p_api,AP_FAIL);
	}
}
/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/

void TpCfg_RxSchedHandler(void *p_event, uint16_t event_size) {
    TpCfg_RxEvent_t *const p_ev = (TpCfg_RxEvent_t *) p_event;
    p_ev->rx_handler(p_ev->tpi);
}

void TpCfg_Init(ble_nus_t *p_nus_, uint16_t *p_conn_handle_, AP_Instance_t * p_api_) {
    ASSERT(p_nus_ != NULL);
    ASSERT(p_conn_handle_ != NULL);
    ASSERT(p_api_ != NULL);
    p_nus = p_nus_;
    p_conn_handle = p_conn_handle_;
    p_api = p_api_;
    TP_Init(&tpi);
}

TP_Instance_t * TpCfg_GetInstance(){
    return &tpi;
}

TpCfg_RxEvent_t * TpCfg_GetRxEventHandle(){
    return &rx_event;
}

/*==================[END OF FILE]============================================*/