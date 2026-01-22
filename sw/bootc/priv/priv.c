#include "priv/priv.h"

void enter(void (*f)(void), PrivMode next_priv, bool_t mie) {
    uint32_t i = !!mie << 7; // MPIE
    asm volatile("csrs mstatus, %0" ::"r"(i));

    i = ((next_priv & 0b11) << 11); // MPP
    asm volatile(
        "csrc mstatus, %0\n"
        "csrs mstatus, %1\n"
        ::"r"((0b11 << 11)), "r"(i)
    );

    // Call f and transition privmode
    asm volatile(
        "csrw mepc, %0\n"
        "mret\n"
        ::"r"(f)
    );
    while (TRUE);
}
