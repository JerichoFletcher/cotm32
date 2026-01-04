`default_nettype none
`timescale 1ns/1ns

import cotm32_pkg::*;
import cotm32_test_pkg::assert_eq;

module tb_register_file;

  localparam NUM_RPORTS = 2;
  localparam CLK_PD = 10;
  localparam ADDR_WIDTH = $clog2(NUM_REGS);

  logic clk;
  logic we;
  logic [XLEN-1:0] wdata;
  logic [ADDR_WIDTH-1:0] waddr;
  logic [ADDR_WIDTH-1:0] raddr [0:NUM_RPORTS-1];
  wire [XLEN-1:0] rdata [0:NUM_RPORTS-1];

  register_file #(
    .N_RPORTS(NUM_RPORTS),
    .N_REGS(NUM_REGS)
  ) dut(
    .i_clk(clk),
    .i_we(we),
    .i_wdata(wdata),
    .i_waddr(waddr),
    .i_raddr(raddr),
    .o_rdata(rdata)
  );

  always #(CLK_PD / 2) clk = ~clk;

  always @* begin
    $display(
      "time %2t, we = %1b, waddr = %3h, wdata = %8h, raddr_a = %3h, raddr_b = %3h, rdata_a = %8h, rdata_b = %8h",
      $time,
      we,
      waddr,
      wdata,
      raddr[0],
      raddr[1],
      rdata[0],
      rdata[1]
    );
  end

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_register_file);
  end

  initial begin
    clk = '0;
    we = '0;
    waddr = '0;
    wdata = '0;
    raddr[0] = '0;
    raddr[1] = '0;

    // Test: x01 = 32'h12345600
    #(CLK_PD);
    waddr = 5'h01;
    wdata = 32'h12345600;
    we = '1;

    // Test: A = x01, expect A == 32'h12345600
    #(CLK_PD);
    we = '0;
    raddr[0] = 5'h01;
    assert_eq(rdata[0], 32'h12345600);

    // Test: x15 = 32'habcdef00
    #(CLK_PD);
    waddr = 5'h0f;
    wdata = 32'habcdef00;
    we = '1;

    // Test: B = x15, expect B == 32'habcdef00
    #(CLK_PD);
    we = '0;
    raddr[1] = 5'h0f;
    assert_eq(rdata[0], 32'h12345600);
    assert_eq(rdata[1], 32'habcdef00);

    // Test: x31 = 32'haabbccdd;
    #(CLK_PD);
    waddr = 5'h1f;
    wdata = 32'haabbccdd;
    we = '1;

    // Test: A = x31, B = x01, expect saved values
    #(CLK_PD);
    we = '0;
    raddr[0] = 5'h1f;
    raddr[1] = 5'h01;
    assert_eq(rdata[0], 32'haabbccdd);
    assert_eq(rdata[1], 32'h12345600);

    // Test: x0 = 32'hccddeeff, A = x0, expect A == '0
    #(CLK_PD);
    waddr = 5'h00;
    wdata = 32'hccddeeff;
    we = '1;

    #(CLK_PD);
    we = '0;
    raddr[0] = 5'h00;
    assert_eq('0, rdata[0]);

    // Test: repeated writes
    #(CLK_PD);
    waddr = 5'h02;
    wdata = 32'h00000001;
    we = '1;
    raddr[0] = 5'h02;

    #(CLK_PD);
    assert_eq(32'h00000001, rdata[0]);
    wdata = 32'h00000002;

    #(CLK_PD);
    assert_eq(32'h00000002, rdata[0]);
    wdata = 32'h00000003;

    #(CLK_PD);
    assert_eq(32'h00000003, rdata[0]);

    #(CLK_PD) $finish;
  end

endmodule