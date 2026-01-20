.include "macros.inc"

.globl exc_table

.section .rodata
exc_table:
    .word       handle_inst_addr_misaligned
    .word       handle_inst_access_fault
    .word       handle_illegal_inst
    .word       handle_breakpoint
    .word       handle_load_addr_misaligned
    .word       handle_load_access_fault
    .word       handle_store_addr_misaligned
    .word       handle_store_access_fault
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_reserved
    .word       handle_ecall_m

########## EXCEPTION HANDLERS ##########
.section .text.trap
handle_inst_addr_misaligned:
    j           handle_inst_addr_misaligned

handle_inst_access_fault:
    j           handle_inst_access_fault

handle_illegal_inst:
    j           handle_illegal_inst

handle_breakpoint:
    j           handle_breakpoint

handle_load_addr_misaligned:
    j           handle_load_addr_misaligned

handle_load_access_fault:
    j           handle_load_access_fault

handle_store_addr_misaligned:
    j           handle_store_addr_misaligned

handle_store_access_fault:
    j           handle_store_access_fault

handle_ecall_m:
    PUSH1       ra

    # Use jump table to look up syscall handler
    slli        t0, a7, 2
    T_LOOKUP    t1, t0, sys_table
    jalr        t1

    POP1        ra
    ret

handle_reserved:
    j           handle_reserved
