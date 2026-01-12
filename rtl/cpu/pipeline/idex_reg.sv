import cotm32_pipeline_pkg::idex_data_t;

// ID/EX pipeline register
module idex_reg(
  input logic i_clk,
  input logic i_rst,

  input logic i_stall,
  input logic i_flush,

  // ID
  input idex_data_t i_data,
  input logic i_valid,

  input logic i_t_inst_addr_misaligned,
  input logic i_t_inst_access_fault,
  input logic i_t_illegal_inst,
  input logic i_t_ebreak,
  input logic i_t_ecall_m,

  input logic i_trap_mret,

  // EX
  output idex_data_t o_data,
  output logic o_valid,

  output logic o_t_inst_addr_misaligned,
  output logic o_t_inst_access_fault,
  output logic o_t_illegal_inst,
  output logic o_t_ebreak,
  output logic o_t_ecall_m,

  output logic o_trap_mret
);

  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;

  // NOP = addi x0, x0, 0
  localparam INST_NOP = 32'h00000013;

  always_ff @(posedge i_clk) begin
    if (i_rst || i_flush) begin
      o_valid <= '0;
      o_data <= '0;
      o_data.inst <= INST_NOP;
      
      o_data.lsu_ls_op <= LSU_NONE;
      o_data.csr_op <= ZICSR_CSR_OP_NONE;

      o_t_inst_addr_misaligned <= '0;
      o_t_inst_access_fault <= '0;
      o_t_illegal_inst <= '0;
      o_t_ebreak <= '0;
      o_t_ecall_m <= '0;

      o_trap_mret <= '0;
    end else if (!i_stall) begin
      o_valid <= i_valid;
      o_data <= i_data;

      o_t_inst_addr_misaligned <= i_t_inst_addr_misaligned;
      o_t_inst_access_fault <= i_t_inst_access_fault;
      o_t_illegal_inst <= i_t_illegal_inst;
      o_t_ebreak <= i_t_ebreak;
      o_t_ecall_m <= i_t_ecall_m;

      o_trap_mret <= i_trap_mret;
    end
    // Use latched values when stalled
  end
  
endmodule
