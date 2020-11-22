#ifndef LED_PORT_H
#define LED_PORT_H
/*==================[INCLUSIONS]=============================================*/
#include "application_protocol.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define LED_PORT_SUPERLOOP_IMPLEMENTATION     1
#define LED_PORT_SCHEDULER_IMPLEMENTATION     2
#define LED_PORT_IMPLEMENTATION_TYPE          LED_PORT_SCHEDULER_IMPLEMENTATION
/*==================[TYPEDEF]================================================*/

/*==================[EXTERNAL FUNCTIONS DECLARATION]=========================*/
void LedPort_Init(AP_Instance_t * ap_instance);
void LedPort_RxHandler();
/*==================[END OF FILE]============================================*/
#endif /* LED_PORT_H */