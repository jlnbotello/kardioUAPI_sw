/**
 * @file ap_cfg.c
 * @brief Application Protocol Configuration Implementation
 *
 * @date 	21/11/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*==================[INCLUSIONS]=============================================*/
#include "ap_cfg.h"
#include "application_protocol.h"
#include "error_handling.h"

/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/
static void tp_allocate(uint8_t *data, uint8_t length);
static bool tp_is_there_space(uint8_t space); 

/*==================[INTERNAL DATA DEFINITION]===============================*/
static TP_Instance_t *p_tpi;

static AP_TxPktQueue_t ap_tx_queue;
static AP_PortQueue_t ap_port_queue;

static AP_PortCallbacks_t lut[AP_CFG_N_OF_PORTS];

static AP_Instance_t api = {
    .lut = lut,
    .lut_length = AP_CFG_N_OF_PORTS,
    .allocate = tp_allocate,
    .is_there_space = tp_is_there_space,
    .tx_pkt_queue = &ap_tx_queue,
    .port_queue = &ap_port_queue,
};

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static void tp_allocate(uint8_t *data, uint8_t length) {
    TP_Allocate(p_tpi, data, length);
}

static bool tp_is_there_space(uint8_t space) {
    return TP_IsThereSpace(p_tpi, space);
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
void APCfg_Init(TP_Instance_t *p_tpi_) {
    ASSERT(p_tpi_ != NULL);
    p_tpi = p_tpi_;
}

AP_Instance_t * APCfg_GetInstance(){
    return &api;
}
/*==================[END OF FILE]============================================*/