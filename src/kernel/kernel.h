#pragma once

#include "limine.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct bootloader_data {
    // Mem Map
    size_t total_mem;
    size_t total_usable_mem;
    uint64_t memmap_entry_count;
    uint64_t usable_entry_count;

    LIMINE_PTR(struct limine_memmap_entry **) limine_memory_map;

    uint64_t hhdm_offset;

    uint64_t physical_kernel_base;
    uint64_t virtual_kernel_base;

    // ACPI/MMIO
    uint64_t *rsdp_table_addr;

    uint64_t p_lapic_base;
    uint32_t p_ioapic_base;

    // framebuffer
    struct limine_framebuffer *framebuffer;

    // SMP
    uint64_t cpu_count;
    LIMINE_PTR(struct limine_mp_info **) cpus;
    bool smp_enabled;

    uint64_t boot_time; // ms since boot

    // scheduler
    bool scheduler_enabled;

    void *kernel_file_data;
    size_t kernel_file_size;
} bootloader_data;

struct bootloader_data *get_bootloader_data();
