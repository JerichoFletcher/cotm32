#include "sys/ksys_dispatch.h"
#include "sys/ksys.h"
#include "kernel/scheduler.h"

void dispatch_syscall(Context* ctx) {
    switch ((SyscallCode)ctx->regs[17]) {
        case SyscallCode_EXIT: {
            k_exit(ctx);
            break;
        }
        case SyscallCode_YIELD: {
            ctx->pc += 4;
            k_yield(ctx);
            break;
        }
        case SyscallCode_PUTC: {
            k_putc(ctx->regs[10]);
            ctx->pc += 4;
            break;
        }
        case SyscallCode_GETC: {
            ctx->regs[10] = k_getc();
            ctx->pc += 4;
            break;
        }
        case SyscallCode_PUTS: {
            k_puts((char*)ctx->regs[10], ctx->regs[11]);
            ctx->pc += 4;
            break;
        }
        default: break;
    }
}
