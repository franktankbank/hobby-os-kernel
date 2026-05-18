#pragma once

#include "limine.h"
#include "kernel.h"
#include <stdint.h>
#include <stdbool.h>

extern struct bootloader_data limine_parsed_data;

struct _console_config {
    void (*set_font)(uint8_t, bool);
    void (*set_fg)(uint32_t);
    void (*set_bg)(uint32_t);
};

struct _console_cursor {
    void (*set_cursor)(int, int);
    int (*get_current_col)();
    int (*get_current_row)();
    int (*get_cols)();
    int (*get_rows)();
};

struct _console {
    // void term_putc(uint32_t ch)
    void (*putc)(uint32_t);
    // void term_putc(const char *s)
    void (*puts)(const char*);
    // void console_init(struct limine_framebuffer *fb)
    void (*init)();
    // void term_puthex(uint64_t value)
    void (*puthex)(uint64_t);
    // int term_printf(const char *fmt, ...)
    int (*printf)(const char*, ...);
    // void term_clear(void)
    void (*clear)(void);
    // void term_centered_puts(const char *s)
    void (*centered_puts)(const char*);
    // int term_centered_printf(const char *fmt, ...)
    int (*centered_printf)(const char*, ...);
    struct _console_config config;
    struct _console_cursor cursor;
};

extern const struct _console Console;
