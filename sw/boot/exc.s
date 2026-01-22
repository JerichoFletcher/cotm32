.include "macros.inc"
.include "sys.inc"

.globl exc_table

.section .rodata
.align 2
exc_table:
    .word       handle_inst_addr_misaligned
    .word       handle_inst_access_fault
    .word       handle_illegal_inst
    .word       handle_breakpoint
    .word       handle_load_addr_misaligned
    .word       handle_load_access_fault
    .word       handle_store_addr_misaligned
    .word       handle_store_access_fault
    .word       handle_ecall_u
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_ecall_m

str_addr_misaligned:
    .ascii      "Panic: Address misaligned"     # 25
str_access_fault:
    .ascii      "Panic: Access fault"           # 19
str_illegal_inst:
    .ascii      "Panic: Illegal instruction"    # 26

########## EXCEPTION HANDLERS ##########
.section .text.trap
handle_inst_addr_misaligned:
    la          a0, str_addr_misaligned
    li          a1, 25
    call        k_puts
1:  j           1b

handle_inst_access_fault:
    la          a0, str_access_fault
    li          a1, 19
    call        k_puts
1:  j           1b

handle_illegal_inst:
    la          a0, str_illegal_inst
    li          a1, 26
    call        k_puts
1:  j           1b

handle_breakpoint:
    j           handle_breakpoint

handle_load_addr_misaligned:
    la          a0, str_addr_misaligned
    li          a1, 25
    call        k_puts
1:  j           1b

handle_load_access_fault:
    la          a0, str_access_fault
    li          a1, 19
    call        k_puts
1:  j           1b

handle_store_addr_misaligned:
    la          a0, str_addr_misaligned
    li          a1, 25
    call        k_puts
1:  j           1b

handle_store_access_fault:
    la          a0, str_access_fault
    li          a1, 19
    call        k_puts
1:  j           1b

handle_ecall_u:
    PUSH1       ra

    # Use jump table to look up syscall handler
    slli        t0, a7, 2
    T_LOOKUP    t1, t0, sys_table
    jalr        t1

    # Advance PC by 4
    csrr        t0, mepc
    addi        t0, t0, 4
    csrw        mepc, t0

    POP1        ra
    ret

handle_ecall_m:
    PUSH1       ra

    # Use jump table to look up syscall handler
    slli        t0, a7, 2
    T_LOOKUP    t1, t0, sys_table
    jalr        t1

    # Advance PC by 4
    csrr        t0, mepc
    addi        t0, t0, 4
    csrw        mepc, t0

    POP1        ra
    ret

handle_reserved:
    j           handle_reserved
