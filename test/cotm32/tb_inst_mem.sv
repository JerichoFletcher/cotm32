`default_nettype none
`timescale 1ns/1ns

`include "defs.svh"
`include "assert.svh"

module tb_inst_mem;

  localparam MEM_SIZE = 1024;
  logic [$clog2(MEM_SIZE)-1:0] addr;
  logic [`INST_WIDTH-1:0] inst;
  
  inst_mem #(
    .MEM_SIZE(MEM_SIZE)
  ) dut(
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
    addr = 32'h00000000;
    #10 addr = 32'h00000004;
    #10 addr = 32'h00000008;
    #10 addr = 32'h0000000c;
    #10 $finish;
  end

endmodule;