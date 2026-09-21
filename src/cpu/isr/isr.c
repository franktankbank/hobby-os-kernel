#include "isr.h"
#include "../../kernel/console.h"
#include "../../kernel/util.h"
#include "../idt/idt.h"

isrHandler isr_handlers[IDT_MAX_DESCRIPTORS];

static const char *const exceptions[] = {"Divide by zero error",
                                         "Debug",
                                         "Non-maskable Interrupt",
                                         "Breakpoint",
                                         "Overflow",
                                         "Bound Range Exceeded",
                                         "Invalid Opcode",
                                         "Device Not Available",
                                         "Double Fault",
                                         "Coprocessor Segment Overrun",
                                         "Invalid TSS",
                                         "Segment Not Present",
                                         "Stack-Segment Fault",
                                         "General Protection Fault",
                                         "Page Fault",
                                         "",
                                         "x87 Floating-Point Exception",
                                         "Alignment Check",
                                         "Machine Check",
                                         "SIMD Floating-Point Exception",
                                         "Virtualization Exception",
                                         "Control Protection Exception ",
                                         "",
                                         "",
                                         "",
                                         "",
                                         "",
                                         "",
                                         "Hypervisor Injection Exception",
                                         "VMM Communication Exception",
                                         "Security Exception",
                                         ""};

static void isr_exceptions(void *ctx) {
    registers_t *regs = ctx;

    Console.config.set_bg(0xCC0000);
    Console.config.set_font(28, true);
    Console.clear();
    int center_row = (Console.cursor.get_rows() / 2) - 4;
    Console.cursor.set_cursor(Console.cursor.get_current_col(), center_row - 1);
    Console.config.set_bg(0xFFFFFF);
    Console.config.set_fg(0xCC0000);
    Console.centered_puts(" Panic! ");
    Console.config.set_bg(0xCC0000);
    Console.config.set_fg(0xFFFFFF);
    Console.puts("\n\n");
    Console.centered_printf("Fatal exception %#llx triggered at RIP=%#llx", regs->interrupt, regs->rip);
    Console.puts("\n\n");
    Console.centered_printf("MSG=\"%s\"", exceptions[regs->interrupt]);
    Console.putc('\n');
    Console.centered_printf("ERR=%#llx", regs->error);
    switch (regs->interrupt) {
        case 14:
            u64 cr2;

            asm volatile("mov %%cr2, %0" : "=r"(cr2));
                SerialConsole.printf(
                    "Page fault: CR2=%p RIP=%p ERR=%lx\n",
                    (void *)cr2,
                    (void *)regs->rip,
                    regs->error
                );
    }
    SerialConsole.printf("Fatal exception %#llx: %s with error code %#llx triggered... Dumping registers:\n\nRAX=%#llx\nRBX=%#llx\nRCX=%#llx\nRDX=%#llx\nRSI=%#llx\nRDI=%#llx\nRSP=%#llx\nRBP=%#llx\nR8=%#llx\nR9=%#llx\nR10=%#llx\nR11=%#llx\nR12=%#llx\nR13=%#llx\nR14=%#llx\nR15=%#llx\nRIP=%#llx\nCS=%#llx\nDS=%#llx\nSS=%#llx\nRFLAGS=%#llx\n", regs->interrupt, exceptions[regs->interrupt], regs->error, regs->rax, regs->rbx, regs->rcx, regs->rdx, regs->rsi, regs->rdi, regs->rsp, regs->rbp, regs->r8, regs->r9, regs->r10, regs->r11, regs->r12, regs->r13, regs->r14, regs->r15, regs->rip, regs->cs, regs->ds, regs->ss, regs->rflags);
    hcf();
}

void isr_install() {
    for (int i = 0; i < 32; i++) {
        isr_registerHandler(i, isr_exceptions);
    }
}

void isr_handler(void *ctx) {
    registers_t *regs = ctx;

    if (isr_handlers[regs->interrupt] != NULL) {
        isr_handlers[regs->interrupt](regs);
    } else if (regs->interrupt >= 32) {
      Console.printf("Unhandled interrupt %#llx\n", regs->interrupt);
      SerialConsole.printf("Unhandled interrupt %#llx\n", regs->interrupt);
    }
}

void isr_registerHandler(int interrupt, isrHandler handler) {
    isr_handlers[interrupt] = handler;
    idt_gate_enable(interrupt);
}
