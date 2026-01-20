.equ PUTC, 1
.equ GETC, 2

.macro SYS_putc reg = a0
.ifnc           \reg, a0
    mv          a0, \reg
.endif
    li          a7, PUTC
    ecall
.endm

.macro SYS_getc
    li          a7, GETC
    ecall
.endm
