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

static inline void set_interrupt(Interrupt interr, bool_t enable) {
    if (enable) {
        size_t flag = bits_interr(interr);
        asm volatile(
            "csrs mie, %0"
            :
            : "r"(flag)
            : "memory"
        );
    } else {
        size_t mask = ~bits_interr(interr);
        asm volatile(
            "csrc mie, %0"
            :
            : "r"(mask)
            : "memory"
        );
    }
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
