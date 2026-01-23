#include "kernel/scheduler.h"
#include "kernel/task.h"
#include "kernel/stack.h"
#include "trap/trap.h"
#include "context.h"
#include "bool.h"

#include "sys/ksys.h"
#include "panic.h"

Task tasks[MAX_TASKS] = {0};
size_t current_tid = 0;
size_t n_task = 0;

void start_schedule(Task* entrypoint) {
    if (entrypoint == NULL ||
        entrypoint->state != TaskState_READY ||
        entrypoint != &tasks[entrypoint->id]
    ) {
        k_puts("Invalid entrypoint task\n", 24);
        panic();
    }
    current_tid = entrypoint->id;
    mret_to_context(&entrypoint->ctx);
}

Task* alloc_new_task(void) {
    for (size_t tid = 0; tid < MAX_TASKS; tid++) {
        if (tasks[tid].state == TaskState_NOT_CREATED) {
            Task* task = &tasks[tid];
            task->id = tid;
            task->state = TaskState_READY;
            n_task++;

            return task;
        }
    }

    return NULL;
}

void clear_task(Task* task) {
    free_stack(task->id);
    task->state = TaskState_NOT_CREATED;
    n_task--;
}

void schedule(void) {
    if (tasks[current_tid].state == TaskState_RUNNING) {
        tasks[current_tid].state = TaskState_READY;
    }

    Task* next = NULL;
    bool_t want_swap = FALSE;

    for (size_t i = 0; i < MAX_TASKS; i++) {
        size_t next_tid = (current_tid + i) % MAX_TASKS;
        
        if (tasks[next_tid].state == TaskState_READY) {
            if (next == NULL ||
                next->priority < tasks[next_tid].priority ||
                (want_swap && next->priority == tasks[next_tid].priority && next_tid != current_tid)
            ) {
                want_swap = next_tid == current_tid;
                next = &tasks[next_tid];
            }
        }
    }

    if (next != NULL) {
        if (tasks[current_tid].state == TaskState_TERMINATED) {
            clear_task(&tasks[current_tid]);
        }
        current_tid = next->id;
        tasks[current_tid].state = TaskState_RUNNING;
    } else {
        panic();
    }
}

size_t wake_irq_tasks(Interrupt interr) {
    size_t interr_bit = bits_interr(interr);
    size_t num_awaken = 0;

    for (size_t i = 0; i < MAX_TASKS; i++) {
        Task* task = &tasks[i];
        if (task->state == TaskState_BLOCKED_IRQ && !!(task->wait_irq_mask & interr_bit)) {
            task->state = TaskState_READY;
            
            for (size_t i = 0; i < 8 * sizeof(size_t); i++) {
                size_t interr_bit = (task->wait_irq_mask >> i) & 1;
                if (interr_bit) {
                    task_set_interr(task, (Interrupt)i, FALSE);
                }
            }
            task->wait_irq_mask = 0;
            num_awaken++;
        }
    }
    return num_awaken;
}
