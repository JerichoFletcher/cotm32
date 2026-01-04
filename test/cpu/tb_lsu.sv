`default_nettype none
`timescale 1ns/1ns

import cotm32_pkg::*;
import cotm32_test_pkg::assert_eq;

module tb_lsu;

  lsu_ls_t op;
  logic [XLEN-1:0] i_wdata;
  logic [XLEN-1:0] i_rdata;
  wire [XLEN-1:0] o_wdata;
  wire [XLEN-1:0] o_rdata;
  wire [XLEN/BYTE_WIDTH-1:0] wstrb;

  lsu dut(
    .i_op(op),
    .i_wdata(i_wdata),
    .i_rdata(i_rdata),
    .o_wdata(o_wdata),
    .o_rdata(o_rdata),
    .o_wstrb(wstrb)
  );

  always @* begin
    $display(
      "time %2t, op = %4b, i_wdata = %8h, i_rdata = %8h, o_wdata = %8h, o_rdata = %8h, wstrb = %4b",
      $time,
      op,
      i_wdata,
      i_rdata,
      o_wdata,
      o_rdata,
      wstrb
    );
  end

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_lsu);
  end

  initial begin
    i_wdata = '0;
    i_rdata = '0;
    op = LSU_NONE;
    #10;

    // SW
    i_wdata = 32'h12345678;
    op = LSU_STORE_W;

    #10;
    assert_eq(32'h12345678, o_wdata);
    assert_eq(4'b1111, wstrb);

    // SH
    i_wdata = 32'h12345678;
    op = LSU_STORE_H;

    #10;
    assert_eq(32'h00005678, o_wdata);
    assert_eq(4'b0011, wstrb);

    // SB
    i_wdata = 32'h12345678;
    op = LSU_STORE_B;

    #10;
    assert_eq(32'h00000078, o_wdata);
    assert_eq(4'b0001, wstrb);

    // LW
    i_rdata = 32'hffaabbcc;
    op = LSU_LOAD_W;

    #10;
    assert_eq(32'hffaabbcc, o_rdata);

    // LH
    i_rdata = 32'hffaabbcc;
    op = LSU_LOAD_H;

    #10;
    assert_eq(32'hffffbbcc, o_rdata);

    // LB
    i_rdata = 32'hffaabbcc;
    op = LSU_LOAD_B;

    #10;
    assert_eq(32'hffffffcc, o_rdata);

    // LHU
    i_rdata = 32'hffaabbcc;
    op = LSU_LOAD_HU;

    #10;
    assert_eq(32'h0000bbcc, o_rdata);

    // LBU
    i_rdata = 32'hffaabbcc;
    op = LSU_LOAD_BU;

    #10;
    assert_eq(32'h000000cc, o_rdata);

    $finish;
  end

endmodule