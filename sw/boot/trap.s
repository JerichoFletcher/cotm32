.include "macros.inc"

.globl trap_entry

.section .text
trap_entry:
    # Establish stack frame and store registers
    PUSH4       ra, t0, t1, t2

    # Branch off between exceptions and interrupts
    csrr        t0, mcause
    bltz        t0, interr_handle
    j           exc_handle

exc_handle:
    slli        t0, t0, 2
    T_LOOKUP    t1, t0, exc_table
    jalr        t1
    j           trap_exit

interr_handle:
    slli        t0, t0, 2
    T_LOOKUP    t1, t0, interr_table
    jalr        t1
    j           trap_exit

trap_exit:
    # Restore registers and tear down stack frame
    POP4        t2, t1, t0, ra
    mret
