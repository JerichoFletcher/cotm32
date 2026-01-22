#pragma once

#include "bool.h"
#include "kernel/task.h"

#define MAX_TASKS 8

extern Task tasks[];
extern size_t n_task;
extern size_t current_tid;

bool_t create_task(size_t* out_tid, void (*entrypoint)(void), size_t stack_base, size_t stack_size);

void schedule(void);
static inline Task* current_task(void) {
    return &tasks[current_tid];
}
