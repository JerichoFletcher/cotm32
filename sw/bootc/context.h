#pragma once

#include "int.h"

#define NUM_REGS 32

typedef struct Context {
    size_t regs[NUM_REGS];
    size_t mstatus;
    size_t mie;
    size_t pc;
} Context;

void copy_context(Context* to, Context* from);

__attribute__((noreturn))
extern void mret_to_context(Context* ctx);
