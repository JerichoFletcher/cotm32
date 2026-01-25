#include "user/syscall.h"

#define BUF_MAX 64U

static inline void newline(void) {
    puts(" \n# _\b", 6);
}

static inline void backspace(void) {
    puts(" \b\b_\b", 5);
}

__attribute__((noreturn))
void user_entry(void) {
    char* buf = malloc(sizeof(char) * BUF_MAX);
    size_t n_buf = 0;

    if (!buf) {
        puts("Failed to allocate buffer!\n", 27);
        exit();
    }

    puts("# _\b", 4);
    for (;;) {
        char c = getc();
        if (c == '\n') {
            puts(" \n", 2);
            break;
        } else if (c == '\b') {
            backspace();
            n_buf--;
        } else if (n_buf < BUF_MAX) {
            putc(c);
            puts("_\b", 2);
            buf[n_buf++] = c;
        }
    }
    puts(buf, n_buf);

    free(buf);
    exit();
}
