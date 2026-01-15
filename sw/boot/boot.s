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

  # Branch off between exceptions and interrupts
  csrr    t0, mcause
  bltz    t0, interr_handle
  j       exc_handle

exc_handle:
  slli    t0, t0, 2
  la      t1, exc_table
  add     t1, t1, t0
  lw      t2, 0(t1)
  jalr    t2
  j       trap_exit

interr_handle:
  slli    t0, t0, 2
  la      t1, interr_table
  add     t1, t1, t0
  lw      t2, 0(t1)
  jalr    t2
  j       trap_exit

trap_exit:
  # Restore registers and tear down stack frame
  lw      t2, 0(sp)
  lw      t1, 4(sp)
  lw      t0, 8(sp)
  lw      ra, 12(sp)
  addi    sp, sp, 16
  mret

########## EXCEPTION HANDLERS ##########
exc_handle_inst_addr_misaligned:
  j       exc_handle_inst_addr_misaligned

exc_handle_inst_access_fault:
  j       exc_handle_inst_access_fault

exc_handle_illegal_inst:
  j       exc_handle_illegal_inst

exc_handle_breakpoint:
  j       exc_handle_breakpoint

exc_handle_load_addr_misaligned:
  j       exc_handle_load_addr_misaligned

exc_handle_load_access_fault:
  j       exc_handle_load_access_fault

exc_handle_store_addr_misaligned:
  j       exc_handle_store_addr_misaligned

exc_handle_store_access_fault:
  j       exc_handle_store_access_fault

exc_handle_ecall_m:
  # TODO: Inspect a7 and handle
  mv      a0, a7

  # Advance MEPC by 4
  csrr    t0, mepc
  addi    t0, t0, 4
  csrw    mepc, t0
  ret

exc_handle_reserved:
  j       exc_handle_reserved

########## INTERRUPT HANDLERS ##########
interr_handle_m_software:
  j       interr_handle_m_software

interr_handle_m_timer:
  j       interr_handle_m_timer

interr_handle_m_external:
  j       interr_handle_m_external

interr_handle_reserved:
  j       interr_handle_reserved

.section .rodata
exc_table:
  .word   exc_handle_inst_addr_misaligned
  .word   exc_handle_inst_access_fault
  .word   exc_handle_illegal_inst
  .word   exc_handle_breakpoint
  .word   exc_handle_load_addr_misaligned
  .word   exc_handle_load_access_fault
  .word   exc_handle_store_addr_misaligned
  .word   exc_handle_store_access_fault
  .word   exc_handle_reserved
  .word   exc_handle_reserved
  .word   exc_handle_reserved
  .word   exc_handle_ecall_m

interr_table:
  .word   interr_handle_reserved
  .word   interr_handle_reserved
  .word   interr_handle_reserved
  .word   interr_handle_m_software
  .word   interr_handle_reserved
  .word   interr_handle_reserved
  .word   interr_handle_reserved
  .word   interr_handle_m_timer
  .word   interr_handle_reserved
  .word   interr_handle_reserved
  .word   interr_handle_reserved
  .word   interr_handle_m_external
