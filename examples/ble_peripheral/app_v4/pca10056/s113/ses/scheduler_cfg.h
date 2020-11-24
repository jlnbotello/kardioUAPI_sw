#ifndef SCHEDULER_CFG_H
#define SCHEDULER_CFG_H
/*==================[INCLUSIONS]=============================================*/
#include "tp_cfg.h"
#include "app_scheduler.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(TpCfg_RxEvent_t)/**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE                2                    /**< Maximum number of events in the scheduler queue. */

/*==================[TYPEDEF]================================================*/

/*==================[EXTERNAL FUNCTIONS DECLARATION]=========================*/
void SchedulerCfg_Init(void);

/*==================[END OF FILE]============================================*/
#endif /* SCHEDULER_CFG_H */