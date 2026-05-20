#pragma once

#include <stdint.h>

// unsigned char port_byte_in(uint16_t port);

// void port_byte_out(uint16_t port, uint8_t data);

// unsigned short port_word_in(uint16_t port);

// void port_word_out(uint16_t port, uint16_t data);

void outb(uint16_t port, uint8_t val);

uint8_t inb(uint16_t port);
