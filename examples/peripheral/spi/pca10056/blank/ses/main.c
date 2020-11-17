/**
 * Copyright (c) 2015 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
//#include "nrf_drv_spi.h"
#include "app_util_platform.h"
//#include "nrf_gpio.h"
//#include "nrf_delay.h"
//#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "ads1292r.h"
#include "nrf_delay.h"

int main(void){
  ADS1292R_RetCode_t ret_code;
  
  nrf_gpio_cfg_output(KARDIOUAPI_LED1_R);
  nrf_gpio_cfg_output(KARDIOUAPI_LED1_G);
  nrf_gpio_cfg_output(KARDIOUAPI_LED1_B);
  
  nrf_gpio_cfg_output(KARDIOUAPI_AFE_B_PWD_RST);
  nrf_gpio_pin_set(KARDIOUAPI_AFE_B_PWD_RST);
  nrf_gpio_cfg_output(KARDIOUAPI_AFE_B_SPI_CS);
  nrf_gpio_pin_set(KARDIOUAPI_AFE_B_SPI_CS);
 
  ret_code = ADS1292R_Init();

  ADS1292R_HW_Start();

  if (ADS1292R_RETCODE_SUCESS == ret_code) {
    nrf_gpio_pin_set(KARDIOUAPI_LED1_G);
    nrf_delay_ms(1000);
    nrf_gpio_pin_clear(KARDIOUAPI_LED1_G);
  } else {
    nrf_gpio_pin_set(KARDIOUAPI_LED1_R);
  }
  ADS1292R_SpiPacket_t pkt;
  while(1){
    if(ADS1292R_NewSamples()){
      ADS1292R_ReadSamples(&pkt);
    }
    nrf_gpio_pin_toggle(KARDIOUAPI_LED1_B);
    nrf_delay_ms(10);
    nrf_gpio_pin_toggle(KARDIOUAPI_LED1_B);
    nrf_delay_ms(200);
    nrf_gpio_pin_toggle(KARDIOUAPI_LED1_B);
    nrf_delay_ms(10);
    nrf_gpio_pin_toggle(KARDIOUAPI_LED1_B);
    nrf_delay_ms(2000);
  }

}