#pragma once

#include "int.h"

#define NUM_REGS 32

/// @brief A snapshot of the execution context.
/// @brief Holds all states necessary to restore an execution.
typedef struct Context {
    /// @brief The values of general-purpose registers (GPRs).
    size_t regs[NUM_REGS];
    /// @brief The value of `mstatus`.
    size_t mstatus;
    /// @brief The value of the program counter.
    size_t pc;
} Context;

/// @brief Copies the values from one snapshot to another.
/// @param to The destination snapshot.
/// @param from The source snapshot.
void copy_context(Context* to, Context* from);

/// @brief Restores execution of a context.
/// @brief This will discard the caller's context. As such, this is a one-way operation.
/// @param ctx The snapshot to restore.
extern void enter_context(Context* ctx) __attribute__((noreturn));
