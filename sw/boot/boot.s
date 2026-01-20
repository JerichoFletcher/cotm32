.include "sys.inc"
.include "mmr.inc"

.globl _start
.extern _estack

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
    li          t1, 0xffffffff
    sw          t1, 4(t0)
    li          t1, 0xffffffff
    sw          t1, 0(t0)

    # Set mstatus.MIE
    csrsi       mstatus, (1 << 3)

    j           main

.section .text
main:
    la          t0, str_welcome
1:  lb          t1, 0(t0)
    beqz        t1, 2f
    SYS_putc    t1
    addi        t0, t0, 1
    j           1b
2:  SYS_getc
    SYS_putc
    j           2b

.section .rodata
str_welcome:
    .asciz      "Hello, World!\n"
