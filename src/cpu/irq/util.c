#include "util.h"

uacpi_cpu_flags irq_save(void) {
    uacpi_cpu_flags flags;

    __asm__ volatile(
        "pushfq\n\t"
        "popq %0\n\t"
        "cli"
        : "=r"(flags)
        :
        : "memory"
    );

    return flags;
}

void irq_restore(uacpi_cpu_flags flags) {
    if (flags & (1ULL << 9))
        __asm__ volatile("sti" ::: "memory");
}
