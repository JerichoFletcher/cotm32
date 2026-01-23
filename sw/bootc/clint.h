#pragma once

#include "int.h"

/// @brief The `mtime` cycle count of a timer tick.
#define TICK_LENGTH 10

#define MTIMECMP    (volatile uint32_t*)0x02004000
#define MTIMECMPH   (volatile uint32_t*)0x02004004

#define MTIME       (volatile uint32_t*)0x0200bff8
#define MTIMEH      (volatile uint32_t*)0x0200bffc

/// @brief Reads the value of `mtimecmp`.
/// @return The value of `mtimecmp`.
static inline uint64_t get_timecmp(void) {
    uint32_t t_hi = *MTIMECMPH;
    uint32_t t_lo = *MTIMECMP;

    return (((uint64_t)t_hi) << 32) | (uint64_t)t_lo;
}

/// @brief Writes the value of `mtimecmp`.
/// @param t The new value.
static inline void set_timecmp(uint64_t t) {
    uint32_t t_lo = ((t >> 0) & 0xffffffff);
    uint32_t t_hi = ((t >> 32) & 0xffffffff);

    *MTIMECMPH = t_hi;
    *MTIMECMP = t_lo;
}

/// @brief Reads the current time (in `mtime`).
/// @return The value of `mtime`.
static inline uint64_t get_time(void) {
    uint32_t t_hi;
    uint32_t t_lo;
    do {
        t_hi = *MTIMEH;
        t_lo = *MTIME;
    } while (t_hi != *MTIMEH);

    return (((uint64_t)t_hi) << 32) | (uint64_t)t_lo;
}
