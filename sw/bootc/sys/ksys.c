#include "sys/ksys.h"
#include "kernel/scheduler.h"
#include "bool.h"

#define UART_TERM_TXDATA (volatile char*)0x10000000
#define UART_TERM_RXDATA (volatile char*)0x10000004
#define UART_TERM_STATUS (volatile uint8_t*)0x10000008

void k_exit(Context* ctx) {
    terminate_task(current_task());
    schedule();
    copy_context(ctx, &current_task()->ctx);
}

void k_yield(Context* ctx) {
    copy_context(&current_task()->ctx, ctx);
    schedule();
    copy_context(ctx, &current_task()->ctx);
}

void k_putc(char c) {
    while (*UART_TERM_STATUS & 1);
    *UART_TERM_TXDATA = c;
}

char k_getc(void) {
    while (!(*UART_TERM_STATUS & 2));
    return *UART_TERM_RXDATA;
}

void k_puts(const char* s, size_t len) {
    for (size_t i = 0; i < len; i++) {
        k_putc(s[i]);
    }
}
