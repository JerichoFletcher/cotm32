import cotm32_pipeline_pkg::memwb_data_t;

// MEM/WB pipeline register
module memwb_reg(
  input logic i_clk,
  input logic i_rst,

  input logic i_stall,
  input logic i_flush,

  // ID
  input memwb_data_t i_data,
  input logic i_valid,

  // EX
  output memwb_data_t o_data,
  output logic o_valid
);

  import cotm32_pkg::*;

  // NOP = addi x0, x0, 0
  localparam INST_NOP = 32'h00000013;

  always_ff @(posedge i_clk) begin
    if (i_rst || i_flush) begin
      o_valid <= '0;
      o_data <= '0;
      o_data.inst <= INST_NOP;
    end else if (!i_stall) begin
      o_valid <= i_valid;
      o_data <= i_data;
    end
    // Use latched values when stalled
  end
  
endmodule
