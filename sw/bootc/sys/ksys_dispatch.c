#include "sys/ksys_dispatch.h"
#include "sys/ksys.h"
#include "bool.h"

void dispatch_syscall(Context* ctx) {
    SyscallStatus status;
    bool_t adv_pc = TRUE;

    switch ((SyscallCode)ctx->regs[17]) {
        case SyscallCode_EXIT: {
            status = k_exit(ctx);
            adv_pc = FALSE;
            break;
        }
        case SyscallCode_YIELD: {
            ctx->pc += 4;
            status = k_yield(ctx);
            adv_pc = FALSE;
            break;
        }
        case SyscallCode_FREE: {
            status = k_free((void*)ctx->regs[10]);
            break;
        }
        case SyscallCode_MALLOC: {
            status = k_malloc(ctx->regs[10], (void*)&ctx->regs[10]);
            break;
        }
        case SyscallCode_PUTC: {
            status = k_putc(ctx->regs[10]);
            break;
        }
        case SyscallCode_GETC: {
            status = k_getc((char*)&ctx->regs[10]);
            break;
        }
        case SyscallCode_PUTS: {
            status = k_puts((char*)ctx->regs[10]);
            break;
        }
        default: status = SyscallStatus_DONE; break;
    }

    if (adv_pc && status == SyscallStatus_DONE) {
        ctx->pc += 4;
    } else if (status == SyscallStatus_BLOCKED) {
        k_yield(ctx);
    } // On RETRY, do not advance PC and do not yield execution
}
