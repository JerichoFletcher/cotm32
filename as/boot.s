.section .text
.globl _start

_start:
  la t0, trap_entry
  andi t0, t0, -4
  csrw mtvec, t0
  ecall
  
trap_entry:
  j trap_entry
