/**
 * @file afe_port.h
 * @brief 
 *
 * @date 	22/11/20
 * @author 	Juli�n Botello <jlnbotello@gmail.com>
 */

#ifndef AFE_PORT_H
#define AFE_PORT_H
/*==================[INCLUSIONS]=============================================*/
#include "application_protocol.h"
#include "ads1292r.h"

/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[TYPEDEF]================================================*/
typedef struct {    
    void (*afe_data_handler)(void);
} AFEPort_AFEEvent_t;

/*==================[EXTERNAL FUNCTIONS DECLARATION]=========================*/
void AfePort_Init(AP_Instance_t * ap_instance);
void AFEPort_AFEDataHandler();
/*==================[END OF FILE]============================================*/
#endif /* AFE_PORT_H */