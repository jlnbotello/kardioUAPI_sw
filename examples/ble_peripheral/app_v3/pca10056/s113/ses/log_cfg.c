/*==================[INCLUSIONS]=============================================*/
#include "log_cfg.h"
#include "nrf_log_default_backends.h"

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
/**@brief Function for initializing the nrf log module.
 */
void LogCfg_Init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/*==================[END OF FILE]============================================*/