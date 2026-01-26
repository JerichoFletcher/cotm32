#include "user/syscall.h"

#define BUF_MAX 64U

__attribute__((noreturn))
void user_entry(void) {
    char* buf = (char*)malloc(sizeof(char) * BUF_MAX);
    size_t n_buf = 0;

    if (!buf) {
        puts("Failed to allocate buffer!");
        exit();
    }

    putc('_'); putc('\b');
    for (;;) {
        char c = getc();
        if (c == '\n') {
            puts(" ");
            buf[n_buf] = '\0';
            break;
        } else if (c == '\b') {
            putc(' '); putc('\b'); putc('\b'); putc('_'); putc('\b');
            n_buf--;
        } else if (n_buf < BUF_MAX - 1) {
            putc(c); putc('_'); putc('\b');
            buf[n_buf++] = c;
        }
    }
    puts(buf);

    free(buf);
    exit();
}
