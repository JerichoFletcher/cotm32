#pragma once

#include "int.h"

#define MTIMECMP (volatile uint32_t*)0x02004000
#define MTIMECMPH (volatile uint32_t*)0x02004004

static inline uint64_t get_timecmp(void) {
    uint32_t t_hi = *MTIMECMPH;
    uint32_t t_lo;
    do {
        t_lo = *MTIMECMP;
    } while (t_hi != *MTIMECMPH);

    return (((uint64_t)t_hi) << 32) | (uint64_t)t_lo;
}

static inline void set_timecmp(uint64_t t) {
    uint32_t t_lo = ((t >> 0) & 0xffffffff);
    uint32_t t_hi = ((t >> 32) & 0xffffffff);

    *MTIMECMPH = t_hi;
    *MTIMECMP = t_lo;
}
