`timescale 1ns/1ns

package cotm32_priv_pkg;

import cotm32_pkg::*;

parameter MXLEN = 32;

parameter ZICSR_CSR_ADDR_WIDTH = 12;

typedef enum logic [1:0] {
  PRIV_U  = 2'b00,
  PRIV_M  = 2'b11
} priv_mode_t;

//////////////////////////////// CSR    ////////////////////////////////
// Trap cause
typedef enum logic [MXLEN-1:0] {
  TRAP_CAUSE_INST_ADDR_MISALIGNED   = {1'b0, 31'd0},
  TRAP_CAUSE_INST_ACCESS_FAULT      = {1'b0, 31'd1},
  TRAP_CAUSE_ILLEGAL_INST           = {1'b0, 31'd2},
  TRAP_CAUSE_BREAKPOINT             = {1'b0, 31'd3},
  TRAP_CAUSE_LOAD_ADDR_MISALIGNED   = {1'b0, 31'd4},
  TRAP_CAUSE_LOAD_ACCESS_FAULT      = {1'b0, 31'd5},
  TRAP_CAUSE_STORE_ADDR_MISALIGNED  = {1'b0, 31'd6},
  TRAP_CAUSE_STORE_ACCESS_FAULT     = {1'b0, 31'd7},
  TRAP_CAUSE_ECALL_U                = {1'b0, 31'd8},
  TRAP_CAUSE_ECALL_M                = {1'b0, 31'd11},
  TRAP_CAUSE_M_SOFTWARE_INTERRUPT   = {1'b1, 31'd3},
  TRAP_CAUSE_M_TIMER_INTERRUPT      = {1'b1, 31'd7},
  TRAP_CAUSE_M_EXTERNAL_INTERRUPT   = {1'b1, 31'd11}
} trap_cause_t;

// Zicsr CSR address
typedef enum logic [ZICSR_CSR_ADDR_WIDTH-1:0] {
  ZICSR_CSR_MSTATUS   = 12'h300,
  ZICSR_CSR_MIE       = 12'h304,
  ZICSR_CSR_MTVEC     = 12'h305,
  ZICSR_CSR_MSCRATCH  = 12'h340,
  ZICSR_CSR_MEPC      = 12'h341,
  ZICSR_CSR_MCAUSE    = 12'h342,
  ZICSR_CSR_MTVAL     = 12'h343,
  ZICSR_CSR_MIP       = 12'h344
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

// mstatus bits
typedef struct packed {
  logic [31:13] reserved_31_13;
  priv_mode_t mpp;
  logic [10:8] reserved_10_8;
  logic mpie;
  logic [6:4] reserved_6_4;
  logic mie;
  logic [2:0] reserved_2_0;
} zicsr_val_mstatus_t;

// mie bits
typedef struct packed {
  logic [31:12] reserved_31_12;
  logic meie;
  logic [10:8] reserved_10_8;
  logic mtie;
  logic [6:4] reserved_6_4;
  logic msie;
  logic [2:0] reserved_2_0;
} zicsr_val_mie_t;

// mtvec bits
typedef struct packed {
  logic [31:2] addr_base;
  logic [1:0] mode;
} zicsr_val_mtvec_t;

// mcause bits
typedef struct packed {
  logic interrupt;
  logic [MXLEN-2:0] code;
} zicsr_val_mcause_t;

// mip bits
typedef struct packed {
  logic [31:12] reserved_31_12;
  logic meip;
  logic [10:8] reserved_10_8;
  logic mtip;
  logic [6:4] reserved_6_4;
  logic msip;
  logic [2:0] reserved_2_0;
} zicsr_val_mip_t;

endpackage : cotm32_priv_pkg
