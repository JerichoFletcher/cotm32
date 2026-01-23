#include "kernel/task.h"
#include "kernel/scheduler.h"
#include "kernel/stack.h"
#include "csr.h"

Task* create_task(void (*entry)(void), size_t priority, PrivMode priv) {
    Task* t = alloc_new_task(priority);
    if (!t) {
        return NULL;
    }

    Stack* s = alloc_stack(t->id);
    if (!s) {
        return NULL;
    }
    t->stack_base = s->base;
    t->stack_size = s->size;
    t->priority = priority;
    
    for (size_t i = 0; i < 32; i++) {
        t->ctx.regs[i] = 0;
    }
    t->ctx.regs[2] = s->base + s->size;
    
    t->ctx.mstatus = bits_mpp(priv) | bits_mpie(TRUE);
    t->ctx.pc = (size_t)entry;

    return t;
}

void task_set_mpie(Task* task, bool_t prev_enable_interr) {
    task->ctx.mstatus &= ~bits_mpie(prev_enable_interr);
    task->ctx.mstatus |= bits_mpie(prev_enable_interr);
}

void terminate_task(Task* task) {
    task->state = TaskState_TERMINATED;
}
