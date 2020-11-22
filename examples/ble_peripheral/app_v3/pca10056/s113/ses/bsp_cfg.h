#ifndef BSP_CFG_H
#define BSP_CFG_H
/*==================[INCLUSIONS]=============================================*/
#include "stdbool.h"
#include "ble_advertising.h"

/*==================[EXTERNAL FUNCTIONS DECLARATION]=========================*/
void BSPCfg_Init(ble_advertising_t * p_advertising_, uint16_t * p_conn_handle_);
void BSPCfg_ButtonsInit(bool * p_erase_bonds);

/*==================[END OF FILE]============================================*/
#endif /* BSP_CFG_H */