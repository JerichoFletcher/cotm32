#include "trap/enums.h"
#include "trap/frame.h"
#include "sys/ksys.h"
#include "sys/ksys_dispatch.h"

#include "clint.h"
#include "bool.h"
#include "int.h"

void trap_halt() {
    while (TRUE);
}

void trap_handler(TrapFrame* frame) {
    TrapCause cause = frame->mcause;
    if ((int32_t)cause < 0) {
        // Interrupt
        switch (cause) {
            case TrapCause_INTERR_M_TIMER: {
                uint64_t t = get_timecmp();
                set_timecmp(t + 100000);
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
                trap_halt();
                break;
            }
            case TrapCause_INST_ACCESS_FAULT:
            case TrapCause_LOAD_ACCESS_FAULT:
            case TrapCause_STORE_ACCESS_FAULT: {
                k_puts("Panic: access fault", 19);
                trap_halt();
                break;
            }
            case TrapCause_ECALL_U:
            case TrapCause_ECALL_M: {
                dispatch_syscall(frame);
                frame->mepc += 4;
                break;
            }
            default: trap_halt();
        }
    }
}
