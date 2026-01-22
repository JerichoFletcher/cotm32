#pragma once

#include "int.h"

#define TIME_SLICE 10000

#define MTIMECMP    (volatile uint32_t*)0x02004000
#define MTIMECMPH   (volatile uint32_t*)0x02004004

#define MTIME       (volatile uint32_t*)0x0200bff8
#define MTIMEH      (volatile uint32_t*)0x0200bffc

static inline uint64_t get_timecmp(void) {
    uint32_t t_hi = *MTIMECMPH;
    uint32_t t_lo = *MTIMECMP;

    return (((uint64_t)t_hi) << 32) | (uint64_t)t_lo;
}

static inline void set_timecmp(uint64_t t) {
    uint32_t t_lo = ((t >> 0) & 0xffffffff);
    uint32_t t_hi = ((t >> 32) & 0xffffffff);

    *MTIMECMPH = t_hi;
    *MTIMECMP = t_lo;
}

static inline uint64_t get_time(void) {
    uint32_t t_hi;
    uint32_t t_lo;
    do {
        t_hi = *MTIMEH;
        t_lo = *MTIME;
    } while (t_hi != *MTIMEH);

    return (((uint64_t)t_hi) << 32) | (uint64_t)t_lo;
}
