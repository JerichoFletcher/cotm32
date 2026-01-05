package cotm32_priv_pkg;

parameter MXLEN = 32;

// Trap cause
typedef enum logic [MXLEN-1:0] {
  TRAP_CAUSE_INST_ADDR_MISALIGNED = 'd0,
  TRAP_CAUSE_ILLEGAL_INST = 'd2,
  TRAP_CAUSE_BREAKPOINT = 'd3,
  TRAP_CAUSE_LOAD_ADDR_MISALIGNED = 'd4,
  TRAP_CAUSE_STORE_ADDR_MISALIGNED = 'd6,
  TRAP_CAUSE_ECALL_M = 'd11
} trap_cause_t;

endpackage : cotm32_priv_pkg