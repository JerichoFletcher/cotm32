#pragma once

#include "bool.h"
#include "int.h"
#include "trap/enums.h"

/// @brief Gets the appropriate bitmask for an interrupt.
/// @param interr The interrupt type.
/// @return The bitmask representing the interrupt.
static inline size_t bits_interr(Interrupt interr) {
    return 1 << interr;
}

/// @brief Stalls the processor until an enabled interrupt is pending.
/// @brief Emits a `wfi` instruction. 
static inline void wait_for_interrupt(void) {
    asm volatile("wfi" : : : "memory");
}

/// @brief Sets the trap vector, the entrypoint that will be entered when a trap triggers.
/// @param fn The trap entrypoint function.
static inline void set_trap_entry(void (*fn)(void)) {
    asm volatile(
        "andi   %0, %0, ~3  \n"
        "csrw   mtvec, %0   \n"
        :
        : "r"(fn)
        : "memory"
    );
}
