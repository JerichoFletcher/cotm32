`default_nettype none
`timescale 1ns/1ns

import cotm32_pkg::*;

module tb_processor_core;

  localparam CLK_PD = 10;
  localparam RUN_STEPS = 60;

  logic clk;
  logic rst;

  processor_core dut(
    .i_clk(clk),
    .i_rst(rst)
  );

  always #(CLK_PD / 2) clk = ~clk;

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_processor_core);
  end

  initial begin
    clk = '0;
    rst = '0;

    #(CLK_PD * RUN_STEPS) $finish;
  end

endmodule