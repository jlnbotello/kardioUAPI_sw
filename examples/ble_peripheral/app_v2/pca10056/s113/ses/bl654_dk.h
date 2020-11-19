#ifndef BL654_DK_H
#define PBL654_DK_H

#include "nrf_gpio.h"

#define SIO(n)			NRF_GPIO_PIN_MAP((n<=31)?0:1,(n<=31)?n:(n-32))

#define SIO_00			NRF_GPIO_PIN_MAP(0,0)
#define SIO_01			NRF_GPIO_PIN_MAP(0,1)
#define SIO_02			NRF_GPIO_PIN_MAP(0,2)
#define SIO_03			NRF_GPIO_PIN_MAP(0,3)
#define SIO_04			NRF_GPIO_PIN_MAP(0,4)
#define SIO_05			NRF_GPIO_PIN_MAP(0,5)
#define SIO_06			NRF_GPIO_PIN_MAP(0,6)
#define SIO_07			NRF_GPIO_PIN_MAP(0,7)
#define SIO_08			NRF_GPIO_PIN_MAP(0,8)			
#define SIO_09			NRF_GPIO_PIN_MAP(0,9)	
#define SIO_10			NRF_GPIO_PIN_MAP(0,10)	
#define SIO_11			NRF_GPIO_PIN_MAP(0,11)
#define SIO_12			NRF_GPIO_PIN_MAP(0,12)
#define SIO_13			NRF_GPIO_PIN_MAP(0,13)
#define SIO_14			NRF_GPIO_PIN_MAP(0,14)
#define SIO_15			NRF_GPIO_PIN_MAP(0,15)
#define SIO_16			NRF_GPIO_PIN_MAP(0,16)
#define SIO_17			NRF_GPIO_PIN_MAP(0,17)
#define SIO_18			NRF_GPIO_PIN_MAP(0,18)
#define SIO_19			NRF_GPIO_PIN_MAP(0,19)
#define SIO_20			NRF_GPIO_PIN_MAP(0,20)
#define SIO_21			NRF_GPIO_PIN_MAP(0,21)
#define SIO_22			NRF_GPIO_PIN_MAP(0,22)
#define SIO_23			NRF_GPIO_PIN_MAP(0,23)
#define SIO_24			NRF_GPIO_PIN_MAP(0,24)
#define SIO_25			NRF_GPIO_PIN_MAP(0,25)
#define SIO_26			NRF_GPIO_PIN_MAP(0,26)
#define SIO_27			NRF_GPIO_PIN_MAP(0,27)
#define SIO_28			NRF_GPIO_PIN_MAP(0,28)	
#define SIO_29			NRF_GPIO_PIN_MAP(0,29)	
#define SIO_30			NRF_GPIO_PIN_MAP(0,30)	
#define SIO_31			NRF_GPIO_PIN_MAP(0,31)	
#define SIO_32			NRF_GPIO_PIN_MAP(1,0)
#define SIO_33			NRF_GPIO_PIN_MAP(1,1)
#define SIO_34			NRF_GPIO_PIN_MAP(1,2)
#define SIO_35			NRF_GPIO_PIN_MAP(1,3)
#define SIO_36			NRF_GPIO_PIN_MAP(1,4)
#define SIO_37			NRF_GPIO_PIN_MAP(1,5)	
#define SIO_38			NRF_GPIO_PIN_MAP(1,6)
#define SIO_39			NRF_GPIO_PIN_MAP(1,7)
#define SIO_40			NRF_GPIO_PIN_MAP(1,8)		
#define SIO_41			NRF_GPIO_PIN_MAP(1,9)
#define SIO_42			NRF_GPIO_PIN_MAP(1,10)	
#define SIO_43			NRF_GPIO_PIN_MAP(1,11)	
#define SIO_44			NRF_GPIO_PIN_MAP(1,12)	
#define SIO_45			NRF_GPIO_PIN_MAP(1,13)	
#define SIO_46			NRF_GPIO_PIN_MAP(1,14)	
#define SIO_47			NRF_GPIO_PIN_MAP(1,15)	
#define SIO_48			NRF_GPIO_PIN_MAP(1,16)	
#define SIO_49			NRF_GPIO_PIN_MAP(1,17)	
#define SIO_50			NRF_GPIO_PIN_MAP(1,18)	
#define SIO_51			NRF_GPIO_PIN_MAP(1,19)	
#define SIO_52			NRF_GPIO_PIN_MAP(1,20)	
#define SIO_53			NRF_GPIO_PIN_MAP(1,21)	
#define SIO_54			NRF_GPIO_PIN_MAP(1,22)	
#define SIO_55			NRF_GPIO_PIN_MAP(1,23)	
#define SIO_56			NRF_GPIO_PIN_MAP(1,24)	
#define SIO_57			NRF_GPIO_PIN_MAP(1,25)	
#define SIO_58			NRF_GPIO_PIN_MAP(1,26)	
#define SIO_59			NRF_GPIO_PIN_MAP(1,27)	
#define SIO_60			NRF_GPIO_PIN_MAP(1,28)	
#define SIO_61			NRF_GPIO_PIN_MAP(1,29)	
#define SIO_62			NRF_GPIO_PIN_MAP(1,30)	
#define SIO_63			NRF_GPIO_PIN_MAP(1,31)	
	
#define LEDS_NUMBER             4

#define PIN_LED_1		SIO_13
#define PIN_LED_2		SIO_14
#define PIN_LED_3 		SIO_15
#define PIN_LED_4		SIO_16
#define PIN_LED_START           PIN_LED_1
#define PIN_LED_STOP            PIN_LED_4
#define LEDS_LIST               {PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_4}

#if LEDS_NUMBER > 0
static const uint8_t m_board_led_list[LEDS_NUMBER] = LEDS_LIST;
#endif

#define LEDS_ACTIVE_STATE 1


#endif //BL654_DK_H