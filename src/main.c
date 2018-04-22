#include "cbuf.h"

#include "host.h"
#include "xbee.h"

#include <stdlib.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void gpio_setup(void) {
  // LED (A8)
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);

  // Attn (C12):
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO12);
}

struct cbuf_t host_to_xbee;
struct cbuf_t xbee_to_host;

int main(void) {
  int i;

  rcc_clock_setup_hse_3v3(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
  //setup_systick(rcc_ahb_frequency);

  gpio_setup();

  cbuf_init(&host_to_xbee);
  cbuf_init(&xbee_to_host);

  host_init(&host_to_xbee);
  xbee_init();

  bool en = false;

  gpio_set(GPIOA, GPIO8);

  while (1) {
    while (!gpio_get(GPIOC, GPIO12) || cbuf_avail(&host_to_xbee)) {
      if (!en) {
        xbee_start();
        en = true;
        //gpio_toggle(GPIOA, GPIO8);
      }
      uint8_t h = cbuf_pop(&host_to_xbee);
      uint8_t x = xbee_transfer(h);
      cbuf_push(&xbee_to_host, x);

      if (i++ == 1000) {
        host_poll();
      }
    }
    if (en) {
      en = false;
      xbee_stop();
      host_send(&xbee_to_host);
    }

    host_poll();
  }


  return 0;
}
