#include "sys/ksys_dispatch.h"
#include "sys/ksys.h"

void dispatch_syscall(TrapFrame* frame) {
    switch ((SyscallCode)frame->regs[17]) {
        case SyscallCode_PUTC: k_putc(frame->regs[10]); break;
        case SyscallCode_GETC: frame->regs[10] = k_getc(); break;
        case SyscallCode_PUTS: k_puts((char*)frame->regs[10], frame->regs[11]); break;
        default: break;
    }
}
