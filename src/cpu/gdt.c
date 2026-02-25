#include "gdt.h"

extern void reloadSegments();

void encodeGdtEntry(uint8_t *target, struct gdt source) {
    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;

    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // Encode the access byte
    target[5] = source.access_byte;

    // Encode the flags
    target[6] |= (source.flags << 4);
}

void gdt_install() {
    uint8_t gdt[8*5]; // or more, depending on how many entries

    struct gdt gdt_null = {
        .base = 0,
        .limit = 0x00000000,
        .access_byte = 0x00,
        .flags = 0x0
    };
    struct gdt gdt_c0 = {
        .base = 0,
        .limit = 0xFFFFF,
        .access_byte = 0x9A,
        .flags = 0xA
    };
    struct gdt gdt_d0 = {
        .base = 0,
        .limit = 0xFFFFF,
        .access_byte = 0x92,
        .flags = 0xC
    };
    struct gdt gdt_d3 = {
        .base = 0,
        .limit = 0xFFFFF,
        .access_byte = 0xF2,
        .flags = 0xC
    };
    struct gdt gdt_c3 = {
        .base = 0,
        .limit = 0xFFFFF,
        .access_byte = 0xFA,
        .flags = 0xA
    };
    __asm__ volatile("cli");
    encodeGdtEntry(&gdt[0*8], gdt_null);
    encodeGdtEntry(&gdt[1*8], gdt_c0);
    encodeGdtEntry(&gdt[2*8], gdt_d0);
    encodeGdtEntry(&gdt[3*8], gdt_d3);
    encodeGdtEntry(&gdt[4*8], gdt_c3);

    struct gdt_ptr {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) gdtr;

    gdtr.base = (uint64_t) &gdt;
    gdtr.limit = sizeof(gdt) - 1;

    asm volatile("lgdt %0" : : "m" (gdtr));
    reloadSegments();
}
