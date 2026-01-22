.include "macros.inc"
.include "mmr.inc"

.globl interr_table

.section .rodata
.align 2
interr_table:
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_m_software
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_m_timer
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_m_external

########## INTERRUPT HANDLERS ##########
.section .text.trap
handle_m_software:
    j           handle_m_software

handle_m_timer:
    li          t0, mtimecmp
    sw          zero, 4(t0)

    lw          t1, 0(t0)
    li          t2, 100000
    add         t1, t1, t2
    sw          t1, 0(t0)

    ret

handle_m_external:
    j           handle_m_external

handle_reserved:
    j           handle_reserved
