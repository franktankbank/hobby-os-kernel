#include "gdt.h"

extern void _load_gdt(gdt_pointer_t *descriptor);
extern void _reload_segments(u64 cs, u64 ds);

gdt_pointer_t gdtr;
struct {
    gdt_entry_t gdt_entries[5];
    tss_entry_t tss_entry;
} PACKED gdt;
tss_t tss = {0};

#define KERNEL_STACK_SIZE 4096 * 8
char kernel_stack[KERNEL_STACK_SIZE];

void gdt_install() {
    gdt.gdt_entries[0] = (gdt_entry_t)GDT_ENTRY(0, 0, 0, 0); // null seg
    gdt.gdt_entries[1] = (gdt_entry_t)GDT_ENTRY(0, 0xFFFFF, 0x9A, 0xA); // kernel code seg
    gdt.gdt_entries[2] = (gdt_entry_t)GDT_ENTRY(0, 0xFFFFF, 0x92, 0xC); // kernel data seg
    gdt.gdt_entries[3] = (gdt_entry_t)GDT_ENTRY(0, 0xFFFFF, 0xFA, 0xA); // user code seg
    gdt.gdt_entries[4] = (gdt_entry_t)GDT_ENTRY(0, 0xFFFFF, 0xF2, 0xC); // user data seg

    gdtr.size    = (u16)(sizeof(gdt) - 1);
    gdtr.pointer = (gdt_entry_t *)&gdt;

    tss.rsp0 = (u64)(kernel_stack + KERNEL_STACK_SIZE);

    gdt.tss_entry.limit_low   = sizeof(tss_t) - 1;
    gdt.tss_entry.base_low    = (u16)((u64)&tss & 0xffff);
    gdt.tss_entry.base_middle = (u8)(((u64)&tss >> 16) & 0xff);
    gdt.tss_entry.access      = 0x89;
    gdt.tss_entry.limit_high_and_flags = 0;
    gdt.tss_entry.base_high   = (u8)(((u64)&tss >> 24) & 0xff);
    gdt.tss_entry.base_higher = (u32)((u64)&tss >> 32);
    gdt.tss_entry.zero        = 0;

    _load_gdt(&gdtr);
    _reload_segments(GDT_KERNEL_CODE, GDT_KERNEL_DATA);
}
