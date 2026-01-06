.section .text
.globl _start

_start:
  la t0, trap_entry
  addi zero, t0, 0
  ecall
  
trap_entry:
  j trap_entry
