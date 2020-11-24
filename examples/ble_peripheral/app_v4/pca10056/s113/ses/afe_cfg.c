/**
 * @file a_template.c
 * @brief This a source template for reuse
 *
 * @date 	23/11/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*==================[INCLUSIONS]=============================================*/
 #include "afe_cfg.h"
 #include "afe_port.h"
 #include "ads1292r.h"
 #include "app_scheduler.h"
 #include "error_handling.h"

/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/

/*==================[INTERNAL DATA DEFINITION]===============================*/
static AFEPort_AFEEvent_t afe_event = {
    .afe_data_handler = AFEPort_AFEDataHandler
};

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static void sched_afe_new_data_handler(void *p_event, uint16_t event_size){
    AFEPort_AFEEvent_t *const p_ev = (AFEPort_AFEEvent_t *) p_event;
    p_ev->afe_data_handler();
}

static void new_samples_handler(){
    uint32_t err_code = app_sched_event_put(&afe_event, sizeof(AFEPort_AFEEvent_t), sched_afe_new_data_handler);
    APP_ERROR_CHECK(err_code);
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
void AfeCfg_Init() {
    ADS1292R_RetCode_t ret = ADS1292R_Init(new_samples_handler);    
    ASSERT(ret == ADS1292R_RETCODE_SUCESS);
}



/*==================[END OF FILE]============================================*/