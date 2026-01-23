#pragma once

#include "context.h"
#include "priv/enums.h"
#include "trap/trap.h"
#include "bool.h"

typedef enum TaskState {
    TaskState_NOT_CREATED = 0,
    TaskState_READY,
    TaskState_RUNNING,
    TaskState_BLOCKED_IRQ,
    TaskState_TERMINATED,
} TaskState;

typedef struct Task {
    Context ctx;
    size_t stack_base;
    size_t stack_size;
    size_t id;
    size_t priority;
    TaskState state;
    size_t wait_irq_mask;
} Task;

Task* create_task(void (*entry)(void), size_t priority, PrivMode priv);

void task_set_mpie(Task* task, bool_t mpie);
void task_set_interr(Task* task, Interrupt interr, bool_t enable);

void block_task_irq(Task* task, Interrupt interr);
void terminate_task(Task* task);
