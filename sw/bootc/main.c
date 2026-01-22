#include "bool.h"
#include "priv/enums.h"
#include "priv/priv.h"
#include "trap/trap.h"
#include "clint.h"

extern void trap_entry(void);
extern void user_main(void);

__attribute__((noreturn))
void main(void) {
    set_trap_entry(trap_entry);
    set_interrupt(Interrupt_M_TIMER, TRUE);
    set_timecmp(100000);

    enter(user_main, PrivMode_U, TRUE);
}
