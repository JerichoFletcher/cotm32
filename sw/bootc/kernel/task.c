#include "kernel/task.h"
#include "kernel/scheduler.h"
#include "kernel/stack.h"
#include "csr.h"

#include "sys/ksys.h"
#include "panic.h"

Task tasks[MAX_TASKS] = {0};
uint8_t current_slot = 0;
size_t n_task = 0;
uint16_t t_gen = 0;

Task* get_task_of_id(task_id_t tid) {
    size_t slot = TASK_SLOT(tid);
    if (slot < MAX_TASKS) {
        Task* task = &tasks[slot];
        if (task->id == tid) return task;
    }
    return NULL;
}

Task* create_task(void (*entry)(void), PrivMode priv, size_t priority) {
    if (n_task == MAX_TASKS) return NULL;
    
    // Find an unallocated task entry
    Task* task = NULL;
    size_t slot;
    for (slot = 0; slot < MAX_TASKS; slot++) {
        if (tasks[slot].state == TaskState_NOT_CREATED) {
            task = &tasks[slot];
            break;
        }
    }
    if (!task) return NULL;
    
    task->id = TASK_ID(slot, t_gen++);
    task->priority = priority;

    StackDescriptor* s = alloc_stack();
    if (!s) return NULL;

    task->stack = s;
    task->priority = priority;
    task->time_slice = SCHED_QUANTUM_TICKS + priority * SCHED_PRIORITY_BONUS_TICKS;
    task->state = TaskState_READY;
    
    for (size_t i = 0; i < 32; i++) {
        task->ctx.regs[i] = 0;
    }
    task->ctx.regs[2] = (size_t)s->base + s->size;
    
    task->ctx.mstatus = bits_mpp(priv) | bits_mpie(TRUE);
    task->ctx.pc = (size_t)entry;

    n_task++;
    return task;
}

task_id_t current_task(void) {
    return tasks[current_slot].id;
}

void set_current_task(task_id_t tid) {
    Task* task = get_task_of_id(tid);
    if (task != NULL && task->state == TaskState_READY) {
        current_slot = TASK_SLOT(tid);
    } else {
        k_puts("Panic: attempting to switch to a non-ready task");
        panic();
    }
}

task_id_t spawn_task(void (*entry)(void), PrivMode priv, size_t priority) {
    Task* task = create_task(entry, priv, priority);
    if (task == NULL) return -1;

    if (add_task_to_schedule(task->id)) {
        return task->id;
    } else {
        destroy_task(task->id);
        return -1;
    }
}

void enter_task(task_id_t tid) {
    Task* task = get_task_of_id(tid);
    if (task == NULL) {
        k_puts("Panic: entering an invalid task");
        panic();
    }
    enter_context(&task->ctx);
}

void destroy_task(task_id_t tid) {
    Task* task = get_task_of_id(tid);
    if (task != NULL && task->state != TaskState_NOT_CREATED) {
        free_stack(task->stack);
        task->state = TaskState_NOT_CREATED;
        n_task--;
    }
    /// TODO: Add a way to keep track of and free owned heap
}

bool_t task_exists(task_id_t tid) {
    return get_task_of_id(tid) != NULL;
}

bool_t get_task_state(task_id_t tid, TaskState* out) {
    Task* task = get_task_of_id(tid);
    if (task != NULL) {
        *out = task->state;
        return TRUE;
    }
    return FALSE;
}

bool_t get_task_priority(task_id_t tid, size_t* out) {
    Task* task = get_task_of_id(tid);
    if (task != NULL) {
        *out = task->priority;
        return TRUE;
    }
    return FALSE;
}

bool_t get_task_time_slice(task_id_t tid, size_t* out) {
    Task* task = get_task_of_id(tid);
    if (task != NULL) {
        *out = task->time_slice;
        return TRUE;
    }
    return FALSE;
}

void set_task_ready(task_id_t tid) {
    Task* task = get_task_of_id(tid);
    if (task != NULL) {
        task->state = TaskState_READY;
    }
}

void set_task_running(task_id_t tid) {
    Task* task = get_task_of_id(tid);
    if (task != NULL) {
        task->state = TaskState_RUNNING;
    }
}

void set_task_blocked_irq(task_id_t tid) {
    Task* task = get_task_of_id(tid);
    if (task != NULL) {
        task->state = TaskState_BLOCKED_IRQ;
    }
}

void set_task_time_slice(task_id_t tid, size_t time_slice) {
    Task* task = get_task_of_id(tid);
    if (task != NULL) {
        task->time_slice = time_slice;
    }
}

void switch_to_next_task(Context* ctx) {
    size_t prev_id = current_task();
    Task* prev = get_task_of_id(prev_id);
    if (prev != NULL && prev->state != TaskState_TERMINATED) {
        copy_context(&prev->ctx, ctx);
    }
    schedule();

    size_t next_id = current_task();
    Task* next = get_task_of_id(next_id);
    if (next != NULL && prev_id != next_id) {
        copy_context(ctx, &next->ctx);
    }
}

void terminate_task(task_id_t tid) {
    Task* task = get_task_of_id(tid);
    if (task != NULL) {
        task->state = TaskState_TERMINATED;
    }
}
