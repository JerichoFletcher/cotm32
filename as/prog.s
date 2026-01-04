.section .text
.globl _start

_start:
  addi s2, zero, 1
  addi s3, zero, 2
  add s1, s2, s3

loop:
  j loop
