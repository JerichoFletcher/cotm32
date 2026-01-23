#include "kernel/task.h"
#include "kernel/scheduler.h"
#include "trap/trap.h"
#include "sys/ksys.h"
#include "clint.h"
#include "csr.h"
#include "int.h"
#include "panic.h"

extern void entry(void);

void idle(void) {
    for (;;) {
        wait_for_interrupt();
    }
}

__attribute__((noreturn))
void kernel_main(void) {
    uint64_t t = get_time();
    set_timecmp(t + TICK_LENGTH);

    Task* t_idle = create_task(idle, 0, PrivMode_M);
    if (t_idle) {
        task_set_mpie(t_idle, TRUE);

        Task* t_entry = create_task(entry, 10, PrivMode_U);
    
        sp_to_mscratch();
        start_schedule(t_entry);
    } else {
        k_puts("Panic: failed to create task\n", 29);
        panic();
    }
}
