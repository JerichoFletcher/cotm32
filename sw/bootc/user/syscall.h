#pragma once

#include "int.h"
#include "sys/enums.h"

static inline void putc(char c) {
    asm volatile(
        "li a7, %0  \n"
        "mv a0, %1  \n"
        "ecall      \n" ::"i"(SyscallCode_PUTC),
        "r"(c)
        : "a0", "a7"
    );
}

static inline char getc(void) {
    char c;
    asm volatile(
        "li a7, %1  \n"
        "ecall      \n"
        "mv %0, a0  \n"
        : "=r"(c)
        : "i"(SyscallCode_GETC)
        : "a7"
    );
    return c;
}

static inline void puts(const char* s, size_t len) {
    asm volatile(
        "li a7, %0  \n"
        "mv a0, %1  \n"
        "mv a1, %2  \n"
        "ecall      \n" ::"i"(SyscallCode_PUTS),
        "r"(s),
        "r"(len)
        : "a0", "a1", "a7"
    );
}
