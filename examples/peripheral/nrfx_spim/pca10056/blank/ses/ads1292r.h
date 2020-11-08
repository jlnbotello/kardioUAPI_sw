/**
 * @file ads1292r.h
 * @brief Texas Instruments Analog Front End(AFE) ADS19292R driver.
 *
 * @date 	30/10/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

#ifndef ADS1292R_H
#define ADS1292R_H
/*==================[INCLUSIONS]=============================================*/
#include "stdint.h"
#include "kardioUAPI_v1_0.h"
/*==================[MACROS AND DEFINITIONS]=================================*/
/*
#define ADS1292R_PIN_MISO       4
#define ADS1292R_PIN_MOSI       40
#define ADS1292R_PIN_SCK        41
#define ADS1292R_PIN_DRDY       11  // Button 1
#define ADS1292R_PIN_CS         13  // Led 1   
#define ADS1292R_PIN_START      14  // Led 2   
#define ADS1292R_PIN_RESET      15  // Led 3   
#define ADS1292R_PIN_PDOWN      16  // Led 4   
*/
#define ADS1292R_PIN_MISO       KARDIOUAPI_AFE_SPI_MISO 
#define ADS1292R_PIN_MOSI       KARDIOUAPI_AFE_SPI_MISO
#define ADS1292R_PIN_SCK        KARDIOUAPI_AFE_SPI_CLK
#define ADS1292R_PIN_DRDY       KARDIOUAPI_AFE_A_DRDY_INT
#define ADS1292R_PIN_CS         KARDIOUAPI_AFE_A_SPI_CS   
#define ADS1292R_PIN_START      KARDIOUAPI_AFE_AB_START   
#define ADS1292R_PIN_RESET      KARDIOUAPI_AFE_A_PWD_RST   



/*==================[TYPEDEF]================================================*/
typedef enum {
  ADS1292R_RATE_125HZ,
  ADS1292R_RATE_250HZ,
  ADS1292R_RATE_500HZ,
  ADS1292R_RATE_1KHZ,
  ADS1292R_RATE_2KHZ,
  ADS1292R_RATE_4KHZ,
  ADS1292R_RATE_8KHZ
} ADS1292R_Rate_t;

typedef enum {
  ADS1292R_GAIN_6,
  ADS1292R_GAIN_1,
  ADS1292R_GAIN_2,
  ADS1292R_GAIN_3,
  ADS1292R_GAIN_4,
  ADS1292R_GAIN_8,
  ADS1292R_GAIN_12
} ADS1292R_Gain_t;

typedef enum {
  ADS1292R_CH_1,
  ADS1292R_CH_2
} ADS1292R_Channel_t;

typedef uint8_t ADS1292R_RetCode_t;


extern volatile bool spi_is_busy;

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
/**
 *  @brief 
 *
 *  @param[in]    a		Param a is...
 *  @param[in]    b		Param b is...
 *  @param[inout] c		Param c is...
 *
 *  @retval       ret           Meaning of ret
 */
ADS1292R_RetCode_t ADS1292R_Init();
void ADS1292R_HW_Reset();
void ADS1292R_HW_Stop();



/*==================[END OF FILE]============================================*/
#endif /* ADS1292R_H */