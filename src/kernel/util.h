#pragma once

#include <stddef.h>

#define IS_BIT_SET(n, pos) ((n >> pos) & 1)

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *restrict dest, const void *restrict src, size_t n);

void *memset(void *s, int c, size_t n);

void *memmove(void *dest, const void *src, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);

size_t strnlen(const char *s, size_t maxlen);

size_t strlen(const char *s);

void reverse(char s[]);

void int_to_string(int n, char str[]);

void hcf(void);

int strncmp(const char *s1, const char *s2, size_t n);

char *strcat(char *dest, const char *src);
