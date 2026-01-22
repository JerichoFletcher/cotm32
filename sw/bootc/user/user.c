#include "user/syscall.h"
#include "bool.h"

__attribute__((noreturn))
void task1(void) {
    for (size_t i = 0; i < 10; i++) {
        puts("TASK 1 says hello: ", 19);
        putc('0' + i);
        putc('\n');
    }
    puts("TASK 1 says goodbye!\n", 21);
    exit();
}

__attribute__((noreturn))
void task2(void) {
    for (size_t i = 0; i < 5; i++) {
        puts("TASK 2 says hello: ", 19);
        putc('0' + i);
        putc('\n');
    }
    puts("TASK 2 says goodbye!\n", 21);
    exit();
}
