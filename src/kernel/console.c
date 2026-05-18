#define SSFN_CONSOLEBITMAP_TRUECOLOR /* use the special renderer for 32 bit    \
                                        truecolor packed pixels */
#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_ALT_FORM_FLAG 1
#include <stdarg.h>
#include <stdbool.h>
#include "console.h"
#include "ssfn.h"
#include "va_list.h"
#include "nanoprintf.h"
#include "font.h"

static struct {
  int row;
  int col;
} cursor;

ssfn_buf_t ssfn_dst;
ssfn_font_t *ssfn_src;

static int term_cols;
static int term_rows;

static int cell_w; // Depends on font size, so it will be set later
static int cell_h; // Depends on font size, so it will be set later

void console_init() {

  ssfn_src = Fonts.font24.normal;
  ssfn_dst.ptr = limine_parsed_data.framebuffer->address;
  ssfn_dst.w = limine_parsed_data.framebuffer->width;
  ssfn_dst.h = limine_parsed_data.framebuffer->height;
  ssfn_dst.p = limine_parsed_data.framebuffer->pitch;
  ssfn_dst.fg = 0xFFFFFF;
  ssfn_dst.bg = 0;
  ssfn_dst.x = 0;
  ssfn_dst.y = 0;

  cell_w = Fonts.font24.width; // Default
  cell_h = Fonts.font24.size; // Default

  term_cols = limine_parsed_data.framebuffer->width / cell_w;
  term_rows = limine_parsed_data.framebuffer->height / cell_h;
}

void term_putc(uint32_t ch) {
  if (ch == '\n') {
    cursor.col = 0;
    cursor.row++;
    return;
  }

  ssfn_dst.x = cursor.col * cell_w;
  ssfn_dst.y = cursor.row * cell_h;

  ssfn_putc(ch);

  cursor.col++;

  if (cursor.col >= term_cols) {
    cursor.col = 0;
    cursor.row++;
  }

  if (cursor.row >= term_rows) {
    cursor.row = term_rows - 1;
  }
}

void term_clear(void) {
  for (uint32_t y = 0; y < ssfn_dst.h; y++) {
    uint32_t *row = (uint32_t *)((uint8_t *)ssfn_dst.ptr + y * ssfn_dst.p);

    for (uint32_t x = 0; x < ssfn_dst.w; x++) {
      row[x] = ssfn_dst.bg;
    }
  }

  cursor.row = 0;
  cursor.col = 0;
}

void term_puts(const char *s) {
  while (*s) {
    term_putc((unsigned char)*s);
    s++;
  }
}

void term_puthex(uint64_t value) {
  char hex[] = "0123456789ABCDEF";
  char buffer[17];
  buffer[16] = '\0';

  for (int i = 15; i >= 0; i--) {
    buffer[i] = hex[value & 0xF];
    value >>= 4;
  }

  term_puts("0x");
  term_puts(buffer);
}

void term_centered_puts(const char *s) {
  // Calculate string length
  int len = 0;
  const char *p = s;
  while (*p++) len++;

  // Compute center col
  int start_col = (term_cols - len) / 2;

  if (start_col < 0) start_col = 0;

  cursor.col = start_col;
  term_puts(s);
}

void cursor_set_cursor(int col, int row) {
  cursor.col = col;
  cursor.row = row;
}

int cursor_get_current_col() {
  return cursor.col;
}

int cursor_get_current_row() {
  return cursor.row;
}

int cursor_get_cols() {
  return term_cols;
}

int cursor_get_rows() {
  return term_rows;
}

int term_printf(const char *fmt, ...) {
  char buffer[1024];
  va_list args;

  va_start(args, fmt);
  int length = npf_vsnprintf(buffer, sizeof(buffer), fmt, args);

  if (length < 0 || length >= (int)sizeof(buffer)) {
    return -1;
  }

  term_puts(buffer);

  return length;
}

int term_centered_printf(const char *fmt, ...) {
  char buffer[1024];
  va_list args;

  va_start(args, fmt);
  int length = npf_vsnprintf(buffer, sizeof(buffer), fmt, args);

  if (length < 0 || length >= (int)sizeof(buffer)) {
    return -1;
  }

  term_centered_puts(buffer);

  return length;
}

