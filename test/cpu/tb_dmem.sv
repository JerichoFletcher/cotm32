`default_nettype none
`timescale 1ns/1ns

import cotm32_pkg::*;
import cotm32_test_pkg::assert_eq;

module tb_dmem;

  localparam MEM_SIZE = 16;
  localparam CLK_PD = 10;
  localparam DATA_BYTE_COUNT = XLEN/BYTE_WIDTH;

  logic clk;
  logic we;
  logic [XLEN-1:0] addr;
  logic [XLEN-1:0] wdata;
  logic [DATA_BYTE_COUNT-1:0] wstrb;
  wire [XLEN-1:0] rdata;

  dmem #(
    .MEM_SIZE(MEM_SIZE)
  ) dut(
    .i_clk(clk),
    .i_we(we),
    .i_addr(addr),
    .i_wdata(wdata),
    .i_wstrb(wstrb),
    .o_rdata(rdata)
  );

  always #(CLK_PD / 2) clk = ~clk;

  always @* begin
    $display(
      "time %2t, we = %1b, addr = %8h, wdata = %8h, wstrb = %4b, rdata = %8h",
      $time,
      we,
      addr,
      wdata,
      wstrb,
      rdata
    );
  end

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_dmem);
  end

  initial begin
    clk = '0;
    we = '0;
    addr = '0;
    wdata = '0;
    wstrb = '0;
    we = '0;

    // M[0] = 32'h11223344
    #(CLK_PD);
    addr = 32'h0;
    wdata = 32'h11223344;
    wstrb = 4'b1111;
    we = '1;

    // M[1] = 16'hccdd
    #(CLK_PD);
    addr = 32'h4;
    wdata = 32'haabbccdd;
    wstrb = 4'b0011;
    we = '1;

    // M[2] = 8'h78
    #(CLK_PD);
    addr = 32'h8;
    wdata = 32'h12345678;
    wstrb = 4'b0001;
    we = '1;

    #(CLK_PD) we = '0;

    addr = 32'h0;
    #(CLK_PD) assert_eq(32'h11223344, rdata);

    addr = 32'h4;
    #(CLK_PD) assert_eq(32'h0000ccdd, rdata);

    addr = 32'h8;
    #(CLK_PD) assert_eq(32'h00000078, rdata);

    // M[0][15:0] = 16'h8899
    addr = 32'h0;
    wdata = 32'h66778899;
    wstrb = 4'b0011;
    we = '1;

    #(CLK_PD) we = '0;
    assert_eq(32'h11228899, rdata);

    $finish;
  end

endmodule