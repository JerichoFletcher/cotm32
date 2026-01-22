#include "kernel/scheduler.h"
#include "trap/trap.h"
#include "sys/ksys.h"
#include "clint.h"
#include "csr.h"

#include "user/syscall.h"

__attribute__((noreturn))
extern void mret_to_context(Context* ctx);

__attribute__((noreturn))
void task1(void) {
    for (size_t i = 0; i < 10; i++) {
        puts("Hello from TASK 1\n", 18);
        yield();
    }
    while (TRUE);
}

__attribute__((noreturn))
void task2(void) {
    for (size_t i = 0; i < 10; i++) {
        puts("TASK 2 says hi\n", 15);
        yield();
    }
    while (TRUE);
}

__attribute__((noreturn))
void kernel_stall(void) {
    while (TRUE);
}

__attribute__((noreturn))
void kernel_main(void) {
    set_interrupt(Interrupt_M_TIMER, TRUE);
    set_timecmp(100000);

    if (create_task(0, task1, 0x80001000, 4096) && create_task(0, task2, 0x80002000, 4096)) {
        sp_to_mscratch();
        mret_to_context(&current_task()->ctx);
    } else {
        k_puts("Failed to create task!\n", 23);
        while (TRUE);
    }
}
