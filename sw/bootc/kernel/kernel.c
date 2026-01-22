#include "kernel/scheduler.h"
#include "trap/trap.h"
#include "sys/ksys.h"
#include "clint.h"
#include "csr.h"
#include "context.h"

#include "user/syscall.h"

__attribute__((noreturn))
void task1(void) {
    while (TRUE) {
        puts("TASK 1 says hello\n", 18);
        yield();
    }
}

__attribute__((noreturn))
void task2(void) {
    while (TRUE) {
        puts("TASK 2 says hi\n", 15);
        yield();
    }
}

__attribute__((noreturn))
void kernel_main(void) {
    set_interrupt(Interrupt_M_TIMER, TRUE);

    uint64_t t = get_time();
    set_timecmp(t + 10000);

    if (create_task(0, task1, 0x80001000, 4096) && create_task(0, task2, 0x80002000, 4096)) {
        sp_to_mscratch();
        mret_to_context(&current_task()->ctx);
    } else {
        k_puts("Failed to create task!\n", 23);
        while (TRUE);
    }
}
