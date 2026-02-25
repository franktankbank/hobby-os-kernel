#pragma once

#include <stdint.h>

#define GDT_NULL 0x00;
#define GDT_KERNEL_CODE 0x08;
#define GDT_KERNEL_DATA 0x10;
#define GDT_USER_DATA 0x18;
#define GDT_USER_CODE 0x20;
#define GDT_TSS 0x28;

struct __attribute__((packed)) gdt {
    uint64_t base;
    uint32_t limit;
    uint8_t access_byte;
    uint8_t flags;
};

void gdt_install();
