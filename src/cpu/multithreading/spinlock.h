#pragma once

#include <stdatomic.h>

#define MAX_UACPI_SPINLOCKS 16

struct spinlock {
    atomic_flag locked;
};

struct spinlock_slot {
    atomic_bool in_use;
    struct spinlock lock;
};

static struct spinlock_slot spinlock_pool[MAX_UACPI_SPINLOCKS];
