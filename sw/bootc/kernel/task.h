#pragma once

#include "context.h"
#include "priv/enums.h"
#include "bool.h"

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
    size_t priority;
    TaskState state;
} Task;

Task* create_task(void (*entry)(void), size_t priority, PrivMode priv, bool_t mie);
void terminate_task(Task* task);
