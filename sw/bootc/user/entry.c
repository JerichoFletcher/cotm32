#include "user/syscall.h"

static inline void newline(void) {
    puts(" \n# _\b", 6);
}

static inline void backspace(void) {
    puts(" \b\b_\b", 5);
}

__attribute__((noreturn))
void user_entry(void) {
    puts("# _\b", 4);
    for (;;) {
        char c = getc();
        if (c == '\n') {
            newline();
        } else if (c == '\b') {
            backspace();
        } else {
            putc(c);
            puts("_\b", 2);
        }
    }
}
