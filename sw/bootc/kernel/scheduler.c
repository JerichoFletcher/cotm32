#include "kernel/scheduler.h"
#include "kernel/task.h"
#include "bool.h"

Task tasks[MAX_TASKS];
size_t n_task = 0;
size_t current_tid = 0;

bool_t create_task(size_t* out_tid, void (*entrypoint)(void), size_t stack_base, size_t stack_size) {
    if (n_task >= MAX_TASKS) {
        return FALSE;
    }

    size_t tid = n_task++;
    Task* task = &tasks[tid];

    task->id = tid;
    task->state = TaskState_READY;
    task->stack_base = stack_base;
    task->stack_size = stack_size;

    for (size_t i = 0; i < 32; i++) {
        task->ctx.regs[i] = 0;
    }
    task->ctx.pc = (size_t)entrypoint;
    task->ctx.regs[2] = stack_base + stack_size;
    task->ctx.mstatus = 0x00000088;

    if (!!out_tid) {
        *out_tid = tid;
    }
    return TRUE;
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
