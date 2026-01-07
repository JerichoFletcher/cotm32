.globl _start
.extern _estack

.section .text
_start: # Place _start at PC reset vector (0x0)
  la    sp, _estack
  la    t0, trap_entry
  andi  t0, t0, -4
  csrw  mtvec, t0
  j     main

trap_entry:
  # Establish stack and store registers
  addi  sp, sp, -16
  sw    ra, 12(sp)
  sw    t0, 8(sp)
  sw    t1, 4(sp)
  sw    t2, 0(sp)

  # Inspect trap
  csrr  t0, mcause
  csrr  t1, mepc

  # TODO: Implement handler
  addi  t1, t1, 4
  csrw  mepc, t1
  
  # Restore registers and tear down stack
  lw    t2, 0(sp)
  lw    t1, 4(sp)
  lw    t0, 8(sp)
  lw    ra, 12(sp)
  addi  sp, sp, 16
  mret

main:
  ecall

stall:
  j stall
