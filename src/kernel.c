<<<<<<< Updated upstream:src/kernel.c
#include "font.h"
#include <limine.h>
=======
// #include "../cpu/isr.h"
// #include "../drivers/keyboard.h"
#include "console.h"
#include "limine.h"
#include "util.h"
>>>>>>> Stashed changes:src/kernel/kernel.c
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Set the base revision to 4, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests"))) static volatile uint64_t
    limine_base_revision[] = LIMINE_BASE_REVISION(4);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};

<<<<<<< Updated upstream:src/kernel.c
=======
// __attribute__((
//     used, section(".limine_requests"))) static volatile struct limine_mp_request
//     mp_request = {.id = LIMINE_MP_REQUEST_ID, .revision = 0};

// __attribute__((
//     used,
//     section(".limine_requests"))) static volatile struct limine_hhdm_request
//     hhdm_request = {.id = LIMINE_HHDM_REQUEST_ID, .revision = 0};

>>>>>>> Stashed changes:src/kernel/kernel.c
// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used,
               section(".limine_requests_start"))) static volatile uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
  uint8_t *restrict pdest = (uint8_t *restrict)dest;
  const uint8_t *restrict psrc = (const uint8_t *restrict)src;

  for (size_t i = 0; i < n; i++) {
    pdest[i] = psrc[i];
  }

  return dest;
}

void *memset(void *s, int c, size_t n) {
  uint8_t *p = (uint8_t *)s;

  for (size_t i = 0; i < n; i++) {
    p[i] = (uint8_t)c;
  }

  return s;
}

void *memmove(void *dest, const void *src, size_t n) {
  uint8_t *pdest = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;

  if (src > dest) {
    for (size_t i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else if (src < dest) {
    for (size_t i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const uint8_t *p1 = (const uint8_t *)s1;
  const uint8_t *p2 = (const uint8_t *)s2;

  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}

// Halt and catch fire function.
static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}

<<<<<<< Updated upstream:src/kernel.c
=======
// uint64_t hhdm_offset;

>>>>>>> Stashed changes:src/kernel/kernel.c
// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
    hcf();
  }

<<<<<<< Updated upstream:src/kernel.c
  // Ensure we got a framebuffer.
=======
  // Ensure we got APIC
  // if (mp_request.response == NULL || mp_request.response->cpu_count < 1) {
  //   hcf();
  // }

  // Ensure we got a framebuxffer.
>>>>>>> Stashed changes:src/kernel/kernel.c
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    hcf();
  }

<<<<<<< Updated upstream:src/kernel.c
=======
  // Ensure we got HHDM
  // if (hhdm_request.response == NULL) {
  //   hcf();
  // }

  // hhdm_offset = hhdm_request.response->offset;

>>>>>>> Stashed changes:src/kernel/kernel.c
  // Fetch the first framebuffer.
  struct limine_framebuffer *framebuffer =
      framebuffer_request.response->framebuffers[0];

<<<<<<< Updated upstream:src/kernel.c
  char_parameters params;
  params.fg = 0xFFFFFF;
  params.bg = 0;
  params.x = 100;
  params.y = 200;
  // putchar(framebuffer, params, 'H');
  puttext(framebuffer, params, "Hello, World!\nGoodbye, World!");
=======
  console_init(framebuffer);
  // Console.init(framebuffer);

  // term_clear();
  // Console.clear();

  // isr_install();

  // lapic_init_x2apic();

  // ioapic_init(hhdm_offset);

  // ioapic_set_redirect(mp_request.response->bsp_lapic_id, 1,
  //                     33); // keyboard → IRQ1 → vector 33

  // asm volatile("sti");

  // init_keyboard();

  term_puts("Hello, World!\nGoodbye, World!");
  // Console.puts("Hello, World!\nGoodbye, World!");
>>>>>>> Stashed changes:src/kernel/kernel.c

  // We're done, just hang...
  hcf();
}
