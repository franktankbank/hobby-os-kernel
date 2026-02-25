#pragma once

#include <stdint.h>

/* Segment selectors */
#define KERNEL_CS 0x08

/* How every interrupt gate (handler) is defined */
typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;        // bits 0..2 = IST, rest must be zero
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed)) idt_gate_t;

extern idt_gate_t idt[256];

/* A pointer to the array of interrupt handlers.
     * Assembly instruction 'lidt' will read it */
typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_register_t;

void set_idt_gate(int n, uintptr_t handler);

void load_idt();
