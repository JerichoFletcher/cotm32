#pragma once

#include "bool.h"
#include "int.h"
#include "priv/enums.h"

static inline size_t bits_mpie(bool_t mpie) {
    return ((size_t)!!mpie) << 7;
}

static inline size_t bits_mpp(PrivMode priv) {
    return ((size_t)priv & 0b11) << 11;
}

static inline void assign_mpie(bool_t mpie) {
    size_t i = bits_mpie(mpie);
    asm volatile(
        "csrc mstatus, %0\n"
        "csrs mstatus, %1\n"
        :
        : "r"(1 << 7), "r"(i)
        : "memory"
    );
}

static inline void assign_mpp(PrivMode priv) {
    size_t i = bits_mpp(priv);
    asm volatile(
        "csrc mstatus, %0\n"
        "csrs mstatus, %1\n"
        :
        : "r"(0b11 << 11), "r"(i)
        : "memory"
    );
}

static inline void sp_to_mscratch(void) {
    asm volatile("csrw mscratch, sp" : : : "memory");
}

static inline void assign_mepc(void (*f)(void)) {
    asm volatile("csrw mepc, %0" : : "r"(f) : "memory");
}
