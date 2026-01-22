#include "user/syscall.h"
#include "bool.h"

__attribute__((noreturn))
void task1(void) {
    while (TRUE) {
        puts("TASK 1 says hello\n", 18);
        yield();
    }
}

__attribute__((noreturn))
void task2(void) {
    while (TRUE) {
        puts("TASK 2 says hi\n", 15);
        yield();
    }
}