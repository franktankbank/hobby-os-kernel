#pragma once

#include "limine.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct bootloader_data {
    // Mem Map
    size_t total_mem;
    size_t total_usable_mem;
    u64 memmap_entry_count;
    u64 usable_entry_count;

    LIMINE_PTR(struct limine_memmap_entry **) limine_memory_map;

    u64 hhdm_offset;

    u64 physical_kernel_base;
    u64 virtual_kernel_base;

    // ACPI/MMIO
    u64 *rsdp_table_addr;

    volatile u32 *lapic_virt;
    volatile u32 *ioapic_virt;

    u64 lapic_phys;
    u64 ioapic_phys;

    // framebuffer
    struct limine_framebuffer *framebuffer;

    // SMP
    u64 cpu_count;
    LIMINE_PTR(struct limine_mp_info **) cpus;
    bool smp_enabled;

    int64_t timestamp_at_boot;

    // scheduler
    bool scheduler_enabled;

    void *kernel_file_data;
    size_t kernel_file_size;

    struct limine_memmap_entry **memmap_entries;
} bootloader_data;

struct bootloader_data *get_bootloader_data();
