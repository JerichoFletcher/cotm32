.section .text
.globl _start

_start:
  li s1, 1
  li s2, 10
  li s3, 0

inc:
  sw s1, 0(s3)
  addi s3, s3, 4
  addi s1, s1, 1
  blt s1, s2, inc
  
  li s1, 1
  li s3, 0

dec:
  lw s2, 0(s3)
  addi s3, s3, 4
  bne s1, s2, end
  addi s1, s1, 1
  j dec

end:
  ecall
