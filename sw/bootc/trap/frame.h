#pragma once

#include "int.h"

typedef struct TrapFrame {
    uint32_t regs[32];
    uint32_t mepc;
    uint32_t mcause;
    uint32_t mstatus;
} TrapFrame;
