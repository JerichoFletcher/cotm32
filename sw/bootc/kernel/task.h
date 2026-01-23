#pragma once

#include "context.h"
#include "priv/enums.h"
#include "trap/trap.h"
#include "bool.h"

#define TIME_SLICE_BASE     10
#define TIME_SLICE_PRIO_SCL 1

typedef enum TaskState {
    TaskState_NOT_CREATED = 0,
    TaskState_READY,
    TaskState_RUNNING,
    TaskState_BLOCKED_IRQ,
    TaskState_TERMINATED,
} TaskState;

typedef struct Task {
    Context ctx;
    size_t id;
    size_t stack_base;
    size_t stack_size;
    size_t priority;
    size_t time_slice;
    TaskState state;
} Task;

Task* create_task(void (*entry)(void), size_t priority, PrivMode priv);

void task_set_mpie(Task* task, bool_t mpie);
void terminate_task(Task* task);
