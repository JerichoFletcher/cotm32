.globl sys_table

.equ UART_term, 0x10000000

.section .rodata
sys_table:
    .word       0
    .word       putc
    .word       getc

########## SYSCALL HANDLERS ##########
.section .text
# putc -- Writes a character to the terminal
# Params:
#   a0 -- A character
putc:
    li          t0, UART_term

1:  # Wait until UART TX is ready (status[0] = 0)
    lb          t1, 8(t0)
    andi        t1, t1, 0x1
    bnez        t1, 1b

    sb          a0, 0(t0)
    ret

# getc -- Reads a character from the terminal
# Returns:
#   a0 -- A character
getc:
    li          t0, UART_term

1:  # Block until UART RX has data (status[1] = 1)
    lb          t1, 8(t0)
    andi        t1, t1, 0x2
    beqz        t1, 1b

    lb          a0, 4(t0)
    ret
