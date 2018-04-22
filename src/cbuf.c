#include "cbuf.h"

#include <libopencm3/stm32/gpio.h>

void cbuf_init(cbuf_t* b) {
  b->start = 0;
  b->end = 0;
}

size_t cbuf_avail(cbuf_t* b) {
  if (b->end >= b->start) {
    return b->end - b->start;
  } else {
    return b->end + BUFFER_SIZE - b->start;
  }
}

void cbuf_push(cbuf_t* b, uint8_t data) {
  b->data[b->end] = data;
  b->end = (b->end + 1) % BUFFER_SIZE;
  if (b->end == b->start) {
    gpio_clear(GPIOA, GPIO8);
  }
}

uint8_t cbuf_pop(cbuf_t* b) {
  if (b->start == b->end) {
    return 0x00;
  }
  uint8_t data = b->data[b->start];
  b->start = (b->start + 1) % BUFFER_SIZE;
  return data;
}
