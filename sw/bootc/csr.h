#pragma once

#include "bool.h"
#include "int.h"
#include "priv/enums.h"

/// @brief Gets the appropriate bitmask for `mstatus.MPIE`.
/// @param mpie The value to set.
/// @return The bitmask.
static inline size_t bits_mpie(bool_t mpie) {
    return ((size_t)!!mpie) << 7;
}

/// @brief Gets the appropriate bitmask for `mstatus.MPP`.
/// @param priv The value to set.
/// @return The bitmask.
static inline size_t bits_mpp(PrivMode priv) {
    return ((size_t)priv & 0b11) << 11;
}

/// @brief Assigns a full interrupt-enable mask to `mie`.
/// @param mie The interrupt-enable mask.
static inline void assign_mie(size_t mie) {
    asm volatile("csrw mie, %0" : : "r"(mie) : "memory");
}

/// @brief Stores the current `sp` to `mscratch`.
static inline void sp_to_mscratch(void) {
    register size_t sp asm("sp");
    asm volatile("csrw mscratch, %0" : : "r"(sp) : "memory");
}
