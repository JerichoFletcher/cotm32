`default_nettype none
`timescale 1ns/1ns

`include "defs.svh"
`include "assert.svh"

module tb_bu;

  logic [`XLEN-1:0] a;
  logic [`XLEN-1:0] b;
  bu_op_t op;
  wire take;

  bu dut(
    .i_a(a),
    .i_b(b),
    .i_op(op),
    .o_take(take)
  );

  always @* begin
    $display("time %2t, a = %d, b = %d, op = %4b, take = %1b", $time, a, b, op, take);
  end

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_bu);
  end

  initial begin
    // take = 1 == 1
    a = 32'd1;
    b = 32'd1;
    op = BU_EQ;

    #10 `assert(take, 1'b1);

    // take = 1 != 1
    a = 32'd1;
    b = 32'd1;
    op = BU_NE;

    #10 `assert(take, 1'b0);

    // take = 2 < 3
    a = 32'd2;
    b = 32'd3;
    op = BU_LT;

    #10 `assert(take, 1'b1);

    // take = 2 >= 3
    a = 32'd2;
    b = 32'd3;
    op = BU_GE;

    #10 `assert(take, 1'b0);
    
    // take = -1 u< 3 (eq. I32 max < 3)
    a = -32'd1;
    b = 32'd3;
    op = BU_LTU;

    #10 `assert(take, 1'b0);

    // take = -1 u>= 3 (eq. I32 max < 3)
    a = -32'd1;
    b = 32'd3;
    op = BU_GEU;

    #10 `assert(take, 1'b1);

    $finish;
  end

endmodule