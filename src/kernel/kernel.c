#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/gdt.h"
// #include "../drivers/keyboard.h"
#include "console.h"
#include "limine.h"
#include "util.h"
#include <stdbool.h>

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

// __attribute__((
//     used, section(".limine_requests"))) static volatile struct limine_mp_request
//     mp_request = {.id = LIMINE_MP_REQUEST_ID, .revision = 0};

// __attribute__((
//     used,
//     section(".limine_requests"))) static volatile struct limine_hhdm_request
//     hhdm_request = {.id = LIMINE_HHDM_REQUEST_ID, .revision = 0};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used,
               section(".limine_requests_start"))) static volatile uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// uint64_t hhdm_offset;

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
    hcf();
  }

  // Ensure we got APIC
  // if (mp_request.response == NULL || mp_request.response->cpu_count < 1) {
  //   hcf();
  // }

  // Ensure we got a framebuxffer.
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    hcf();
  }

  // Ensure we got HHDM
  // if (hhdm_request.response == NULL) {
  //   hcf();
  // }

  // hhdm_offset = hhdm_request.response->offset;

  // Fetch the first framebuffer.
  struct limine_framebuffer *framebuffer =
      framebuffer_request.response->framebuffers[0];

  Console.init(framebuffer);

  Console.clear();

  gdt_install();

  load_idt();

  isr_install();

  // lapic_init_x2apic();

  // ioapic_init(hhdm_offset);

  // ioapic_set_redirect(mp_request.response->bsp_lapic_id, 1,
  //                     33); // keyboard → IRQ1 → vector 33

  // asm volatile("sti");

  // init_keyboard();

  Console.puts("Hello, World!\nGoodbye, World!");

  __asm__ volatile("int3");

  // We're done, just hang...
  hcf();
}
