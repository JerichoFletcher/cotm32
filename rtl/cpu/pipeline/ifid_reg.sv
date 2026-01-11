import cotm32_pipeline_pkg::ifid_data_t;

// IF/ID pipeline register
module ifid_reg(
  input logic i_clk,
  input logic i_rst,

  input logic i_stall,
  input logic i_flush,

  // IF
  input ifid_data_t i_data,

  input logic i_t_inst_addr_misaligned,
  input logic i_t_inst_access_fault,

  // ID
  output ifid_data_t o_data,
  output logic o_valid,

  output logic o_t_inst_addr_misaligned,
  output logic o_t_inst_access_fault
);

  import cotm32_pkg::*;

  // NOP = addi x0, x0, 0
  localparam INST_NOP = 32'h00000013;

  always_ff @(posedge i_clk) begin
    if (i_rst || i_flush) begin
      o_valid <= '0;
      o_data <= '0;
      o_data.inst <= INST_NOP;

      o_t_inst_addr_misaligned <= '0;
      o_t_inst_access_fault <= '0;
    end else if (!i_stall) begin
      o_valid <= '1;
      o_data <= i_data;

      o_t_inst_addr_misaligned <= i_t_inst_addr_misaligned;
      o_t_inst_access_fault <= i_t_inst_access_fault;
    end
    // Use latched values when stalled
  end
  
endmodule
