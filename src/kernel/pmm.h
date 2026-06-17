#pragma once

#include <stdint.h>

#include "globals.h"

// #define MAKE_HHDM(x) (void *)((uintptr_t)x + limine_parsed_data.hhdm_offset)
// #define FROM_HHDM(x) (void *)((uintptr_t)x - limine_parsed_data.hhdm_offset)

// Convert physical memory address to a virtual one
static inline void *make_hhdm(uintptr_t phys) {
    return (void *)(phys + (uintptr_t)limine_parsed_data.hhdm_offset);
}

// Convert virtual memory address to a physical one
static inline uintptr_t from_hhdm(const void *virt) {
    return (uintptr_t)virt - (uintptr_t)limine_parsed_data.hhdm_offset;
}

static inline void *mmio_map(uint64_t phys, size_t size) {
    (void)phys;
    (void)size;
    __asm__ volatile("int3");
}
