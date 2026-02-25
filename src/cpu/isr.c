#include "isr.h"
#include "../drivers/ports.h"
#include "../kernel/console.h"
#include "idt.h"
#include "../kernel/util.h"
#include <stdbool.h>

// --- x2APIC MSRs ---
// #define X2APIC_ID_MSR 0x802
// #define X2APIC_EOI_MSR 0x80B
// #define X2APIC_SVR_MSR 0x80F
// #define X2APIC_TPR_MSR 0x808
// #define X2APIC_ICR_LOW 0x830
// #define X2APIC_ICR_HIGH 0x831

// extern uint64_t hhdm_offset;

// #define IOAPIC_PHYS 0xFEC00000ULL
// #define PAGE_PRESENT 0x1
// #define PAGE_RW 0x2
// #define PAGE_PWT 0x8
// #define PAGE_PCD 0x10

// volatile uint32_t *ioapic;

// static uint64_t ioapic_page_table[512] __attribute__((aligned(4096)));

// void map_ioapic_static(uint64_t virt, uint64_t phys) {
//   // assume PML4/PT already exist
//   ioapic_page_table[virt >> 12 & 0x1FF] =
//       phys | PAGE_PRESENT | PAGE_RW | PAGE_PCD | PAGE_PWT;
//   asm volatile("invlpg (%0)" ::"r"(virt) : "memory");
// }

// void ioapic_init(uint64_t hhdm_offset) {
//   ioapic = (volatile uint32_t *)(IOAPIC_PHYS + hhdm_offset);
//   map_ioapic_static(IOAPIC_PHYS, *ioapic);
// }

// static inline uint32_t ioapic_read(uint8_t reg) {
//   ioapic[0] = reg;
//   return ioapic[4];
// }

// static inline void ioapic_write(uint8_t reg, uint32_t val) {
//   ioapic[0] = reg;
//   ioapic[4] = val;
// }

// void ioapic_set_redirect(uint32_t bsp_lapic_id, int irq, int vector) {
//   uint32_t lo = vector;       // delivery = fixed, unmasked
//   uint32_t hi = bsp_lapic_id; // destination = BSP (lapic id 0)

//   ioapic_write(0x10 + irq * 2, lo);
//   ioapic_write(0x11 + irq * 2, hi);
// }

// isr_t interrupt_handlers[256];

/* To print the message which defines every exception */
char *exception_messages[] = {"Division By Zero",
                              "Debug",
                              "Non Maskable Interrupt",
                              "Breakpoint",
                              "Into Detected Overflow",
                              "Out of Bounds",
                              "Invalid Opcode",
                              "No Coprocessor",
                              "Double Fault",
                              "Coprocessor Segment Overrun",
                              "Bad TSS",
                              "Segment Not Present",
                              "Stack Fault",
                              "General Protection Fault",
                              "Page Fault",
                              "Intel Reserved",
                              "Coprocessor Fault",
                              "Alignment Check",
                              "Machine Check",
                              "SIMD Floating-Point Exception",
                              "Virtualization Exception",
                              "Control Protection Exception",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Hypervisor Injection Exception",
                              "VMM Communication Exception",
                              "Security Exception",
                              "Reserved"};

static void term_puthex(uint64_t value) {
  char hex[] = "0123456789ABCDEF";
  char buffer[17];
  buffer[16] = '\0';

  for (int i = 15; i >= 0; i--) {
    buffer[i] = hex[value & 0xF];
    value >>= 4;
  }

  term_puts("0x");
  term_puts(buffer);
}

void isr_handler(registers_t *r) {
  term_clear();

  uint64_t n = r->int_no;

  term_puts("ISR ");
  term_puthex(n);
  term_puts(" triggered: ");

  if (n < 32)
    term_puts(exception_messages[n]);
  else
    term_puts("unknown interrupt");

  term_putc((unsigned char)'\n');
  hcf();
}

static inline void pic_disable(void) {
  port_byte_out(0x21, 0xFF);
  port_byte_out(0xA1, 0xFF);
}

// void register_interrupt_handler(uint8_t n, isr_t handler) {
//   interrupt_handlers[n] = handler;
// }

