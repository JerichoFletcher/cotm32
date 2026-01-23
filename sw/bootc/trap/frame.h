#pragma once

#include "context.h"
#include "int.h"

/// @brief Contains information about the current trap.
typedef struct TrapFrame {
    /// @brief The context of the task that triggered the trap.
    Context ctx;
    /// @brief The cause of the trap.
    size_t mcause;
} TrapFrame;
