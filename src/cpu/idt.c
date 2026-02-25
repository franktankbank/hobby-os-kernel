#include "idt.h"

idt_gate_t idt[256];
idt_register_t idt_reg;

void set_idt_gate(int n, uintptr_t handler) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = KERNEL_CS;
    idt[n].ist = 0; // very important for now
    idt[n].type_attr = 0x8E; // present, ring 0, interrupt gate
    idt[n].offset_mid = (handler >> 16) & 0xFFFF;
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].zero = 0;
}

void load_idt() {
    idt_reg.base = (uint64_t) &idt;
    idt_reg.limit = sizeof(idt) - 1;
    __asm__ volatile("lidt %0" : : "m"(idt_reg));
}
