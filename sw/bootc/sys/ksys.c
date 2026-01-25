#include "sys/ksys.h"
#include "kernel/scheduler.h"
#include "kernel/heap.h"
#include "trap/trap.h"
#include "bool.h"

#define MAX_SPIN_ITER 1024

#define UART_TERM_TXDATA (volatile char*)0x10000000
#define UART_TERM_RXDATA (volatile char*)0x10000004
#define UART_TERM_STATUS (volatile uint8_t*)0x10000008

SyscallStatus k_exit(Context* ctx) {
    terminate_task(current_task());
    schedule();
    copy_context(ctx, &current_task()->ctx);
    return SyscallStatus_DONE;
}

SyscallStatus k_yield(Context* ctx) {
    copy_context(&current_task()->ctx, ctx);
    schedule();
    copy_context(ctx, &current_task()->ctx);
    return SyscallStatus_DONE;
}

SyscallStatus k_free(void* ptr) {
    if (is_valid_heap_ptr(ptr)) free_heap(ptr);
    return SyscallStatus_DONE;
}

SyscallStatus k_malloc(size_t size, void** out_ptr) {
    *out_ptr = allocate_heap(size);
    return SyscallStatus_DONE;
}

SyscallStatus k_putc(char c) {
    for (size_t i = 0; i <= MAX_SPIN_ITER; i++) {
        if (i == MAX_SPIN_ITER) return SyscallStatus_RETRY;
        if (!(*UART_TERM_STATUS & 1)) break;
    }
    *UART_TERM_TXDATA = c;
    return SyscallStatus_DONE;
}

SyscallStatus k_getc(char* out_c) {
    if (!(*UART_TERM_STATUS & 2)) {
        block_task_irq(current_task(), Interrupt_M_EXTERNAL);
        return SyscallStatus_BLOCKED;
    }

    *out_c = *UART_TERM_RXDATA;
    return SyscallStatus_DONE;
}

SyscallStatus k_puts(const char* s, size_t len) {
    for (size_t i = 0; i < len; i++) {
        k_putc(s[i]);
    }
    return SyscallStatus_DONE;
}