// // --- Helpers for MSR access ---
// static inline uint64_t rdmsr(uint32_t msr) {
//   uint32_t low, high;
//   asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
//   return ((uint64_t)high << 32) | low;
// }

// static inline void wrmsr(uint32_t msr, uint64_t value) {
//   uint32_t low = (uint32_t)value;
//   uint32_t high = (uint32_t)(value >> 32);
//   asm volatile("wrmsr" : : "c"(msr), "a"(low), "d"(high));
// }

// // --- Check x2APIC support via CPUID ---
// static bool cpu_supports_x2apic(void) {
//   uint32_t eax, ebx, ecx, edx;
//   asm volatile("cpuid"
//                : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
//                : "a"(1), "c"(0));
//   return (ecx & (1 << 21)) != 0; // x2APIC bit
// }

// // --- LAPIC init in x2APIC mode ---
// void lapic_init_x2apic(void) {
//   if (!cpu_supports_x2apic()) {
//     Console.puts("x2APIC not supported!\n");
//     return;
//   }

//   // Enable x2APIC + APIC globally
//   uint64_t apic_base = rdmsr(0x1B);
//   apic_base |= (1ULL << 10) |
//                (1ULL << 11); // bit 10 = x2APIC, bit 11 = global APIC enable
//   wrmsr(0x1B, apic_base);

//   Console.puts("x2APIC enabled\n");

//   // Read LAPIC ID
//   uint32_t lapic_id = (uint32_t)rdmsr(X2APIC_ID_MSR);
//   Console.puts("LAPIC ID: ");
//   term_puthex(lapic_id);
//   Console.puts("\n");

//   // Set Spurious Vector = 0xFF, enable APIC
//   wrmsr(X2APIC_SVR_MSR, 0x100 | 0xFF);
//   Console.puts("SVR set to 0xFF\n");
// }

// // --- Send EOI ---
// static inline void lapic_eoi(void) { wrmsr(X2APIC_EOI_MSR, 0); }

void isr_install() {
  memset(idt, 0, sizeof(idt));

  pic_disable();

  // internal ISRs

  set_idt_gate(0, (uintptr_t)isr0);
  set_idt_gate(1, (uintptr_t)isr1);
  set_idt_gate(2, (uintptr_t)isr2);
  set_idt_gate(3, (uintptr_t)isr3);
  set_idt_gate(4, (uintptr_t)isr4);
  set_idt_gate(5, (uintptr_t)isr5);
  set_idt_gate(6, (uintptr_t)isr6);
  set_idt_gate(7, (uintptr_t)isr7);
  set_idt_gate(8, (uintptr_t)isr8);
  set_idt_gate(9, (uintptr_t)isr9);
  set_idt_gate(10, (uintptr_t)isr10);
  set_idt_gate(11, (uintptr_t)isr11);
  set_idt_gate(12, (uintptr_t)isr12);
  set_idt_gate(13, (uintptr_t)isr13);
  set_idt_gate(14, (uintptr_t)isr14);
  set_idt_gate(15, (uintptr_t)isr15);
  set_idt_gate(16, (uintptr_t)isr16);
  set_idt_gate(17, (uintptr_t)isr17);
  set_idt_gate(18, (uintptr_t)isr18);
  set_idt_gate(19, (uintptr_t)isr19);
  set_idt_gate(20, (uintptr_t)isr20);
  set_idt_gate(21, (uintptr_t)isr21);
  set_idt_gate(22, (uintptr_t)isr22);
  set_idt_gate(23, (uintptr_t)isr23);
  set_idt_gate(24, (uintptr_t)isr24);
  set_idt_gate(25, (uintptr_t)isr25);
  set_idt_gate(26, (uintptr_t)isr26);
  set_idt_gate(27, (uintptr_t)isr27);
  set_idt_gate(28, (uintptr_t)isr28);
  set_idt_gate(29, (uintptr_t)isr29);
  set_idt_gate(30, (uintptr_t)isr30);
  set_idt_gate(31, (uintptr_t)isr31);

  load_idt();
}

// void irq_handler(registers_t *r) {
//   if (interrupt_handlers[r->int_no]) {
//     interrupt_handlers[r->int_no](r);
//   }

//   lapic_eoi();
// }
