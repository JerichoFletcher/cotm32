#include "kernel/task.h"
#include "kernel/scheduler.h"
#include "trap/trap.h"
#include "sys/ksys.h"
#include "clint.h"
#include "csr.h"
#include "context.h"
#include "int.h"

#include "user/syscall.h"

extern void task1(void);
extern void task2(void);

void idle(void) {
    yield();
    for (;;) {
        wait_until_interrupt();
    }
}

__attribute__((noreturn))
void kernel_main(void) {
    set_interrupt(Interrupt_M_TIMER, TRUE);
    set_interrupt(Interrupt_M_EXTERNAL, TRUE);

    uint64_t t = get_time();
    set_timecmp(t + 10000);

    if (create_task(idle, 0, PrivMode_M, TRUE)) {
        create_task(task1, 100, PrivMode_U, TRUE);
        create_task(task2, 100, PrivMode_U, TRUE);

        sp_to_mscratch();
        mret_to_context(&current_task()->ctx);
    } else {
        k_puts("Failed to create task!\n", 23);
        for (;;);
    }
}
