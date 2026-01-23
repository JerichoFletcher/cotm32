#include "user/syscall.h"

__attribute__((noreturn))
void entry(void) {
    puts("# _", 3);
    for (;;) {
        char c = getc();
        if (c == '\n') {
            puts("\b \n# _", 6);
        } else if (c == '\b') {
            puts("\b \b\b_", 5);
        } else {
            putc('\b');
            putc(c);
            putc('_');
        }
    }
}
