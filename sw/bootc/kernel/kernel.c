#include "kernel/task.h"
#include "kernel/scheduler.h"
#include "kernel/stack.h"
#include "kernel/heap.h"
#include "trap/trap.h"
#include "sys/ksys.h"
#include "clint.h"
#include "csr.h"
#include "int.h"
#include "panic.h"

extern void user_entry(void);

void idle(void) {
    for (;;) {
        wait_for_interrupt();
    }
}

__attribute__((noreturn))
void kernel_main(void) {
    Task* t_idle = create_task(idle, 0, PrivMode_M);
    Task* t_entry = create_task(user_entry, 50, PrivMode_U);
    
    if (t_idle && t_entry) {
        uint64_t time = get_time();
        set_timecmp(time + TICK_LENGTH);
        sp_to_mscratch();
        
        init_heap(STACK_SPACE_END, 32768U);
        start_schedule(t_entry);
    } else {
        k_puts("Panic: failed to create task\n", 29);
        panic();
    }
}
