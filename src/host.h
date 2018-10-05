#pragma once

#include "cbuf.h"

#include <stdint.h>
#include <stdbool.h>

void host_init(cbuf_t* buf);
void host_poll(void);
bool host_connected(void);

void host_debug(uint8_t* buf);
void host_send(cbuf_t* buf);
