#include "kernel/scheduler.h"
#include "kernel/stack.h"
#include "trap/trap.h"
#include "sys/ksys.h"
#include "clint.h"
#include "csr.h"
#include "context.h"
#include "int.h"

extern void task1(void);
extern void task2(void);

Task* create_task(void (*entry)(void)) {
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
    
    for (size_t i = 0; i < 32; i++) {
        t->ctx.regs[i] = 0;
    }
    t->ctx.pc = (size_t)entry;
    t->ctx.mstatus = 0x00000088;
    t->ctx.regs[2] = s->base + s->size;

    return t;
}

__attribute__((noreturn))
void kernel_main(void) {
    set_interrupt(Interrupt_M_TIMER, TRUE);

    uint64_t t = get_time();
    set_timecmp(t + 10000);

    if (create_task(task1) && create_task(task2)) {
        sp_to_mscratch();
        mret_to_context(&current_task()->ctx);
    } else {
        k_puts("Failed to create task!\n", 23);
        while (TRUE);
    }
}
