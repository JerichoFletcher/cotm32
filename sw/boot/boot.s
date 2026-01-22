.include "sys.inc"
.include "mmr.inc"

.globl _start
.extern _estack

.equ PRIV_U, 0b00

.section .text.init
_start:
    # Set stack pointer
    la          sp, _estack

    # Set trap vector
    la          t0, trap_entry
    andi        t0, t0, ~3
    csrw        mtvec, t0

    # Enable timer interrupt (mie.MTIE)
    li          t0, (1 << 7)
    csrs        mie, t0

    # Set initial mtimecmp
    li          t0, mtimecmp
    sw          zero, 4(t0)
    li          t1, 100000
    sw          t1, 0(t0)

    ############### PRIV-MODE TRANSITION ###############
    # Load user code entrypoint to mepc
    la          t0, main
    csrw        mepc, t0

    # Set mstatus.MPIE (mstatus.MIE = 1 after transition)
    li          t0, (1 << 7)
    csrs        mstatus, t0
    
    # Set mstatus.MPP (priv = U after transition)
    li          t0, (3 << 11)
    csrc        mstatus, t0
    li          t0, (PRIV_U << 11)
    csrs        mstatus, t0

    # Transition to U-mode
    mret

.section .text
main:
    la          a0, str_welcome
    li          a1, 14
    SYS_puts
1:  j           1b

.section .rodata
str_welcome:
    .ascii      "Hello, World!\n"   # 14
