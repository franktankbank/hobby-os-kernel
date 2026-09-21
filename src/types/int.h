#pragma once

#include <stdint.h>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

#define rd32(p) (*(const u32 *)(p))
#define rd16(p) (*(const u16 *)(p))
