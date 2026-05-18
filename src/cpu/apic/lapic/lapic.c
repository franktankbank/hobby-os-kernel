#include "lapic.h"

#include "../../isr/isr.h"

uint32_t lapic_timer_ticks_per_ms = 0;
bool lapic_status = false;
uint64_t lapic_base = 0;

bool is_lapic_enabled() {
    return lapic_status;
}

void set_lapic_base(uint64_t base) {
    lapic_base = base;
}

void lapic_write_reg(uint64_t reg, uint32_t value) {
    if (!lapic_base)
        return;

    cpu_reg_write((uint32_t *)(lapic_base + reg), value);
}

uint64_t lapic_read_reg(uint64_t reg) {
    if (!lapic_base)
        return 0;

    return cpu_reg_read((uint32_t *)(lapic_base + reg));
}

uint64_t lapic_get_id() {
    return lapic_read_reg(LAPIC_ID_REG);
}

void lapic_send_eoi() {
    lapic_write_reg(LAPIC_EOI_REG, 0);
}
