.section .text
.globl _start

_start:
  li x10, 1
  li x11, 10

inc:
  addi x10, x10, 1
  blt x10, x11, inc
  li x11, 0

dec:
  addi x10, x10, -1
  bge x10, x11, dec
