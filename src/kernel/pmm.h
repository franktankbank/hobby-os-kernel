#include <stdint.h>

#include "globals.h"

#define MAKE_HHDM(x) (void *)((uintptr_t)x + limine_parsed_data.hhdm_offset)
#define FROM_HHDM(x) (void *)((uintptr_t)x - limine_parsed_data.hhdm_offset)
