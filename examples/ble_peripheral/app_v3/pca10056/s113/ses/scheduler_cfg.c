/*==================[INCLUSIONS]=============================================*/
#include "scheduler_cfg.h"


/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL DATA DEFINITION]===============================*/

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
/**@brief Function for the Event Scheduler initialization.
 */
void SchedulerCfg_Init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}



/*==================[END OF FILE]============================================*/