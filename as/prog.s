.section .text
.globl _start

_start:
  addi s2, zero, 144
  addi s3, s2, 0

a:
  addi s3, s3, -72
  sw s3, -4(sp)
  beq s2, s3, a

b:
  lui s4, 0xabc
  j b
