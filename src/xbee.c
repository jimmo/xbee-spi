#include "xbee.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#define XBEE_SPI_PORT GPIOB
#define XBEE_SPI_MOSI GPIO15
#define XBEE_SPI_MISO GPIO14
#define XBEE_SPI_SCK GPIO13
#define XBEE_SPI_NSS GPIO12
/* #define XBEE_SPI_MOSI GPIO7 */
/* #define XBEE_SPI_MISO GPIO6 */
/* #define XBEE_SPI_SCK GPIO5 */
/* #define XBEE_SPI_NSS GPIO4 */

#define XBEE_ATTN_PORT GPIOC
#define XBEE_ATTN GPIO12

void xbee_init(void) {
  // Enable SPI periperal clock
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_SPI2);

  // Set pin mode for SPI managed pins to alternate function
  gpio_mode_setup(XBEE_SPI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE,
                  XBEE_SPI_SCK
                  | XBEE_SPI_MOSI
                  | XBEE_SPI_MISO
                  | XBEE_SPI_NSS
                  );

  // Set alternate function for SPI managed pins to AF5 for SPI2
  gpio_set_af(XBEE_SPI_PORT, GPIO_AF5,
              XBEE_SPI_SCK
              | XBEE_SPI_MOSI
              | XBEE_SPI_MISO
              | XBEE_SPI_NSS
              );

  // Initialize SPI2 as master
  spi_init_master(
                  SPI2,
                  SPI_CR1_BAUDRATE_FPCLK_DIV_64,
                  SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,   // CPOL: Clock low when idle
                  SPI_CR1_CPHA_CLK_TRANSITION_1,     // CPHA: Clock phase: read on rising edge of clock
                  SPI_CR1_DFF_8BIT,
                  SPI_CR1_MSBFIRST);

  spi_disable_crc(SPI2);

  // Bit Order: Send MSB first
  // Clock Phase (CPHA): Sample data on first (leading) edge
  //  Clock Polarity (CPOL): First (leading) edge rises
  spi_send_msb_first(SPI2);
  spi_set_clock_phase_0(SPI2);
  spi_set_clock_polarity_0(SPI2);

  // Have SPI peripheral manage NSS pin (pulled low when SPI enabled)
  spi_enable_ss_output(SPI2);
}

void xbee_start(void) {
  spi_enable(SPI2);
}

void xbee_stop(void) {
  spi_disable(SPI2);
}

uint8_t xbee_transfer(uint8_t data) {
  return spi_xfer(SPI2, data);
}
