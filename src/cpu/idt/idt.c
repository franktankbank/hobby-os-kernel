#include "idt.h"
#include <stdbool.h>
#include "../gdt/gdt.h"
#include "../../kernel/macro.h"

typedef struct {
    u16 base_low;  // The lower 16 bits of the ISR's address
    u16 kernel_cs; // The GDT segment selector that the CPU will load into
                        // CS before calling the ISR
    u8 ist; // The IST in the TSS that the CPU will load into RSP; set to
                 // zero for now
    u8 attributes; // Type and attributes; see the IDT page
    u16 base_mid;  // The higher 16 bits of the lower 32 bits of the ISR's
                        // address
    u32 base_high; // The higher 32 bits of the ISR's address
    u32 reserved;  // Set to zero
} PACKED idt_entry_t;

typedef struct {
    u16 limit;
    idt_entry_t *base;
} PACKED idtr_t;

ALIGNED(0x10) static idt_entry_t idt_entries[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];

void load_idt() {
    idtr.base = (idt_entry_t *) &idt_entries[0];
    idtr.limit = (u16)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (u16 vector = 0; vector < IDT_MAX_DESCRIPTORS; vector++) {
        set_idt_gate(vector, isr_stub_table[vector], GDT_KERNEL_CODE, 0x8E);
        vectors[vector] = true;
    }
    __asm__ volatile("lidt %0" : : "m"(idtr)); // load the new IDT
    // __asm__ volatile("sti");                   // set the interrupt flag
}

void set_idt_gate(u8 index, void *base, u16 selector, u8 flags) {
    idt_entries[index].base_low   = (u64)base & 0xFFFF;
    idt_entries[index].kernel_cs  = selector;
    idt_entries[index].ist        = 0;
    idt_entries[index].attributes = flags;
    idt_entries[index].base_mid   = ((u64)base >> 16) & 0xFFFF;
    idt_entries[index].base_high  = ((u64)base >> 32) & 0xFFFFFFFF;
    idt_entries[index].reserved   = 0;
}

void idt_gate_enable(int interrupt) {
    FLAG_SET(idt_entries[interrupt].attributes, IDT_FLAG_PRESENT);
}

void idt_gate_disable(int interrupt) {
    FLAG_UNSET(idt_entries[interrupt].attributes, IDT_FLAG_PRESENT);
}
