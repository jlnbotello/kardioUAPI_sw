/**
 * @file ads1292r.c
 * @brief Texas Instruments Analog Front End(AFE) ADS19292R driver.
 *
 * @date 	30/10/20
 * @author 	Julián Botello <jlnbotello@gmail.com>
 */

/*==================[INCLUSIONS]=============================================*/
#include "app_error.h"
#include "ads1292r.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrfx_spim.h"
#include "nrfx_gpiote.h"

/*==================[MACROS AND DEFINITIONS]=================================*/
#define ADS1292R_RETCODE_SUCESS       0x00
#define ADS1292R_RETCODE_FAIL         0x01

#define ADS1292R_RO_ID                0x73  /**< Read-Only ID for ADS1292R (Factory-Programmed) */
#define ADS1292R_NOF_REGS             12    /**< Number of registers */
/* System Commands */
#define ADS1292R_CMD_WAKEUP           0x02  /**< Wake-up from standby mode */
#define ADS1292R_CMD_STANDBY          0x04  /**< Enter standby mode */
#define ADS1292R_CMD_RESET            0x06  /**< Reset the device*/
#define ADS1292R_CMD_START            0x08  /**< Start or restart (synchronize) conversions */
#define ADS1292R_CMD_STOP             0x0A  /**< Stop conversion */
#define ADS1292R_CMD_OFFSETCAL        0x1A  /**< Channel offset calibration */

/* Data Read Commands */
#define ADS1292R_CMD_RDATAC           0x10  /**< Enable Read Data Continuous mode. */
#define ADS1292R_CMD_SDATAC           0x11  /**< Stop Read Data Continuously mode */
#define ADS1292R_CMD_RDATA            0x12  /**< Read data by command; supports multiple read back. */

/* Register Read/Write Commands */
#define ADS1292R_CMD_RREG_FB(r_rrrr)  ((uint8_t)(0x20 | (0x1F & r_rrrr))) /**< r_rrrr = starting register address for read    */
#define ADS1292R_CMD_RREG_SB(n_nnnn)  ((uint8_t)(0x00 | (0x1F & n_nnnn))) /**< n_nnnn = number of registers to be read – 1    */
#define ADS1292R_CMD_WREG_FB(r_rrrr)  ((uint8_t)(0x40 | (0x1F & r_rrrr))) /**< r_rrrr = starting register address for write   */
#define ADS1292R_CMD_WREG_SB(n_nnnn)  ((uint8_t)(0x00 | (0x1F & n_nnnn))) /**< n_nnnn = number of registers to be written – 1 */

/* Register addresses */
#define ADS1292R_REG_ID               0x00
#define ADS1292R_REG_CONFIG1          0x01
#define ADS1292R_REG_CONFIG2          0x02
#define ADS1292R_REG_LOFF             0x03
#define ADS1292R_REG_CH1SET           0x04
#define ADS1292R_REG_CH2SET           0x05
#define ADS1292R_REG_RLDSENS          0x06
#define ADS1292R_REG_LOFFSENS         0x07
#define ADS1292R_REG_LOFFSTAT         0x08
#define ADS1292R_REG_RESP1            0x09
#define ADS1292R_REG_RESP2            0x0A
#define ADS1292R_GPIO                 0x0B

#define ADS1292R_GAIN_MASK(gain)      ((uint8_t)(0x70 & (gain << 4)))
#define ADS1292R_RATE_MASK(rate)      ((uint8_t)(0x07 & rate))

#define ADS1292R_SPI_BUFFER_SIZE      10
#define ADS1292R_DUMMY_BYTE           0x00

/*==================[TYPEDEF]================================================*/
typedef union {
  struct {
    uint8_t id;
    uint8_t config1;
    uint8_t config2;
    uint8_t loff;
    uint8_t ch1set;
    uint8_t ch2set;
    uint8_t rld_sens;
    uint8_t loff_sens;
    uint8_t loff_stat;
    uint8_t resp1;
    uint8_t resp2;
    uint8_t gpio;
  } by_field;

  uint8_t by_index[ADS1292R_NOF_REGS];
} __attribute__((packed)) ADS1292R_Regs_t;

typedef union {
  struct {
    uint8_t status[3];
    uint8_t channel_1[3];
    uint8_t channel_2[3];
  } by_field;
  uint8_t by_index[9];
} __attribute__((packed)) ADS1292R_SpiPacket_t;

    /*==================[INTERNAL DATA DEFINITION]===============================*/
    int a_var; /**< Inline comment for a_var */
static ADS1292R_Regs_t device_reg;
static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(3); /**< SPI instance 0. */
static uint8_t m_tx_buf[ADS1292R_SPI_BUFFER_SIZE];
static uint8_t m_rx_buf[ADS1292R_SPI_BUFFER_SIZE];
volatile bool spi_is_busy = false; /**< Flag used to indicate that SPI instance completed the transfer. */

