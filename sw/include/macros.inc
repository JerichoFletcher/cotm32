.macro PUSH1 reg0
    addi        sp, sp, -16
.ifnc \reg0, zero
    sw          \reg0, 12(sp)
.endif
.endm

.macro PUSH2 reg0, reg1
    addi        sp, sp, -16
.ifnc \reg0, zero
    sw          \reg0, 12(sp)
.endif
.ifnc \reg1, zero
    sw          \reg1, 8(sp)
.endif
.endm

.macro PUSH3 reg0, reg1, reg2
    addi        sp, sp, -16
.ifnc \reg0, zero
    sw          \reg0, 12(sp)
.endif
.ifnc \reg1, zero
    sw          \reg1, 8(sp)
.endif
.ifnc \reg2, zero
    sw          \reg2, 4(sp)
.endif
.endm

.macro PUSH4 reg0, reg1, reg2, reg3
    addi        sp, sp, -16
.ifnc \reg0, zero
    sw          \reg0, 12(sp)
.endif
.ifnc \reg1, zero
    sw          \reg1, 8(sp)
.endif
.ifnc \reg2, zero
    sw          \reg2, 4(sp)
.endif
.ifnc \reg3, zero
    sw          \reg3, 0(sp)
.endif
.endm

.macro POP1 reg0
.ifnc \reg0, zero
    lw          \reg0, 12(sp)
.endif
    addi        sp, sp, 16
.endm

.macro POP2 reg0, reg1
.ifnc \reg0, zero
    lw          \re0, 8(sp)
.endif
.ifnc \reg1, zero
    lw          \reg1, 12(sp)
.endif
    addi        sp, sp, 16
.endm

.macro POP3 reg0, reg1, reg2
.ifnc \reg0, zero
    lw          \re0, 4(sp)
.endif
.ifnc \reg1, zero
    lw          \reg1, 8(sp)
.endif
.ifnc \reg2, zero
    lw          \reg2, 12(sp)
.endif
    addi        sp, sp, 16
.endm

.macro POP4 reg0, reg1, reg2, reg3
.ifnc \reg0, zero
    lw          \reg0, 0(sp)
.endif
.ifnc \reg1, zero
    lw          \reg1, 4(sp)
.endif
.ifnc \reg2, zero
    lw          \reg2, 8(sp)
.endif
.ifnc \reg3, zero
    lw          \reg3, 12(sp)
.endif
    addi        sp, sp, 16
.endm

.macro T_LOOKUP dest, offset, table_addr
    la          \dest, \table_addr
    add         \dest, \dest, \offset
    lw          \dest, 0(\dest)
.endm
