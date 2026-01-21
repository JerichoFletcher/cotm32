.equ PUTC, 1
.equ GETC, 2
.equ PUTS, 3

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

.macro SYS_puts reg0 = a0, reg1 = a1
.ifnc           \reg0, a0
    mv          a0, \reg0
.endif
.ifnc           \reg1, a1
    mv          a1, \reg1
.endif
    li          a7, PUTS
    ecall
.endm
