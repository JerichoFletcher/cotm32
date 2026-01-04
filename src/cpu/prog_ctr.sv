import cotm32_pkg::*;

// Program counter register
module prog_ctr #(
  parameter RESET_VECTOR = '0
) (
  input logic i_clk,
  input logic i_rst,
  input logic [XLEN-1:0] i_addr,
  output logic [XLEN-1:0] o_addr
);

  initial begin
    o_addr = RESET_VECTOR;
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      o_addr <= RESET_VECTOR;
    end else begin
      o_addr <= i_addr;
    end
  end

endmodule