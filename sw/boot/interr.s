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
    j           handle_m_timer

handle_m_external:
    j           handle_m_external

handle_reserved:
    j           handle_reserved
