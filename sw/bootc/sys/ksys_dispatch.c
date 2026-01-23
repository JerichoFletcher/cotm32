#include "sys/ksys_dispatch.h"
#include "sys/ksys.h"

void dispatch_syscall(Context* ctx) {
    SyscallStatus status;
    switch ((SyscallCode)ctx->regs[17]) {
        case SyscallCode_EXIT: {
            status = k_exit(ctx);
            break;
        }
        case SyscallCode_YIELD: {
            ctx->pc += 4;
            status = k_yield(ctx);
            break;
        }
        case SyscallCode_PUTC: {
            status = k_putc(ctx->regs[10]);
            if (status == SyscallStatus_DONE) {
                ctx->pc += 4;
            }
            break;
        }
        case SyscallCode_GETC: {
            char c;
            status = k_getc(ctx, &c);
            if (status == SyscallStatus_DONE) {
                ctx->regs[10] = c;
                ctx->pc += 4;
            }
            break;
        }
        case SyscallCode_PUTS: {
            status = k_puts((char*)ctx->regs[10], ctx->regs[11]);
            if (status == SyscallStatus_DONE) {
                ctx->pc += 4;
            }
            break;
        }
        default: status = SyscallStatus_DONE; break;
    }
}
