set(CPU_INCLUDES
    "src/cpu/gdt/gdt.h"
    "src/cpu/idt/idt.h"
    "src/cpu/irq/util.h"
    "src/cpu/isr/isr.h"
    "src/cpu/multithreading/mutex.h" "src/cpu/multithreading/spinlock.h"
)

set(DRIVERS_INCLUDES "src/drivers/ports.h" "src/drivers/serial.h")

set(KERNEL_INCLUDES
    "src/kernel/acpi/acpi_bus.h" "src/kernel/acpi/acpi.h" "src/kernel/acpi/table.h" "src/kernel/acpi/uacpi_config.h"
    "src/kernel/console.h" "src/kernel/font.h" "src/kernel/globals.h" "src/kernel/kernel.h" "src/kernel/limine.h" "src/kernel/macro.h" "src/kernel/nanoprintf" "src/kernel/pmm.h" "src/kernel/util.h" "src/kernel/va_list.h"
)
