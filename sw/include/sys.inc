.macro SYS_putc reg = a0
.ifnc           \reg, a0
    mv          a0, \reg
.endif
    li          a7, 1
    ecall
.endm

.macro SYS_getc
    li          a7, 2
    ecall
.endm
