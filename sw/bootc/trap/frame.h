#pragma once

#include "context.h"
#include "int.h"

typedef struct TrapFrame {
    Context ctx;
    size_t mcause;
} TrapFrame;
