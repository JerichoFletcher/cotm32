#pragma once

#include "context.h"

typedef enum TaskState {
    TaskState_NOT_CREATED   = 0,
    TaskState_READY         = 1,
    TaskState_RUNNING       = 2,
    TaskState_TERMINATED    = 3,
} TaskState;

typedef struct Task {
    Context ctx;
    size_t stack_base;
    size_t stack_size;
    size_t id;
    TaskState state;
} Task;
