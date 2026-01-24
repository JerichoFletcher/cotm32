.global _start
.extern _estack
.extern __bss_start
.extern __bss_end

.section .text.init
_start:
    la      sp, _estack
    la      t0, __bss_start
    la      t1, __bss_end
1:  beq     t0, t1, 2f
    sw      zero, 0(t0)
    addi    t0, t0, 4
    j       1b
2:  call    entry
    j       .
