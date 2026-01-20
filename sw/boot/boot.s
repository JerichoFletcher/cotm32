.include "macros.inc"
.include "sys.inc"

.globl _start
.extern _estack

.equ mtime,     0x0200bff8
.equ mtimecmp,  0x02004000

.section .text.init
_start:
    # Set stack pointer
    la          sp, _estack

    # Set trap vector
    la          t0, trap_entry
    andi        t0, t0, -4
    csrw        mtvec, t0

    # Set mie.MTIE
    li          t0, (1 << 7)
    csrs        mie, t0

    # Set mtimecmp
    li          t0, mtimecmp
    li          t1, 0x0000ffff
    sw          t1, 4(t0)
    li          t1, 0xffffffff
    sw          t1, 0(t0)

    # Set mstatus.MIE
    csrsi       mstatus, (1 << 3)

    call        main
1:
    j           1b

.section .text
main:
    SYS_getc
    SYS_putc
    j           main

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
    csrr        t0, mepc
    addi        t0, t0, 4
    csrw        mepc, t0

    # Restore registers and tear down stack frame
    POP4        t2, t1, t0, ra
    mret
