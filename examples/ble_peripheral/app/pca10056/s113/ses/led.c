#include "led.h"
#include "bl654_dk.h"

void Led_Init(){
    for (uint8_t i = 0; i < LEDS_NUMBER; ++i)
    {
        nrf_gpio_cfg_output(m_board_led_list[i]);
    }
    Led_AllOff();
}

void Led_On(LedId_t led_idx){
    ASSERT(led_idx < LEDS_NUMBER);
    nrf_gpio_pin_write(m_board_led_list[led_idx], LEDS_ACTIVE_STATE ? 1 : 0);
}

void Led_Off(LedId_t led_idx){
    ASSERT(led_idx < LEDS_NUMBER);
    nrf_gpio_pin_write(m_board_led_list[led_idx], LEDS_ACTIVE_STATE ? 0 : 1);
}

void Led_Toggle(LedId_t led_idx){
   ASSERT(led_idx < LEDS_NUMBER);
   nrf_gpio_pin_toggle(m_board_led_list[led_idx]);
}

void Led_AllOff(){
  for (uint8_t i = 0; i < LEDS_NUMBER; ++i){
    Led_Off(i);
  }
}