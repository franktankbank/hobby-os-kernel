#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct freelist_node {
    size_t length; // length in bytes

    struct freelist_node *next;
} freelist_node;
