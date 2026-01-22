#pragma once

#include "bool.h"
#include "int.h"

typedef struct Stack {
    size_t base;
    size_t size;
} Stack;

Stack* alloc_stack(size_t owner_tid);
void free_stack(size_t owner_tid);
