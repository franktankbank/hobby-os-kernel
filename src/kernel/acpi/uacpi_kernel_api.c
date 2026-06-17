#include "uacpi_config.h"
#include "../va_list.h"
#include "../nanoprintf.h"
#include "../console.h"
#include "../pmm.h"
#include "../globals.h"
#include "../../cpu/multithreading/mutex.h"
#include "../../cpu/multithreading/spinlock.h"
#include "../../cpu/irq/util.h"
#include <uacpi/kernel_api.h>
#include <stdbool.h>
// #include <assert.h>

void uacpi_kernel_log(uacpi_log_level level, const uacpi_char *fmt, ...) {
    char buffer[1024];
    va_list args;

    va_start(args, fmt);
    npf_vsnprintf(buffer, sizeof(buffer), fmt, args);

    char* prefix;

    switch (level) {
        case UACPI_LOG_DEBUG:
            prefix = "[DEBUG]";
            break;
        case UACPI_LOG_TRACE:
            prefix = "[TRACE]";
            break;
        case UACPI_LOG_INFO:
            prefix = "[INFO]";
            break;
        case UACPI_LOG_WARN:
            prefix = "[WARN]";
            break;
        case UACPI_LOG_ERROR:
            prefix = "[ERROR]";
            break;
        default:
            prefix = "[UNKNOWN]";
            break;
    }

    SerialConsole.printf("%s %s", prefix, buffer);
}

// Map rsdp from a virutal memory address to a physical memory address
uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *rsdp_out) {
    *rsdp_out = from_hhdm(limine_parsed_data.rsdp_table_addr);
    return UACPI_STATUS_OK;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
    (void)len;
    return make_hhdm(addr);
}

void uacpi_kernel_unmap(void *addr, uacpi_size len) {
    /*
     * Physical memory is permanently accessible through the HHDM,
     * so no temporary mapping was created and nothing needs to
     * be unmapped.
     */
     (void)addr;
     (void)len;
}

uacpi_handle uacpi_kernel_create_mutex(void) {
    for (size_t i = 0; i < MAX_UACPI_MUTEXES; i++) {
        bool expected = false;

        if (atomic_compare_exchange_strong(&mutex_pool[i].in_use, &expected, true)) {
            atomic_flag_clear(&mutex_pool[i].locked);
            return &mutex_pool[i];
        }
    }

    return NULL;
}

void uacpi_kernel_free_mutex(uacpi_handle handle) {
    struct mutex *m = handle;

    atomic_flag_clear(&m->locked);
    atomic_store(&m->in_use, false);
}

uacpi_status uacpi_kernel_acquire_mutex(uacpi_handle handle, uacpi_u16 timeout) {
    struct mutex *m = handle;

    if (timeout == 0) {
        if (!atomic_flag_test_and_set(&m->locked))
            return UACPI_STATUS_OK;

        return UACPI_STATUS_TIMEOUT;
    }

    while (atomic_flag_test_and_set(&m->locked))
        __builtin_ia32_pause();

    return UACPI_STATUS_OK;
}

void uacpi_kernel_release_mutex(uacpi_handle handle) {
    struct mutex *m = handle;
    atomic_flag_clear(&m->locked);
}

uacpi_cpu_flags uacpi_kernel_lock_spinlock(uacpi_handle handle) {
    struct spinlock *lock = handle;

    uacpi_cpu_flags flags = irq_save();

    while (atomic_flag_test_and_set_explicit(&lock->locked, memory_order_acquire)) {
        __builtin_ia32_pause();
    }

    return flags;
}

void uacpi_kernel_unlock_spinlock(uacpi_handle handle, uacpi_cpu_flags flags) {
    struct spinlock *lock = handle;

    atomic_flag_clear_explicit(&lock->locked, memory_order_release);

    irq_restore(flags);
}

uacpi_handle uacpi_kernel_create_spinlock(void) {
    for (size_t i = 0; i < MAX_UACPI_SPINLOCKS; i++) {
        bool expected = false;

        if (atomic_compare_exchange_strong(&spinlock_pool[i].in_use, &expected, true)) {
            atomic_flag_clear(&spinlock_pool[i].lock.locked);

            return &spinlock_pool[i].lock;
        }
    }

    return NULL;
}

void uacpi_kernel_free_spinlock(uacpi_handle handle) {
    struct spinlock *lock = handle;

    // assert(!atomic_flag_test_and_set(&lock->locked));
    atomic_flag_clear(&lock->locked);

    struct spinlock_slot *slot = (struct spinlock_slot *)((uintptr_t)lock - offsetof(struct spinlock_slot, lock));

    atomic_store(&slot->in_use, false);
}
