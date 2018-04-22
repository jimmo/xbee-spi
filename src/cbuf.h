#pragma once

#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE (32*1024)

typedef struct cbuf_t {
  uint8_t data[BUFFER_SIZE];
  size_t start;
  size_t end;
} cbuf_t;

void cbuf_init(cbuf_t* b);
size_t cbuf_avail(cbuf_t* b);
void cbuf_push(cbuf_t* b, uint8_t data);
uint8_t cbuf_pop(cbuf_t* b);
