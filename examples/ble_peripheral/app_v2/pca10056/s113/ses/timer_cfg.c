/*==================[INCLUSIONS]=============================================*/
#include "timer_cfg.h"
#include "app_timer.h"

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
/**@brief Function for initializing the timer module.
 */
void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

/*==================[END OF FILE]============================================*/