#include "kernel/scheduler.h"
#include "kernel/task.h"
#include "int.h"

Task tasks[MAX_TASKS] = {0};
size_t current_tid = 0;

size_t n_task = 0;
size_t unused_tid = 0;

Task* alloc_new_task(void) {
    for (size_t tid = 0; tid < MAX_TASKS; tid++) {
        if (tasks[tid].state == TaskState_NOT_CREATED) {
            Task* task = &tasks[tid];
            task->id = unused_tid++;
            task->state = TaskState_READY;
            n_task++;

            return task;
        }
    }

    return NULL;
}

void schedule(void) {
    if (tasks[current_tid].state == TaskState_RUNNING) {
        tasks[current_tid].state = TaskState_READY;
    }

    for (size_t i = 1; i < n_task; i++) {
        size_t next_tid = (current_tid + i) % n_task;
        if (tasks[next_tid].state == TaskState_READY) {
            current_tid = next_tid;
            tasks[current_tid].state = TaskState_RUNNING;
            return;
        }
    }

    tasks[current_tid].state = TaskState_RUNNING;
    return;
}
