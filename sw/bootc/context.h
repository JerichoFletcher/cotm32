#pragma once

#include "int.h"

#define NUM_REGS 32

typedef struct Context {
    size_t regs[NUM_REGS];
    size_t pc;
    size_t mstatus;
} Context;

void copy_context(Context* to, Context* from);
