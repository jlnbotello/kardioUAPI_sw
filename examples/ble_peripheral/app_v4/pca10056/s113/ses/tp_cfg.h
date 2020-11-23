#ifndef A_TEMPLATE_H
#define A_TEMPLATE_H
/*==================[INCLUSIONS]=============================================*/
#include "transport_protocol.h"
#include "application_protocol.h"
#include "ble_nus.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define TP_CFG_EVENT_LEN        8 //FIXME
#define TP_RX_RINGBUFFER_LEN    TP_PAYLOAD_MAX
/*==================[TYPEDEF]================================================*/
typedef struct {    
    TP_Instance_t * tpi;  
    void (*rx_handler)(TP_Instance_t *tpi);    
} TPCfg_RxEvent_t;
/*==================[EXTERNAL FUNCTIONS DECLARATION]=========================*/
void TPCfg_Init(ble_nus_t *p_nus_, uint16_t *p_conn_handle_, AP_Instance_t * p_api_);

void TpCfg_RxSchedHandler(void *p_event, uint16_t event_size);

TPCfg_RxEvent_t * TPCfg_GetRxEventHandle();

TP_Instance_t * TPCfg_GetInstance();

/*==================[END OF FILE]============================================*/
#endif /* A_TEMPLATE_H */