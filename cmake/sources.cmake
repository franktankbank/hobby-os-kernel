set(CPU_SOURCES
    "src/cpu/gdt/gdt.asm" "src/cpu/gdt/gdt.c"
    "src/cpu/idt/idt.c"
    "src/cpu/irq/util.c"
    "src/cpu/isr/isr.asm" "src/cpu/isr/isr.c"
    "src/cpu/pic/pic.c"
)

set(DRIVERS_SOURCES "src/drivers/ports.c" "src/drivers/serial.c" "src/drivers/keyboard.c")

set(KERNEL_SOURCES
    "src/kernel/acpi/acpi.c" "src/kernel/acpi/table.c" "src/kernel/acpi/uacpi_kernel_api.c"
    "src/kernel/console.c" "src/kernel/font.c" "src/kernel/kernel.c" "src/kernel/util.c"
    "src/kernel/paging.c" "src/kernel/pmm.c"
)
