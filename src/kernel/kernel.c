#include "../cpu/isr/isr.h"
#include "../cpu/idt/idt.h"
#include "../cpu/gdt/gdt.h"
#include "../cpu/pic/pic.h"
#include "kernel.h"
#include "../drivers/keyboard.h"
#include "../drivers/serial.h"
#include "console.h"
#include "paging.h"
#include "limine.h"
#include "pmm.h"
#include "util.h"
#include "macro.h"
#include "acpi/table.h"
#include "acpi/acpi.h"
#include <stdbool.h>

// Set the base revision to 5, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

USED_SECTION(".limine_requests") static volatile u64
    limine_base_revision[] = LIMINE_BASE_REVISION(5);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

USED_SECTION(".limine_requests") static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};

USED_SECTION(".limine_requests") static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST_ID,
    .revision = 0
};

USED_SECTION(".limine_requests") static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0
};

USED_SECTION(".limine_requests") static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

USED_SECTION(".limine_requests_start") static volatile u64
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

USED_SECTION(".limine_requests_end") static volatile u64
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

struct bootloader_data limine_parsed_data;

struct bootloader_data *get_bootloader_data() {
    return &limine_parsed_data;
}

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
    serial_init();
    SerialConsole.printf("\033[2J\033[H");
    SerialConsole.printf("A\n");

    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        SerialConsole.printf("B\n");
        hcf();
    }

    SerialConsole.printf("C\n");

    // Ensure we got a framebuxffer.
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
        SerialConsole.printf("FB NULL\n");
        hcf();
    }

    if (hhdm_request.response == NULL) {
        SerialConsole.printf("HHDM NULL\n");
        hcf();
    }

    if (rsdp_request.response == NULL) {
        SerialConsole.printf("RSDP NULL\n");
        hcf();
    }

    if (memmap_request.response == NULL) {
        SerialConsole.printf("MEMMAP NULL\n");
        hcf();
    }

    SerialConsole.printf("D\n");

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    limine_parsed_data.hhdm_offset = hhdm_request.response->offset;
    limine_parsed_data.rsdp_table_addr = rsdp_request.response->address;
    limine_parsed_data.framebuffer = framebuffer;

    Console.init();

    Console.clear();

    u64 entry_count = memmap_request.response->entry_count;

    limine_parsed_data.memmap_entries = memmap_request.response->entries;
    limine_parsed_data.memmap_entry_count = entry_count;

    limine_parsed_data.ioapic_phys = get_ioapic_phys();
    limine_parsed_data.lapic_phys = get_lapic_phys();

    pmm_init();

    setup_paging();

    gdt_install();

    load_idt();

    isr_install();

    pic_disable();
    SerialConsole.puts("Disabled PIC\n");
    set_irqs();
    SerialConsole.puts("Set IRQs\n");
    init_keyboard();
    SerialConsole.puts("Initialized keyboard\n");
    asm volatile("sti");
    SerialConsole.puts("Enabled Interrupts\n");

    // Console.puts("Hello, World!\nGoodbye, World!");

    // __asm__ volatile("int3");

    // We're done, just hang...
    hcf();
}
