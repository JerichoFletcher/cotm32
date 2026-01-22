#pragma once

#include "kernel/task.h"
#include "int.h"

#define MAX_TASKS 8

extern Task tasks[];
extern size_t current_tid;

Task* alloc_new_task(void);

void schedule(void);
static inline Task* current_task(void) {
    return &tasks[current_tid];
}
