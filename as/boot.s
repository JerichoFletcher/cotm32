.globl _start
.extern _estack

.section .text
_start: # _start at PC reset vector (0x0)
  la    sp, _estack
  la    t0, trap_entry
  andi  t0, t0, -4
  csrw  mtvec, t0
  j     main

main:
  # Test ecall (should see 0xdeadbeef in a0)
  li    a7, 0xdeadbeef
  ecall
  j     stall

stall:
  mv    zero, a0
  j     stall

trap_entry:
  # Establish stack and store registers
  addi  sp, sp, -16
  sw    ra, 12(sp)
  sw    t0, 8(sp)
  sw    t1, 4(sp)
  sw    t2, 0(sp)

  # Look up the handler in the trap jump table
  csrr  t0, mcause
  slli  t0, t0, 2
  la    t1, trap_table
  add   t1, t1, t0
  lw    t2, 0(t1)
  jr    t2
  
trap_ret:
  # Restore registers and tear down stack
  lw    t2, 0(sp)
  lw    t1, 4(sp)
  lw    t0, 8(sp)
  lw    ra, 12(sp)
  addi  sp, sp, 16
  mret

trap_handle_inst_addr_misaligned:
  j trap_handle_inst_addr_misaligned

trap_handle_illegal_inst:
  j trap_handle_illegal_inst

trap_handle_breakpoint:
  j trap_handle_breakpoint

trap_handle_load_addr_misaligned:
  j trap_handle_load_addr_misaligned

trap_handle_store_addr_misaligned:
  j trap_handle_store_addr_misaligned

trap_handle_ecall_m:
  # TODO: Inspect a7 and handle
  mv    a0, a7

  # Advance MEPC by 4
  csrr  t0, mepc
  addi  t0, t0, 4
  csrw  mepc, t0
  j trap_ret

trap_handle_reserved:
  j trap_handle_reserved

.section .rodata
trap_table:
  .word trap_handle_inst_addr_misaligned
  .word trap_handle_reserved
  .word trap_handle_illegal_inst
  .word trap_handle_breakpoint
  .word trap_handle_load_addr_misaligned
  .word trap_handle_reserved
  .word trap_handle_store_addr_misaligned
  .word trap_handle_reserved
  .word trap_handle_reserved
  .word trap_handle_reserved
  .word trap_handle_reserved
  .word trap_handle_ecall_m
