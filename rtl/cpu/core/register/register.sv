import cotm32_pkg::XLEN;

// Register
module register (
  input logic i_clk,
  input logic i_rst,
  input logic i_we,
  input logic [XLEN-1:0] i_wdata,
  output logic [XLEN-1:0] o_qbus
);

  import cotm32_pkg::*;

  initial begin
    o_qbus = '0;
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      o_qbus <= '0;
    end else if (i_we) begin
      o_qbus <= i_wdata;
    end
  end

endmodule
