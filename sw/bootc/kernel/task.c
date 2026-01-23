#include "kernel/task.h"
#include "kernel/scheduler.h"
#include "kernel/stack.h"
#include "csr.h"

Task* create_task(void (*entry)(void), size_t priority, PrivMode priv) {
    Task* t = alloc_new_task();
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
    t->wait_irq_mask = 0;
    
    for (size_t i = 0; i < 32; i++) {
        t->ctx.regs[i] = 0;
    }
    t->ctx.regs[2] = s->base + s->size;
    
    t->ctx.mstatus = bits_mpp(priv);
    t->ctx.mie = bits_interr(Interrupt_M_TIMER);
    t->ctx.pc = (size_t)entry;

    return t;
}

void task_set_mpie(Task* task, bool_t prev_enable_interr) {
    task->ctx.mstatus &= ~bits_mpie(prev_enable_interr);
    task->ctx.mstatus |= bits_mpie(prev_enable_interr);
}

void task_set_interr(Task* task, Interrupt interr, bool_t enable) {
    if (enable) {
        task->ctx.mie |= bits_interr(interr);
    } else {
        task->ctx.mie &= ~bits_interr(interr);
    }
}

void block_task_irq(Task* task, Interrupt interr) {
    task->state = TaskState_BLOCKED_IRQ;
    task->wait_irq_mask |= bits_interr(interr);
    task_set_interr(task, interr, TRUE);
}

void terminate_task(Task* task) {
    task->state = TaskState_TERMINATED;
}
