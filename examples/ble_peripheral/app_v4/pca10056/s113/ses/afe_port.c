/**
 * @file afe_port.c
 * @brief
 *
 * @date 	22/11/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*==================[INCLUSIONS]=============================================*/
 #include "afe_port.h"
 #include "afe_port_defs.h"
  #include "error_handling.h"

/*==================[MACROS AND DEFINITIONS]=================================*/

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/
static void on_tx_event(AP_RetVal_t  ret);

static void send(uint8_t * data, uint8_t length);

static void process_rx_multi_byte(uint8_t port, uint8_t *data, uint8_t length);

static void rx_handler();

/*==================[INTERNAL DATA DEFINITION]===============================*/
static AP_PortCallbacks_t afe_port_cbs = {      
        .port = AFE_PORT,                       
        .rx_cb = process_rx_multi_byte,         
        .tx_cb = on_tx_event,                   
};
    
static AP_TransferStatus_t rx_status;
static AP_TransferStatus_t tx_status;

static AP_Instance_t * api;

static afe_port_32bit_data_t samples32 = {
    .id = AFE_PORT_DATA_PKT_ID
};

static afe_port_16bit_data_t samples16 = {
    .id = AFE_PORT_DATA_PKT_ID
};

static afe_port_cfg_t  config;


static uint8_t * rx_buffer = NULL;
static uint8_t rx_data_len = 0;

/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
static void on_tx_event(AP_RetVal_t  ret){
	if(ret == AP_SUCCESS)
		tx_status = AP_FREE;
}
            
static void send(uint8_t * data, uint8_t length){
	if(tx_status == AP_FREE){
		AP_SendPacket(api, AFE_PORT, data, length);
		tx_status = AP_BUSY;
	}
}

static void process_rx_multi_byte(uint8_t port, uint8_t *data, uint8_t length) {
    if (rx_status == AP_FREE) {
        rx_status = AP_BUSY;
        rx_data_len = length;
        rx_buffer = data;        
        rx_handler();
    }
}

static void rx_handler() {
    if (rx_status == AP_BUSY) {
        switch (rx_buffer[AFE_PORT_ID_POS]) {
        case AFE_PORT_CMD_START_ID:
            //if (rx_data_len == AFE_PORT_CMD_LEN) {
                ADS1292R_HW_Start();
            //} else {
                // Wrong length!
            //}
            break;
        case AFE_PORT_CMD_STOP_ID:
            //if (rx_data_len == AFE_PORT_CMD_LEN) {
                ADS1292R_HW_Stop();
            //} else {
                // Wrong length!
            //}
            break;

        default:
            // Not recognized packet
            break;
        }
        rx_status = AP_FREE;
    }
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
void AfePort_Init(AP_Instance_t * ap_instance){
    api = ap_instance;
    rx_status = AP_FREE;
    tx_status = AP_FREE;
    ASSERT(true == AP_AddCallbacks(api, afe_port_cbs));
}

#define CONV_INT24_TO_INT32(x)     ((int32_t)(x[2]<<16)+(x[1]<<8)+(x[0]))
#define CONV_INT24_TO_INT16(x)     ((int16_t)(x[1]<<8)+(x[0]))

void AFEPort_AFEDataHandler(){
    ADS1292R_SpiPacket_t const * const p_afe_pkt = ADS1292R_GetPacketHandle();
    
    
    samples16.sample_ch1 = CONV_INT24_TO_INT16(p_afe_pkt->by_field.channel_1);
    samples16.sample_ch2 = CONV_INT24_TO_INT16(p_afe_pkt->by_field.channel_2);
    ADS1292R_ReleasePacket();
    send((uint8_t *)&samples16, 5);
    
    
    
}
/*==================[END OF FILE]============================================*/