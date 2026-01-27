#include "kernel/kernel.h"
#include "kernel/task.h"
#include "kernel/scheduler.h"
#include "kernel/stack.h"
#include "trap/trap.h"
#include "sys/ksys.h"
#include "clint.h"
#include "csr.h"
#include "int.h"
#include "panic.h"

HeapDescriptor global_heap;

extern void user_entry(void);

void idle(void) {
    for (;;) {
        wait_for_interrupt();
    }
}

HeapDescriptor* get_global_heap(void) {
    return &global_heap;
}

__attribute__((noreturn))
void kernel_main(void) {
    global_heap = init_heap(STACK_SPACE_END, 32768U);

    task_id_t t_idle = spawn_task(idle, PrivMode_M, 0);
    task_id_t t_entry = spawn_task(user_entry, PrivMode_U, 50);
    
    if (t_idle >= 0) {
        uint64_t time = get_time();
        set_timecmp(time + TICK_LENGTH);
        
        sp_to_mscratch();
        start_schedule(t_entry);
    } else {
        k_puts("Panic: failed to create task");
        panic();
    }
}
