package cotm32_priv_pkg;

parameter MXLEN = 32;

parameter ZICSR_CSR_ADDR_WIDTH = 12;

// Trap cause
typedef enum logic [MXLEN-1:0] {
  TRAP_CAUSE_INST_ADDR_MISALIGNED = 'd0,
  TRAP_CAUSE_ILLEGAL_INST = 'd2,
  TRAP_CAUSE_BREAKPOINT = 'd3,
  TRAP_CAUSE_LOAD_ADDR_MISALIGNED = 'd4,
  TRAP_CAUSE_STORE_ADDR_MISALIGNED = 'd6,
  TRAP_CAUSE_ECALL_M = 'd11
} trap_cause_t;

// Zicsr CSR address
typedef enum logic [ZICSR_CSR_ADDR_WIDTH-1:0] {
  ZICSR_CSR_MTVEC   = 12'h305,
  ZICSR_CSR_MEPC    = 12'h341,
  ZICSR_CSR_MCAUSE  = 12'h342,
  ZICSR_CSR_MTVAL   = 12'h343
} zicsr_csr_addr_t;

// Zicsr CSR operation
typedef enum logic [$clog2(4)-1:0] {
  ZICSR_CSR_OP_NONE,
  ZICSR_CSR_OP_RW,
  ZICSR_CSR_OP_RS,
  ZICSR_CSR_OP_RC
} zicsr_csr_op_t;

// Zicsr CSR data mux
parameter ZICSR_DATA_VALCOUNT = 2;
typedef enum logic [$clog2(ZICSR_DATA_VALCOUNT)-1:0] {
  ZICSR_DATA_RS1,
  ZICSR_DATA_IMM
} zicsr_data_sel_t;

// Zicsr instruction f3
typedef enum logic [2:0] {
  ZICSR_F3_CSRRW  = 3'b001,
  ZICSR_F3_CSRRS  = 3'b010,
  ZICSR_F3_CSRRC  = 3'b011,
  ZICSR_F3_CSRRWI = 3'b101,
  ZICSR_F3_CSRRSI = 3'b110,
  ZICSR_F3_CSRRCI = 3'b111
} zicsr_f3_t;

endpackage : cotm32_priv_pkg