.include "macros.inc"

.equ UART_term, 0x10000000

.section .text
# k_putc -- Writes a character to the terminal
# Params:
#   a0 -- A character
.globl k_putc
k_putc:
    li          t0, UART_term

1:  # Wait until UART TX is ready (status[0] = 0)
    lb          t1, 8(t0)
    andi        t1, t1, 0x1
    bnez        t1, 1b
    sb          a0, 0(t0)

    ret

# k_getc -- Reads a character from the terminal
# Returns:
#   a0 -- A character
.globl k_getc
k_getc:
    li          t0, UART_term

1:  # Block until UART RX has data (status[1] = 1)
    lb          t1, 8(t0)
    andi        t1, t1, 0x2
    beqz        t1, 1b
    lb          a0, 4(t0)

    ret

# k_puts -- Writes a string to the terminal
# Params:
#   a0 -- A pointer to the beginning of the string
#   a1 -- The length of the string
.globl k_puts
k_puts:
    PUSH4       ra, s0, s1, s2
    mv          s0, a0
    li          s1, 0
    mv          s2, a1

1:  # Write the next character
    beq         s1, s2, 2f
    add         t0, s0, s1
    lb          a0, 0(t0)
    call        k_putc
    addi        s1, s1, 1
    j           1b

2:  POP4        s2, s1, s0, ra
    ret
