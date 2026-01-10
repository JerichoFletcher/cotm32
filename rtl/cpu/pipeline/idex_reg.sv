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

  // EX
  output idex_data_t o_data,
  output logic o_valid
);

  import cotm32_pkg::*;

  always_ff @(posedge i_clk) begin
    if (i_rst || i_flush) begin
      o_valid <= '0;
      o_data <= '0;
      o_data.lsu_ls_op <= LSU_NONE;
      o_data.csr_op <= ZICSR_CSR_OP_NONE;
    end else if (!i_stall) begin
      o_valid <= i_valid;
      o_data <= i_data;
    end
    // Use latched values when stalled
  end
  
endmodule
