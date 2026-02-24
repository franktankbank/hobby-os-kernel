#pragma once

#include "limine.h"
#include <stdint.h>

void term_putc(uint32_t ch);

void term_puts(const char *s);

void console_init(struct limine_framebuffer *fb);

void term_clear(void);

// struct _console {
//     void (*putc)(uint32_t);
//     void (*puts)(const char*);
//     void (*init)(struct limine_framebuffer*);
//     void (*clear)(void);
// };

// extern const struct _console Console;
