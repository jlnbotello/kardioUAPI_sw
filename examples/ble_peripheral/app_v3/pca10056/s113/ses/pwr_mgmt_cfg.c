/*==================[INCLUSIONS]=============================================*/
#include "pwr_mgmt_cfg.h"
#include "nrf_pwr_mgmt.h"
#include "error_handling.h"
#include "nrf_log_ctrl.h"
#include "bsp_btn_ble.h"

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
/**@brief Function for initializing power management.
 */
void PwrMgmtCfg_Init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling the idle state (main loop).
*
*  @details If there is no pending log operation, then sleep until next the next event occurs.
*/
void PwrMgmtCfg_IdleStateHandle(void){
    if (NRF_LOG_PROCESS() == false) //nothing else to process
    {
        nrf_pwr_mgmt_run();
    }
}

/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
void PwrMgmtCfg_SleepMode(void)
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}

/*==================[END OF FILE]============================================*/