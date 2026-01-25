#include "user/syscall.h"

#define SYSCALL_CLOBBER_LIST "t0", "t1", "t2", "t3", "t4", "t5", "t6", "memory"

void exit(void) {
    register size_t a7 asm("a7") = SyscallCode_EXIT;

    asm volatile(
        "ecall"
        :
        : "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
    for (;;);
}

void yield(void) {
    register size_t a7 asm("a7") = SyscallCode_YIELD;

    asm volatile(
        "ecall"
        :
        : "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
}

void free(void* ptr) {
    register size_t a0 asm("a0") = (size_t)ptr;
    register size_t a7 asm("a7") = SyscallCode_FREE;

    asm volatile(
        "ecall"
        :
        : "r"(a0), "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
}

void* malloc(size_t size) {
    register size_t a0 asm("a0") = size;
    register size_t a7 asm("a7") = SyscallCode_MALLOC;

    asm volatile(
        "ecall"
        : "+r"(a0)
        : "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
    return (void*)a0;
}

void putc(char c) {
    register size_t a0 asm("a0") = c;
    register size_t a7 asm("a7") = SyscallCode_PUTC;

    asm volatile(
        "ecall"
        :
        : "r"(a0), "r"(a7)
        : SYSCALL_CLOBBER_LIST
    );
}

char getc(void) {
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

void puts(const char* s, size_t len) {
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
