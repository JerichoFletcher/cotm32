#include "trap/enums.h"
#include "trap/frame.h"
#include "sys/ksys.h"
#include "sys/ksys_dispatch.h"
#include "kernel/scheduler.h"

#include "clint.h"
#include "int.h"

__attribute__((noreturn))
void panic() {
    for (;;);
}

void trap_handler(TrapFrame* frame) {
    asm volatile("" : : : "memory");
    
    TrapCause cause = frame->mcause;
    if ((int32_t)cause < 0) {
        // Interrupt
        switch (cause) {
            case TrapCause_INTERR_M_TIMER: {
                k_yield(&frame->ctx);
                
                uint64_t t = get_time();
                set_timecmp(t + 10000);
                break;
            }
            default: break;
        }
    } else {
        // Exception
        switch (cause) {
            case TrapCause_INST_ADDR_MISALIGNED:
            case TrapCause_LOAD_ADDR_MISALIGNED:
            case TrapCause_STORE_ADDR_MISALIGNED: {
                k_puts("Panic: address misaligned", 25);
                panic();
            }
            case TrapCause_INST_ACCESS_FAULT:
            case TrapCause_LOAD_ACCESS_FAULT:
            case TrapCause_STORE_ACCESS_FAULT: {
                k_puts("Panic: access fault", 19);
                panic();
            }
            case TrapCause_ECALL_U:
            case TrapCause_ECALL_M: {
                dispatch_syscall(&frame->ctx);
                break;
            }
            default: panic();
        }
    }
}
