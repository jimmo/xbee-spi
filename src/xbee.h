#pragma once

#include <stdint.h>

void xbee_init(void);
void xbee_poll(void);

void xbee_start(void);
void xbee_stop(void);
uint8_t xbee_transfer(uint8_t);
