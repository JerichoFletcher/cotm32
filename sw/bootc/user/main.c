#include "user/syscall.h"
#include "bool.h"

__attribute__((noreturn))
int user_main(void) {
    puts("Hello, World!", 13);
    while (TRUE) {
        yield();
    }
}
