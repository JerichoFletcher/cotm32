#pragma once

#include "bool.h"
#include "int.h"
#include "trap/enums.h"

static inline size_t bits_interr(Interrupt interr) {
    return 1 << interr;
}

static inline void wait_for_interrupt(void) {
    asm volatile("wfi" : : : "memory");
}

static inline void set_trap_entry(void* addr) {
    asm volatile(
        "andi   %0, %0, ~3  \n"
        "csrw   mtvec, %0   \n"
        :
        : "r"(addr)
        : "memory"
    );
}
