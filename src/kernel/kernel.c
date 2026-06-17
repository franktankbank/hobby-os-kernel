#include "../cpu/isr/isr.h"
#include "../cpu/idt/idt.h"
#include "../cpu/gdt/gdt.h"
#include "kernel.h"
// #include "../drivers/keyboard.h"
#include "../drivers/serial.h"
#include "console.h"
#include "pmm.h"
#include "limine.h"
#include "util.h"
#include "macro.h"
#include "acpi/table.h"
#include "acpi/acpi.h"
#include <stdbool.h>

// Set the base revision to 5, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

USED_SECTION(".limine_requests") static volatile uint64_t
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

USED_SECTION(".limine_requests_start") static volatile uint64_t
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

USED_SECTION(".limine_requests_end") static volatile uint64_t
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

    gdt_install();

    load_idt();

    // serial_init();

    isr_install();

  // lapic_init_x2apic();

  // ioapic_init(hhdm_offset);

  // ioapic_set_redirect(mp_request.response->bsp_lapic_id, 1,
  //                     33); // keyboard → IRQ1 → vector 33

  // asm volatile("sti");

  // init_keyboard();
//   acpi_init();

    // limine_parsed_data.lapic_addr = make_hhdm(get_lapic_addr());

    uint64_t entry_count = memmap_request.response->entry_count;

    SerialConsole.printf("Memmap Entries: %lu\n", entry_count);

    for (uint64_t i = 0; i < entry_count; i++) {
        struct limine_memmap_entry *entry = memmap_request.response->entries[i];
        char* type = "";
        switch (entry->type) {
            case 0:
                type = "LIMINE_MEMMAP_USABLE";
                break;
            case 1:
                type = "LIMINE_MEMMAP_RESERVED";
                break;
            case 2:
                type = "LIMINE_MEMMAP_ACPI_RECLAIMABLE";
                break;
            case 3:
                type = "LIMINE_MEMMAP_ACPI_NVS";
                break;
            case 4:
                type = "LIMINE_MEMMAP_BAD_MEMORY";
                break;
            case 5:
                type = "LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE";
                break;
            case 6:
                type = "LIMINE_MEMMAP_EXECUTABLE_AND_MODULES";
                break;
            case 7:
                type = "LIMINE_MEMMAP_FRAMEBUFFER";
                break;
            case 8:
                type = "LIMINE_MEMMAP_RESERVED_MAPPED";
                break;
        }
        SerialConsole.printf("Entry %lu:\nBase: %lu\nLength: %lu\nType: %s\n\n", i, entry->base, entry->length, type);
    }

    // SerialConsole.printf("id = %#x", limine_parsed_data.lapic_addr[0x20 / 4]);

    uint64_t phys = get_lapic_addr();
    // void *virt = make_hhdm(phys);

    // SerialConsole.printf("lapic phys=%#lx\n", phys);
    // SerialConsole.printf("lapic virt=%p\n", virt);

    void *lapic = mmio_map(phys, 0x1000);

    // hcf();

    Console.puts("Hello, World!\nGoodbye, World!");

    __asm__ volatile("int3");

    // We're done, just hang...
    hcf();
}
