#define SSFN_CONSOLEBITMAP_TRUECOLOR /* use the special renderer for 32 bit    \
                                        truecolor packed pixels */

#include <limine.h>
#include <ssfn.h>
#include <string.h>

extern void _binary_fonts_ter_v24n_sfn_start;

struct _char_parameters {
  int fg;
  int bg;
  int x;
  int y;
};

typedef struct _char_parameters char_parameters;

void putchar(struct limine_framebuffer *fb, char_parameters *params,
             uint32_t unicode) {
  ssfn_src = &_binary_fonts_ter_v24n_sfn_start;
  ssfn_dst.ptr = fb->address;
  ssfn_dst.p = fb->pitch;
  ssfn_dst.fg = params->fg;
  ssfn_dst.bg = params->bg;
  ssfn_dst.x = params->x;
  ssfn_dst.y = params->y;
  ssfn_dst.w = fb->width;
  ssfn_dst.h = fb->height;

  ssfn_putc(unicode);

  // write cursor back
  params->x = ssfn_dst.x;
  params->y = ssfn_dst.y;
}

void puttext(struct limine_framebuffer *fb, char_parameters params,
             const char *s) {
  while (*s) {
    putchar(fb, &params, *s++);
  }
}
