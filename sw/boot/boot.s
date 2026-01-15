.globl _start
.extern _estack

.equ mtime,       0x0200bff8
.equ mtimecmp,    0x02004000

.section .text.init
_start:
  # Set stack pointer
  la      sp, _estack

  # Set trap vector
  la      t0, trap_entry
  andi    t0, t0, -4
  csrw    mtvec, t0

  # Set mie.MTIE
  li      t0, (1 << 7)
  csrs    mie, t0

  # Set mtimecmp = 10 cycles
  la      t0, mtimecmp
  li      t1, 0
  sw      t1, 4(t0)
  li      t1, 10
  sw      t1, 0(t0)

  # Set mstatus.MIE
  csrsi   mstatus, (1 << 3)

  call    main
1:
  j       1b

.section .text
main:
  la      t2, mtime
  lw      t0, 0(t2)
  lw      t1, 4(t2)
  ret

trap_entry:
  # Establish stack frame and store registers
  addi    sp, sp, -16
  sw      ra, 12(sp)
  sw      t0, 8(sp)
  sw      t1, 4(sp)
  sw      t2, 0(sp)

  # Look up the handler in the trap jump table
  csrr    t0, mcause
  slli    t0, t0, 2
  la      t1, trap_table
  add     t1, t1, t0
  lw      t2, 0(t1)
  jalr    t2
  
  # Restore registers and tear down stack frame
  lw      t2, 0(sp)
  lw      t1, 4(sp)
  lw      t0, 8(sp)
  lw      ra, 12(sp)
  addi    sp, sp, 16
  mret

trap_handle_inst_addr_misaligned:
  j       trap_handle_inst_addr_misaligned

trap_handle_inst_access_fault:
  j       trap_handle_inst_access_fault

trap_handle_illegal_inst:
  j       trap_handle_illegal_inst

trap_handle_breakpoint:
  j       trap_handle_breakpoint

trap_handle_load_addr_misaligned:
  j       trap_handle_load_addr_misaligned

trap_handle_load_access_fault:
  j       trap_handle_load_access_fault

trap_handle_store_addr_misaligned:
  j       trap_handle_store_addr_misaligned

trap_handle_store_access_fault:
  j       trap_handle_store_access_fault

trap_handle_ecall_m:
  # TODO: Inspect a7 and handle
  mv      a0, a7

  # Advance MEPC by 4
  csrr    t0, mepc
  addi    t0, t0, 4
  csrw    mepc, t0
  ret

trap_handle_reserved:
  j       trap_handle_reserved

.section .rodata
trap_table:
  .word   trap_handle_inst_addr_misaligned
  .word   trap_handle_inst_access_fault
  .word   trap_handle_illegal_inst
  .word   trap_handle_breakpoint
  .word   trap_handle_load_addr_misaligned
  .word   trap_handle_load_access_fault
  .word   trap_handle_store_addr_misaligned
  .word   trap_handle_store_access_fault
  .word   trap_handle_reserved
  .word   trap_handle_reserved
  .word   trap_handle_reserved
  .word   trap_handle_ecall_m
