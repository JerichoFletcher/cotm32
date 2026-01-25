#include "trap/trap.h"
#include "lib/cstr.h"
#include "int.h"

extern uint8_t __bss_start;
extern uint8_t __bss_end;

extern void trap_entry(void);
extern void kernel_main(void);

void entry(void) {
    void* bss_start = &__bss_start;
    size_t bss_size = (size_t)&__bss_end - (size_t)&__bss_start;
    memset(bss_start, 0, bss_size);

    set_trap_entry(trap_entry);
    kernel_main();
}
