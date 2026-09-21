#pragma once

#include "../types/int.h"
#include <stdbool.h>

struct _console_config {
    void (*set_font)(u8, bool);
    void (*set_fg)(u32);
    void (*set_bg)(u32);
};

struct _console_cursor {
    void (*set_cursor)(int, int);
    int (*get_current_col)();
    int (*get_current_row)();
    int (*get_cols)();
    int (*get_rows)();
};

struct _console {
    // void term_putc(u32 ch)
    void (*putc)(u32);
    // void term_puts(const char *s)
    void (*puts)(const char*);
    // void console_init(struct limine_framebuffer *fb)
    void (*init)();
    // void term_puthex(u64 value)
    void (*puthex)(u64);
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

struct _serial_console {
    // void serial_putc(char ch)
    void (*putc)(char);
    // void serial_puts(const char *s)
    void (*puts)(const char*);
    // void serial_puthex(u64 value)
    void (*puthex)(u64);
    // int serial_printf(const char *fmt, ...)
    int (*printf)(const char*, ...);
};

extern const struct _serial_console SerialConsole;

extern const struct _console Console;
