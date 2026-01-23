#pragma once

#include "bool.h"
#include "int.h"
#include "trap/enums.h"

static inline void wait_until_interrupt(void) {
    asm volatile("wfi" : : : "memory");
}

static inline void set_interrupt(Interrupt interr, bool_t enable) {
    if (enable) {
        size_t flag = 1 << interr;
        asm volatile(
            "csrs mie, %0"
            :
            : "r"(flag)
            : "memory"
        );
    } else {
        size_t mask = ~(1 << interr);
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
