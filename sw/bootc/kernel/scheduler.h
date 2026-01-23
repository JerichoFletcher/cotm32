#pragma once

#include "kernel/task.h"
#include "trap/enums.h"
#include "int.h"

#define MAX_TASKS 8

extern Task tasks[];
extern size_t current_tid;

static inline Task* current_task(void) {
    return &tasks[current_tid];
}

__attribute__((noreturn))
void start_schedule(Task* entrypoint);

Task* alloc_new_task(size_t priority);
void schedule(void);
void block_task_irq(Task* task, Interrupt interr);
size_t wake_irq_tasks(Interrupt interr);
