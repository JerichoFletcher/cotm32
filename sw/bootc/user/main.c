#include "user/syscall.h"
#include "bool.h"

int user_main(void) {
    puts("Hello, World!", 13);
    while (TRUE);
}
