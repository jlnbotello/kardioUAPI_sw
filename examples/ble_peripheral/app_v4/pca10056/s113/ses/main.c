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
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "kardioUAPI_v1_0.h"
#include "app_button.h"

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
//FIXME
/**
Call after ads and leds init. because it inits the gpiote
*/
static void supercap_ready(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    Led_Toggle(LED_R);
    Led_On(LED_G);
}

static void back_up_supply_init(){
  nrf_gpio_cfg_output(KARDIOUAPI_BACK_UP_ENABLE);
  nrf_gpio_pin_set(KARDIOUAPI_BACK_UP_ENABLE);
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
  in_config.pull = NRF_GPIO_PIN_PULLUP;
  uint32_t err_code = nrfx_gpiote_in_init(KARDIOUAPI_BACK_UP_READY, &in_config, supercap_ready);
  APP_ERROR_CHECK(err_code);
  nrfx_gpiote_in_event_enable(KARDIOUAPI_BACK_UP_READY, true);
}

static void on_push_ev_handler(uint8_t pin_no, uint8_t button_action){
    nrf_gpio_pin_clear(KARDIOUAPI_POWER_LATCH);
    nrf_gpio_pin_clear(KARDIOUAPI_BACK_UP_ENABLE);
    Led_AllOff();
}
#define BUTTON_DETECTION_DELAY APP_TIMER_TICKS(5000) // MS TO TICKS

/**@brief Function for initializing the button handler module.
 */
static void button_init(void)
{
    ret_code_t err_code;

    //The array must be static because a pointer to it will be saved in the button handler module.
    static app_button_cfg_t buttons[] =
    {
        {KARDIOUAPI_BUTTON1, APP_BUTTON_ACTIVE_HIGH, NRF_GPIO_PIN_NOPULL, on_push_ev_handler}
    };

    err_code = app_button_init(buttons, ARRAY_SIZE(buttons),
                               BUTTON_DETECTION_DELAY);
    APP_ERROR_CHECK(err_code);
    
    app_button_enable();
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
    back_up_supply_init();
    //LATCH POWER
    nrf_gpio_cfg_output(KARDIOUAPI_POWER_LATCH);
    nrf_gpio_pin_set(KARDIOUAPI_POWER_LATCH);
    button_init();
    LedPort_Init(p_api);
    AfePort_Init(p_api);    
    SchedulerCfg_Init();
    // will change to green when supercap is ready
    Led_On(LED_R);

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