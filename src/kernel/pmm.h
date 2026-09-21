#pragma once

#include "../types/int.h"

#include "globals.h"

#define PAGE_SIZE 4096

// Convert physical memory address to a virtual one
static inline void *make_hhdm(uintptr_t phys) {
    return (void *)(phys + (uintptr_t)limine_parsed_data.hhdm_offset);
}

// Convert virtual memory address to a physical one
static inline uintptr_t from_hhdm(const void *virt) {
    return (uintptr_t)virt - (uintptr_t)limine_parsed_data.hhdm_offset;
}

static inline void *mmio_map(u64 phys, size_t size) {
    (void)phys;
    (void)size;
    __asm__ volatile("int3");
}

u64 alloc_4k_page(void);

void pmm_init();
