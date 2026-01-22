#pragma once

#include "context.h"

typedef enum TaskState {
    TaskState_READY,
    TaskState_RUNNING,
    TaskState_TERMINATED,
} TaskState;

typedef struct Task {
    Context ctx;
    size_t stack_base;
    size_t stack_size;
    size_t id;
    TaskState state;
} Task;
