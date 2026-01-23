#pragma once

#include "bool.h"
#include "int.h"

/// @brief Descriptor for an allocation of stack space for a task.
typedef struct StackDescriptor {
    /// @brief The lower address limit of the stack.
    size_t base;
    /// @brief The size of the stack, in bytes.
    size_t size;
} StackDescriptor;

/// @brief Allocate a fixed-size stack space for a given task.
/// @param owner_tid The ID of the owner task.
/// @return A pointer to the stack space descriptor.
StackDescriptor* alloc_stack(size_t owner_tid);

/// @brief Deallocates the stack space owned by a given task.
/// @param owner_tid The ID of the owner task.
void free_stack(size_t owner_tid);
