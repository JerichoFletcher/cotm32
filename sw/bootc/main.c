#include "priv/enums.h"
#include "trap/trap.h"
#include "csr.h"
#include "bool.h"

extern void trap_entry(void);
extern void kernel_main(void);

void main(void) {
    set_trap_entry(trap_entry);
    kernel_main();
}