void config_set_font(uint8_t fontsize, bool bold) {
  term_clear();
  if (bold) {
    switch (fontsize) {
      case 12:
        ssfn_src = Fonts.font12.bold;
        cell_h = Fonts.font12.size;
        cell_w = Fonts.font12.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 14:
        ssfn_src = Fonts.font14.bold;
        cell_h = Fonts.font14.size;
        cell_w = Fonts.font14.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 16:
        ssfn_src = Fonts.font16.bold;
        cell_h = Fonts.font16.size;
        cell_w = Fonts.font16.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 18:
        ssfn_src = Fonts.font18.bold;
        cell_h = Fonts.font18.size;
        cell_w = Fonts.font18.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 20:
        ssfn_src = Fonts.font20.bold;
        cell_h = Fonts.font20.size;
        cell_w = Fonts.font20.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 22:
        ssfn_src = Fonts.font22.bold;
        cell_h = Fonts.font22.size;
        cell_w = Fonts.font22.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 24:
        ssfn_src = Fonts.font24.bold;
        cell_h = Fonts.font24.size;
        cell_w = Fonts.font24.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 28:
        ssfn_src = Fonts.font28.bold;
        cell_h = Fonts.font28.size;
        cell_w = Fonts.font28.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 32:
        ssfn_src = Fonts.font32.bold;
        cell_h = Fonts.font32.size;
        cell_w = Fonts.font32.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      default:
        term_printf("Invalid font size %d. Defaulting to size 24\n");
        ssfn_src = Fonts.font24.bold;
        cell_h = Fonts.font24.size;
        cell_w = Fonts.font24.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
    }
  } else {
    switch (fontsize) {
      case 12:
        ssfn_src = Fonts.font12.normal;
        cell_h = Fonts.font12.size;
        cell_w = Fonts.font12.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 14:
        ssfn_src = Fonts.font14.normal;
        cell_h = Fonts.font14.size;
        cell_w = Fonts.font14.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 16:
        ssfn_src = Fonts.font16.normal;
        cell_h = Fonts.font16.size;
        cell_w = Fonts.font16.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 18:
        ssfn_src = Fonts.font18.normal;
        cell_h = Fonts.font18.size;
        cell_w = Fonts.font18.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 20:
        ssfn_src = Fonts.font20.normal;
        cell_h = Fonts.font20.size;
        cell_w = Fonts.font20.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 22:
        ssfn_src = Fonts.font22.normal;
        cell_h = Fonts.font22.size;
        cell_w = Fonts.font22.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 24:
        ssfn_src = Fonts.font24.normal;
        cell_h = Fonts.font24.size;
        cell_w = Fonts.font24.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 28:
        ssfn_src = Fonts.font28.normal;
        cell_h = Fonts.font28.size;
        cell_w = Fonts.font28.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      case 32:
        ssfn_src = Fonts.font32.normal;
        cell_h = Fonts.font32.size;
        cell_w = Fonts.font32.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
      default:
        term_printf("Invalid font size %d. Defaulting to size 24\n");
        ssfn_src = Fonts.font24.normal;
        cell_h = Fonts.font24.size;
        cell_w = Fonts.font24.width;
        term_rows = ssfn_dst.h / cell_h;
        term_cols = ssfn_dst.w / cell_w;
        break;
    }
  }

}

void config_set_bg(uint32_t bg) {
  ssfn_dst.bg = bg;
}

void config_set_fg(uint32_t fg) {
  ssfn_dst.fg = fg;
}

const struct _console Console = {
  .clear = term_clear,
  .puthex = term_puthex,
  .printf = term_printf,
  .init = console_init,
  .putc = term_putc,
  .puts = term_puts,
  .centered_puts = term_centered_puts,
  .config.set_font = config_set_font,
  .config.set_bg = config_set_bg,
  .config.set_fg = config_set_fg,
  .cursor.set_cursor = cursor_set_cursor,
  .cursor.get_current_col = cursor_get_current_col,
  .cursor.get_current_row = cursor_get_current_row,
  .cursor.get_cols = cursor_get_cols,
  .cursor.get_rows = cursor_get_rows,
  .centered_printf = term_centered_printf
};
