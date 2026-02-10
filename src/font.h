#ifndef FONT_H_
#define FONT_H_

#include <limine.h>
#include <stddef.h>
#include <stdint.h>

struct _char_parameters {
  int fg;
  int bg;
  int x;
  int y;
};

typedef struct _char_parameters char_parameters;

void putchar(struct limine_framebuffer *fb, char_parameters params,
             uint32_t unicode);

void puttext(struct limine_framebuffer *fb, char_parameters params,
             const char *s);

#endif
