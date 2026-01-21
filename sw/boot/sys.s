.include "macros.inc"

.globl sys_table

.section .rodata
.align 2
sys_table:
    .word       sys_dummy
    .word       sys_putc
    .word       sys_getc
    .word       sys_puts

########## SYSCALL HANDLERS ##########
.section .text
# Dummy handler for syscall code 0
sys_dummy:
    ret

# sys_putc -- Writes a character to the terminal
# Params:
#   a0 -- A character
sys_putc:
    PUSH1       ra
    call        k_putc
    POP1        ra
    ret

# sys_getc -- Reads a character from the terminal
# Returns:
#   a0 -- A character
sys_getc:
    PUSH1       ra
    call        k_getc
    POP1        ra
    ret

# sys_puts -- Writes a string to the terminal
# Params:
#   a0 -- A pointer to the beginning of the string
#   a1 -- The length of the string
sys_puts:
    PUSH1       ra
    call        k_puts
    POP1        ra
    ret
