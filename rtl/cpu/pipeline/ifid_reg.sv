import cotm32_pkg::XLEN;
import cotm32_pkg::INST_WIDTH;

// IF/ID pipeline register
module ifid_reg(
  input logic i_clk,
  input logic i_rst,

  input logic i_stall,
  input logic i_flush,

  // IF
  input logic [XLEN-1:0] i_pc,
  input logic [XLEN-1:0] i_pc_4,
  input logic [INST_WIDTH-1:0] i_inst,

  // ID
  output logic [XLEN-1:0] o_pc,
  output logic [XLEN-1:0] o_pc_4,
  output logic [INST_WIDTH-1:0] o_inst,

  output logic o_valid
);

  import cotm32_pkg::*;

  // NOP = addi x0, x0, 0
  localparam INST_NOP = 32'h00000013;

  always_ff @(posedge i_clk) begin
    if (i_rst || i_flush) begin
      o_valid <= '0;
      o_pc <= '0;
      o_pc_4 <= '0;
      o_inst <= INST_NOP;
    end else if (!i_stall) begin
      o_valid <= '1;
      o_pc <= i_pc;
      o_pc_4 <= i_pc_4;
      o_inst <= i_inst;
    end
    // Use latched values when stalled
  end
  
endmodule
