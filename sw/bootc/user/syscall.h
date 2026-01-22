#pragma once

#include "sys/enums.h"
#include "int.h"

#define SYSCALL_CLOBBER_LIST "t0", "t1", "t2", "t3", "t4", "t5", "t6", "memory"

__attribute__((noreturn))
static inline void exit(void) {
    register size_t a7 asm("a7") = SyscallCode_EXIT;

    asm volatile(
        "ecall"
        :
        : "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
    for (;;);
}

static inline void yield(void) {
    register size_t a7 asm("a7") = SyscallCode_YIELD;

    asm volatile(
        "ecall"
        :
        : "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
}

static inline void putc(char c) {
    register size_t a0 asm("a0") = c;
    register size_t a7 asm("a7") = SyscallCode_PUTC;

    asm volatile(
        "ecall"
        :
        : "r"(a0), "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
}

static inline char getc(void) {
    register size_t a0 asm("a0");
    register size_t a7 asm("a7") = SyscallCode_GETC;
    
    asm volatile(
        "ecall"
        : "=r"(a0)
        : "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
    
    return (char)a0;
}

static inline void puts(const char* s, size_t len) {
    register size_t a0 asm("a0") = (size_t)s;
    register size_t a1 asm("a1") = len;
    register size_t a7 asm("a7") = SyscallCode_PUTS;

    asm volatile(
        "ecall"
        :
        : "r"(a0), "r"(a1), "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
}
