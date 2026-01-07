`default_nettype none
`timescale 1ns/1ns

import cotm32_pkg::*;

module tb_processor_core;

  localparam CLK_PD = 10;
  localparam RUN_STEPS = 200;

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
    $display(
      "Memory sections:\nIMEM [%8h:%8h]\nDMEM [%8h:%8h]",
      INST_MEM_START,
      INST_MEM_END,
      DATA_MEM_START,
      DATA_MEM_END
    );
  end

  initial begin
    clk = '0;
    rst = '1;
    #(CLK_PD) rst = '0;
    #(CLK_PD * RUN_STEPS) $finish;
  end

endmodule