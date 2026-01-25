#pragma once

#include "int.h"

extern char stackalloc_start;

/// @brief The maximum number of stack slots that can be allocated.
#define MAX_STACK_SLOT      8U
/// @brief The size of each stack space, in bytes.
#define STACK_SIZE          1024U
/// @brief The lowest address of stack memory.
#define STACK_SPACE_START   (void*)(&stackalloc_start)
/// @brief The highest address (non-inclusive) of stack memory.
#define STACK_SPACE_END     (void*)((size_t)STACK_SPACE_START + MAX_STACK_SLOT * STACK_SIZE)

/// @brief Descriptor for an allocation of stack space for a task.
typedef struct StackDescriptor {
    /// @brief The lower address limit of the stack.
    void* base;
    /// @brief The size of the stack, in bytes.
    size_t size;
} StackDescriptor;

/// @brief Allocate a fixed-size stack space.
/// @return A pointer to the stack space descriptor, or `NULL` if allocation fails.
StackDescriptor* alloc_stack(void);

/// @brief Frees a stack space to be used later.
/// @param stack The stack descriptor.
void free_stack(StackDescriptor* stack);
