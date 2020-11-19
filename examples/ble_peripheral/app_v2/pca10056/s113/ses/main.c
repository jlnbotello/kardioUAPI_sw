#include "ble_cfg.h"
#include "bsp_cfg.h"
#include "log_cfg.h"
#include "nus_cfg.h"
#include "power_cfg.h"
#include "timer_cfg.h"
#include "uart_cfg.h"

#include "led.h"
#include "rc_led.h"
#include "transport_protocol.h"
#include "application_protocol.h"

AP_CREATE_CALLBACK_TABLE(RC_Led,RC_Led);
TP_CREATE_CALLBACK_TABLE(AP_LowerCallback);
    
/**@brief Application main function.
 */
int main(void)
{
    bool erase_bonds;

    // Initialize.
    Led_Init();       
    uart_init(nus_get_instance(),  get_conn_handle());
    log_init();
    timers_init();
    bsp_cfg_init(get_ble_advertiser(), get_conn_handle());
    buttons_leds_init(&erase_bonds);
    power_management_init();   
    ble_cfg_init();

    // Start execution.
    NRF_LOG_INFO("Debug logging for UART over RTT started.");
    advertising_start();
  
    Led_On(L1);
    Led_On(L3);

    // Enter main loop.
    for (;;)
    { 
        idle_state_handle();
    }
}
