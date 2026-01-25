#include "trap/enums.h"
#include "trap/frame.h"
#include "sys/ksys.h"
#include "sys/ksys_dispatch.h"
#include "kernel/scheduler.h"

#include "clint.h"
#include "panic.h"
#include "int.h"

void trap_handler(TrapFrame* frame) {
    asm volatile("" : : : "memory");
    
    TrapCause cause = frame->mcause;
    if ((int32_t)cause < 0) {
        // Interrupt
        Interrupt interr = (Interrupt)(cause & 0x7fffffff);
        size_t time_slice;
        get_task_time_slice(current_task(), &time_slice);

        switch (interr) {
            case Interrupt_M_TIMER: {
                time_slice--;
                set_task_time_slice(current_task(), time_slice);
                
                uint64_t t = get_timecmp();
                set_timecmp(t + TICK_LENGTH);
                break;
            }
            default: break;
        }

        size_t num_awaken = wake_irq_tasks(interr);
        if (time_slice == 0 || num_awaken > 0) {
            k_yield(&frame->ctx);
        }
    } else {
        // Exception
        switch ((Exception)(cause & 0x7fffffff)) {
            case Exception_INST_ADDR_MISALIGNED:
            case Exception_LOAD_ADDR_MISALIGNED:
            case Exception_STORE_ADDR_MISALIGNED: {
                k_puts("Panic: address misaligned", 25);
                panic();
            }
            case Exception_INST_ACCESS_FAULT:
            case Exception_LOAD_ACCESS_FAULT:
            case Exception_STORE_ACCESS_FAULT: {
                k_puts("Panic: access fault", 19);
                panic();
            }
            case Exception_ECALL_U:
            case Exception_ECALL_M: {
                dispatch_syscall(&frame->ctx);
                break;
            }
            default: panic();
        }
    }
}
