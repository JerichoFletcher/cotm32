import cotm32_pkg::*;
import cotm32_priv_pkg::*;

module csr #(
  parameter CSR_REG_WIDTH = MXLEN
) (
  input logic i_clk,
  input logic i_rst,
  input logic i_we,
  input zicsr_csr_op_t i_op,
  input zicsr_csr_addr_t i_addr,
  input logic [MXLEN-1:0] i_wdata,
  input logic [MXLEN-1:0] i_pc,

  input logic i_trap_req,
  input trap_cause_t i_trap_cause,
  input logic [MXLEN-1:0] i_trap_tval,

  output logic [MXLEN-1:0] o_rdata,
  output logic [MXLEN-1:0] o_mtvec,
  output logic [MXLEN-1:0] o_mepc
);
endmodule