/**
 * @file ap_cfg.h
 * @brief Application Protocol Configuration Interface
 *
 * @date 	21/11/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

#ifndef AP_CFG_H
#define AP_CFG_H
/*==================[INCLUSIONS]=============================================*/
#include "transport_protocol.h"
#include "application_protocol.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define AP_CFG_N_OF_PORTS     3     /**< Number of ports */
/*==================[TYPEDEF]================================================*/

/*==================[EXTERNAL FUNCTIONS DECLARATION]=========================*/
void ApCfg_Init(TP_Instance_t *p_tpi_);
AP_Instance_t * ApCfg_GetInstance();

/*==================[END OF FILE]============================================*/
#endif /* AP_CFG_H */
