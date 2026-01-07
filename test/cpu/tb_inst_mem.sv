`default_nettype none
`timescale 1ns/1ns

import cotm32_pkg::*;
import cotm32_test_pkg::assert_eq;

module tb_inst_mem;

  logic [$clog2(MEM_SIZE)-1:0] addr;
  logic [INST_WIDTH-1:0] inst;
  
  inst_mem dut(
    .i_addr(addr),
    .o_inst(inst)
  );

  always @* begin
    $display("time %2t, addr = %3h, inst = %8h", $time, addr, inst);
  end

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_inst_mem);
  end

  initial begin
    addr = 32'h0;
    #10 addr = 32'h4;
    #10 addr = 32'h8;
    #10 addr = 32'hc;
    #10 $finish;
  end

endmodule;