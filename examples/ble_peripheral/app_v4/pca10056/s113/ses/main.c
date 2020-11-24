#include "ble_cfg.h"
#include "bsp_cfg.h"
#include "log_cfg.h"
#include "nus_cfg.h"
#include "pwr_mgmt_cfg.h"
#include "timer_cfg.h"
#include "scheduler_cfg.h"
#include "ap_cfg.h"
#include "tp_cfg.h"
#include "afe_cfg.h"
#include "afe_port.h"
#include "led.h"
#include "led_port.h"
#include "transport_protocol.h"
#include "application_protocol.h"

#include "nrf_gpio.h"
#include "kardioUAPI_v1_0.h"

static ble_advertising_t * p_adver_h = NULL;
static uint16_t * p_conn_h = NULL;
static ble_nus_t * p_nus = NULL;
static AP_Instance_t * p_api = NULL;
static TP_Instance_t * p_tpi = NULL;
static TpCfg_RxEvent_t * p_rx_event;

static void disable_afe2() {
    nrf_gpio_cfg_output(KARDIOUAPI_AFE_B_PWD_RST);
    nrf_gpio_pin_set(KARDIOUAPI_AFE_B_PWD_RST);
    nrf_gpio_cfg_output(KARDIOUAPI_AFE_B_SPI_CS);
    nrf_gpio_pin_set(KARDIOUAPI_AFE_B_SPI_CS);
}

/**@brief Application main function.
 */
int main(void)
{
    bool erase_bonds;
    p_adver_h = BleCfg_GetAdverHandle();
    p_conn_h = BleCfg_GetConnHandle();
    p_nus = NusCfg_GetInstance();
    p_api = ApCfg_GetInstance();
    p_tpi = TpCfg_GetInstance();
    p_rx_event = TpCfg_GetRxEventHandle();

    // Initialize.
    Led_Init();
    LogCfg_Init();
    TimerCfg_Init();
    //BSPCfg_Init(p_adver_h, p_conn_h);
    //BSPCfg_ButtonsInit(&erase_bonds);
    PwrMgmtCfg_Init();   
    BleCfg_Init(p_rx_event);
    ApCfg_Init(p_tpi);
    TpCfg_Init(p_nus, p_conn_h, p_api);
    disable_afe2();
    AfeCfg_Init();
    LedPort_Init(p_api);
    AfePort_Init(p_api);    
    SchedulerCfg_Init();

    // Start execution.
    NRF_LOG_INFO("Debug logging for UART over RTT started.");
    BleCfg_StartAdvertising();
  


    // Enter main loop.
    for (;;)
    { 
        app_sched_execute();
        AP_TxHandler(p_api); // Will allocate all AP packets inside TP packet
        TP_TxHandler(p_tpi); // Will send the packet if payload is > 0
        PwrMgmtCfg_IdleStateHandle();
    }
}