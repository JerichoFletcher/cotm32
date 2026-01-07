.section .text
.globl _start

_start:
  j init

trap_entry:
  csrr t0, mcause
  csrr t1, mtval
  csrr t2, mepc

trap_stall:
  j trap_stall

init:
  la t0, trap_entry
  andi t0, t0, -4
  csrw mtvec, t0
  j main

main:
  ecall
