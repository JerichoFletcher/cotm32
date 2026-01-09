`timescale 1ns/1ns

// Top module
module cotm32 (
  input logic i_clk,
  input logic i_rst
);

  processor_core core(
    .i_clk(i_clk),
    .i_rst(i_rst)
  );

endmodule
