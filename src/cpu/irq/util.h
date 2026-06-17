#pragma once

#include <uacpi/uacpi.h>

uacpi_cpu_flags irq_save(void);

void irq_restore(uacpi_cpu_flags flags);