/*==================[INTERNAL FUNCTIONS DECLARATION]=========================*/
void on_data_rdy(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
/*==================[INTERNAL FUNCTIONS DEFINITION]==========================*/
void spim_event_handler(nrfx_spim_evt_t const * p_event, void * p_context) {
  spi_is_busy = false;
  NRF_LOG_INFO("Transfer completed.");
}

static void init_spi() {
  static nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
  spi_config.frequency      = NRF_SPIM_FREQ_1M;
  spi_config.miso_pin       = ADS1292R_PIN_MISO;
  spi_config.mosi_pin       = ADS1292R_PIN_MOSI;
  spi_config.sck_pin        = ADS1292R_PIN_SCK;
  spi_config.use_hw_ss      = false;
  //APP_ERROR_CHECK(nrfx_spim_init(&spi, &spi_config, spim_event_handler, NULL));
  nrfx_spim_init(&spi, &spi_config, spim_event_handler, NULL);
  nrf_gpio_cfg_output(44);
  nrf_gpio_pin_write(44, 1); //FIXME: dev only. disable eeprom on laird dvk
}

static void init_gpio() {
  /* SPI CS Pin Init */
  nrf_gpio_cfg_output(ADS1292R_PIN_CS);
  /* Start Pin Init */
  nrf_gpio_cfg_output(ADS1292R_PIN_START);
  /* Reset Pin Init */
  nrf_gpio_cfg_output(ADS1292R_PIN_RESET);
   
  /* Data Ready Pin Init */
  ret_code_t err_code;
  err_code = nrfx_gpiote_init();
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
  in_config.pull = NRF_GPIO_PIN_PULLUP;
  err_code = nrfx_gpiote_in_init(ADS1292R_PIN_DRDY, &in_config, on_data_rdy);
  //APP_ERROR_CHECK(err_code);
  nrfx_gpiote_in_event_enable(ADS1292R_PIN_DRDY, true);
}

static void set_high(uint8_t pin) {
  nrf_gpio_pin_write(pin, 1);
}

static void set_low(uint8_t pin) {
  nrf_gpio_pin_write(pin, 0);
}

static void spi_transfer(uint8_t *data_src, uint8_t length) {
  nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(m_tx_buf, length, m_rx_buf, length);
  if (length <= ADS1292R_SPI_BUFFER_SIZE && !spi_is_busy) {
    spi_is_busy = true;
    memset(m_rx_buf, 0, length);
    memcpy(m_tx_buf, data_src, length);
    APP_ERROR_CHECK(nrfx_spim_xfer_dcx(&spi, &xfer_desc,0, length));
  }
}

static void spi_transfer_byte(uint8_t data) {
  spi_transfer(&data, 1);
}

static void write_reg(uint8_t address, uint8_t data) {
  set_low(ADS1292R_PIN_CS);
  spi_transfer_byte(ADS1292R_CMD_WREG_FB(address));
  spi_transfer_byte(ADS1292R_CMD_WREG_FB(address));
  spi_transfer_byte(ADS1292R_CMD_WREG_SB(0));
  spi_transfer_byte(ADS1292R_CMD_WREG_SB(data));
  set_high(ADS1292R_PIN_CS);
}

static uint8_t read_reg(uint8_t address) {
  uint8_t ret_val = 0x00;
  // ret_val = spi_receive(address)
  return ret_val;
}

static void read_n_regs(uint8_t address, uint8_t *data, uint8_t n) {
  set_low(ADS1292R_PIN_CS);
  spi_transfer_byte(ADS1292R_CMD_RREG_FB(address));
  spi_transfer_byte(ADS1292R_CMD_RREG_SB(n-1));
  for(uint8_t i=0; i<n; i++){
    spi_transfer_byte(ADS1292R_DUMMY_BYTE);
  }
  set_high(ADS1292R_PIN_CS);
}

void on_data_rdy(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
  
}

/*==================[EXTERNAL FUNCTIONS DEFINITION]==========================*/
ADS1292R_RetCode_t ADS1292R_Init() {

  ADS1292R_RetCode_t ret_val = ADS1292R_RETCODE_FAIL;

  /* Init communication interface */
  init_spi();
  init_gpio();

  /* AFE Init */
  //TODO
  ADS1292R_HW_Reset();
  nrf_delay_ms(100);
  ADS1292R_HW_Stop();
  //

  /* Read default registers */
  read_n_regs(ADS1292R_REG_ID, device_reg.by_index, ADS1292R_NOF_REGS);

  /* Check device availability */
  if (device_reg.by_field.id == ADS1292R_RO_ID)
    ret_val = ADS1292R_RETCODE_SUCESS;

  return ret_val;
}

void ADS1292R_ReadPacket() {
  //cs low
  //read 9 bytes
  //cs high
}

void ADS1292R_HW_Start() {
  nrf_gpio_pin_write(ADS1292R_PIN_START, 0);
}

void ADS1292R_HW_Stop() {
  nrf_gpio_pin_write(ADS1292R_PIN_START, 1);
}

void ADS1292R_HW_Reset(){
  nrf_gpio_pin_write(ADS1292R_PIN_START, 1);
  nrf_delay_ms(100);
  nrf_gpio_pin_write(ADS1292R_PIN_START, 0);
  nrf_delay_ms(100);
  nrf_gpio_pin_write(ADS1292R_PIN_START, 1);
  nrf_delay_ms(100);
}

void ADS1292R_SetGain(ADS1292R_Channel_t ch, ADS1292R_Gain_t gain) {
  switch (ch) {
  case ADS1292R_CH_1:
    write_reg(ADS1292R_REG_CH1SET, device_reg.by_field.ch1set | ADS1292R_GAIN_MASK(gain));
    break;
  case ADS1292R_CH_2:
    write_reg(ADS1292R_REG_CH2SET, device_reg.by_field.ch2set | ADS1292R_GAIN_MASK(gain));
    break;
  default:
    break;
  }
}

void ADS1292R_SetRate(ADS1292R_Rate_t rate) {
  write_reg(ADS1292R_REG_CONFIG1, device_reg.by_field.config1 | ADS1292R_RATE_MASK(rate));
}

/*==================[END OF FILE]============================================*/