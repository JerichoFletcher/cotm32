import cotm32_pkg::XLEN;
import cotm32_priv_pkg::MXLEN;
import cotm32_priv_pkg::trap_cause_t;

// Trap dispatch
module trap_dispatch (
  input logic [XLEN-1:0] i_pc,
  input logic [XLEN-1:0] i_inst,
  input logic [XLEN-1:0] i_ls_addr,

  input logic i_illegal_inst,
  input logic i_inst_addr_misaligned,
  input logic i_ecall_m,
  input logic i_ebreak,
  input logic i_load_addr_misaligned,
  input logic i_store_addr_misaligned,

  output logic o_trap_req,
  output trap_cause_t o_trap_cause,
  output logic [MXLEN-1:0] o_trap_tval
);

  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;

  always_comb begin
    o_trap_req = '0;
    o_trap_tval = '0;

    if (i_illegal_inst) begin
      o_trap_req = '1;
      o_trap_cause = TRAP_CAUSE_ILLEGAL_INST;
      o_trap_tval = i_inst;
    end else if (i_inst_addr_misaligned) begin
      o_trap_req = '1;
      o_trap_cause = TRAP_CAUSE_INST_ADDR_MISALIGNED;
      o_trap_tval = i_pc;
    end else if (i_ecall_m) begin
      o_trap_req = '1;
      o_trap_cause = TRAP_CAUSE_ECALL_M;
    end else if (i_ebreak) begin
      o_trap_req = '1;
      o_trap_cause = TRAP_CAUSE_BREAKPOINT;
    end else if (i_load_addr_misaligned) begin
      o_trap_req = '1;
      o_trap_cause = TRAP_CAUSE_LOAD_ADDR_MISALIGNED;
      o_trap_tval = i_ls_addr;
    end else if (i_store_addr_misaligned) begin
      o_trap_req = '1;
      o_trap_cause = TRAP_CAUSE_STORE_ADDR_MISALIGNED;
      o_trap_tval = i_ls_addr;
    end
  end

endmodule
