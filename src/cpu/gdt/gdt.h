#pragma once

#include "../../types/int.h"
#include "../../kernel/macro.h"

#define GDT_NULL 0x00
#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_DATA 0x18
#define GDT_USER_CODE 0x20

// Helper macros
#define GDT_LIMIT_LOW(limit)  (limit & 0xFFFF)
#define GDT_BASE_LOW(base)    (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base) ((base >> 16) & 0xFF)
#define GDT_FLAGS_HI_LIMIT(limit, flags)                                       \
    (((limit >> 16) & 0xF) | ((flags << 4) & 0xF0))
#define GDT_BASE_HIGH(base) ((base >> 24) & 0xFF)

#define GDT_ENTRY(base, limit, access, flags)                                  \
    {GDT_LIMIT_LOW(limit),                                                     \
     GDT_BASE_LOW(base),                                                       \
     GDT_BASE_MIDDLE(base),                                                    \
     access,                                                                   \
     GDT_FLAGS_HI_LIMIT(limit, flags),                                         \
     GDT_BASE_HIGH(base)}

void gdt_install();

typedef struct {
    u32 reserved0;
    u64 rsp0;
    u64 rsp1;
    u64 rsp2;
    u64 reserved1;
    u64 ist[7];
    u64 reserved2;
    u32 iobase;
} PACKED tss_t;

typedef struct {
    u16 limit_low;           // limit & 0xFF
    u16 base_low;            // base & 0xFF
    u8 base_middle;          // (base >> 16) & 0xFF
    u8 access;               // access
    u8 limit_high_and_flags; // ((limit >> 16) & 0xF) | (flags & 0xF0)
    u8 base_high;            // (base >> 24) & 0xF
} PACKED gdt_entry_t;

typedef struct {
    u16 limit_low;
    u16 base_low;
    u8 base_middle;
    u8 access;
    u8 limit_high_and_flags;
    u8 base_high;
    u32 base_higher;
    u32 zero;
} PACKED tss_entry_t;

typedef struct {
    u16 size;
    gdt_entry_t *pointer;
} PACKED gdt_pointer_t;
