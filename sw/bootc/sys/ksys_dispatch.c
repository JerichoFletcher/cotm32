#include "sys/ksys_dispatch.h"
#include "sys/ksys.h"
#include "kernel/scheduler.h"

void dispatch_syscall(Context* ctx) {
    switch ((SyscallCode)ctx->regs[17]) {
        case SyscallCode_YIELD: {
            copy_context(&current_task()->ctx, ctx);
            schedule();
            copy_context(ctx, &current_task()->ctx);
            break;
        }
        case SyscallCode_PUTC: k_putc(ctx->regs[10]); break;
        case SyscallCode_GETC: ctx->regs[10] = k_getc(); break;
        case SyscallCode_PUTS: k_puts((char*)ctx->regs[10], ctx->regs[11]); break;
        default: break;
    }
}
