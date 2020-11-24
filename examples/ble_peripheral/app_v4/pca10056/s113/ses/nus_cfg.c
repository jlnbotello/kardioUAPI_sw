/*==================[INCLUSIONS]=============================================*/
#include "nus_cfg.h"
#include "app_error.h"
#include "nrf_log.h"
#include "app_scheduler.h"

/*==================[INTERNAL DATA DEFINITION]===============================*/
BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT); /**< BLE NUS service instance. */
static TpCfg_RxEvent_t *p_tp_cfg_rx_event;

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service
 *         
 *
 * @param[in] p_evt       Nordic UART Service event.
 */
/**@snippet [Handling the data received over BLE] */
static void nus_data_handler(ble_nus_evt_t *p_evt) {

    if (p_evt->type == BLE_NUS_EVT_RX_DATA) {
        uint32_t err_code;

        NRF_LOG_DEBUG("Received data from BLE NUS. Writing data on UART.");
        NRF_LOG_HEXDUMP_DEBUG(p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);
        /* Copy received data from nus in TP buffer */
        TP_BufferizeRx(p_tp_cfg_rx_event->tpi, p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);
        /* Schedule the TP Rx Handler. Will run on main loop */
        err_code = app_sched_event_put(p_tp_cfg_rx_event, sizeof(TpCfg_RxEvent_t), TpCfg_RxSchedHandler);
        APP_ERROR_CHECK(err_code);
    }
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
void NusCfg_Init(TpCfg_RxEvent_t *p_event) {
    uint32_t err_code;
    ble_nus_init_t nus_init;

    // Initialize NUS.
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;

    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
    ASSERT(p_event != NULL);
    p_tp_cfg_rx_event = p_event;
}

ble_nus_t * NusCfg_GetInstance() {
    return &m_nus;
}

/*==================[END OF FILE]============================================*/