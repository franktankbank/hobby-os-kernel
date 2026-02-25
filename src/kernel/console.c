#define SSFN_CONSOLEBITMAP_TRUECOLOR /* use the special renderer for 32 bit    \
                                        truecolor packed pixels */

#include "console.h"
#include "ssfn.h"

extern ssfn_font_t _binary_fonts_ter_v24n_sfn_start;

static struct {
  int row;
  int col;
} cursor;

ssfn_buf_t ssfn_dst;
ssfn_font_t *ssfn_src;

static int term_cols;
static int term_rows;

static int cell_w = 12;
static int cell_h; // Depends on font size, so it will be set later

// void init(struct limine_framebuffer *fb) {
void console_init(struct limine_framebuffer *fb) {
  ssfn_src = &_binary_fonts_ter_v24n_sfn_start;
  ssfn_dst.ptr = fb->address;
  ssfn_dst.w = fb->width;
  ssfn_dst.h = fb->height;
  ssfn_dst.p = fb->pitch;
  ssfn_dst.fg = 0xFFFFFF;
  ssfn_dst.bg = 0;
  ssfn_dst.x = 0;
  ssfn_dst.y = 0;

  cell_h = 24; // Default

  term_cols = fb->width / cell_w;
  term_rows = fb->height / cell_h;
}

// void putc(uint32_t ch) {
void term_putc(uint32_t ch) {
  if (ch == '\n') {
    cursor.col = 0;
    cursor.row++;
    return;
  }

  ssfn_dst.x = cursor.col * cell_w;
  // ssfn_dst.y = (cursor.row + 1) * cell_h;
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

// void clear(void) {
void term_clear(void) {
  // uint32_t *fb = (uint32_t *)ssfn_dst.ptr;

  for (uint32_t y = 0; y < ssfn_dst.h; y++) {
    uint32_t *row = (uint32_t *)((uint8_t *)ssfn_dst.ptr + y * ssfn_dst.p);

    for (uint32_t x = 0; x < ssfn_dst.w; x++) {
      row[x] = ssfn_dst.bg;
    }
  }

  cursor.row = 0;
  cursor.col = 0;
}

// void puts(const char *s) {
void term_puts(const char *s) {
  while (*s) {
    // putc((unsigned char)*s);
    term_putc((unsigned char)*s);
    s++;
  }
}

// const struct _console Console = {
//   .clear = clear,
//   .init = init,
//   .putc = putc,
//   .puts = puts
// };
