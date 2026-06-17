#pragma once

#include <stdatomic.h>

#define MAX_UACPI_MUTEXES 32

struct mutex {
    atomic_flag locked;
    atomic_bool in_use;
};

static struct mutex mutex_pool[MAX_UACPI_MUTEXES];
