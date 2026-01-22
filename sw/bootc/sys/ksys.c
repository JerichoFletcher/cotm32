#include "sys/ksys.h"
#include "bool.h"

#define UART_TERM_TXDATA (volatile char*)0x10000000
#define UART_TERM_RXDATA (volatile char*)0x10000004
#define UART_TERM_STATUS (volatile uint8_t*)0x10000008

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
