#include "led.h"
#include "kardioUAPI_v1_0.h"
#include "nrf_gpio.h"


#if LEDS_NUMBER > 0
static const uint8_t m_board_led_list[LEDS_NUMBER] = LEDS_LIST;
#endif

void Led_Init(){
    for (uint8_t i = 0; i < LEDS_NUMBER; ++i)
    {
        uint8_t led = m_board_led_list[i];
        nrf_gpio_cfg_output(led);
    }
    Led_AllOff();
}

void Led_On(LedId_t led_idx){
    if(led_idx < LEDS_NUMBER)
        nrf_gpio_pin_write(m_board_led_list[led_idx], LEDS_ACTIVE_STATE ? 1 : 0);

}

void Led_Off(LedId_t led_idx){
    if(led_idx < LEDS_NUMBER)
        nrf_gpio_pin_write(m_board_led_list[led_idx], LEDS_ACTIVE_STATE ? 0 : 1);
}

void Led_Toggle(LedId_t led_idx){
    if(led_idx < LEDS_NUMBER)
        nrf_gpio_pin_toggle(m_board_led_list[led_idx]);
}

void Led_AllOff(){
  for (uint8_t i = 0; i < LEDS_NUMBER; ++i){
    Led_Off(i);
  }
}