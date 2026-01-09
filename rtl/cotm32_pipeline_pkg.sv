`timescale 1ns/1ns

package cotm32_pipeline_pkg;

import cotm32_pkg::*;
import cotm32_priv_pkg::*;

// IF/ID data
typedef struct packed {
  logic [XLEN-1:0] pc;
  logic [XLEN-1:0] pc_4;
  logic [INST_WIDTH-1:0] inst;
} ifid_data_t;

// ID/EX data
typedef struct packed {
  alu_op_t alu_op;
  alu_a_sel_t alu_a_sel;
  alu_b_sel_t alu_b_sel;

  logic bu_be;
  bu_op_t bu_op;

  logic regfile_we;
  logic [$clog2(NUM_REGS)-1:0] rd_addr;
  logic [$clog2(NUM_REGS)-1:0] rs1_addr;
  logic [$clog2(NUM_REGS)-1:0] rs2_addr;
  lsu_ls_t lsu_ls_op;
  reg_wb_sel_t reg_wb_sel;

  logic csr_we;
  zicsr_data_sel_t csr_data_sel;
  zicsr_csr_addr_t csr_addr;
  zicsr_csr_op_t csr_op;
  logic [MXLEN-1:0] csr_zimm;

  logic [XLEN-1:0] rs1;
  logic [XLEN-1:0] rs2;
  logic [XLEN-1:0] imm;

  logic [XLEN-1:0] pc;
  logic [XLEN-1:0] pc_4;
} idex_data_t;

endpackage : cotm32_pipeline_pkg
