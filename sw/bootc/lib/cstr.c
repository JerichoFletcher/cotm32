#include "lib/cstr.h"

void* memcpy(void* restrict dest, const void* restrict src, size_t count) {
    for (size_t i = 0; i < count; i++) {
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
    }
    return dest;
}

void* memset(void* dest, uint8_t ch, size_t count) {
    for (size_t i = 0; i < count; i++) {
        ((uint8_t*)dest)[i] = ch;
    }
    return dest;
}
