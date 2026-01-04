`default_nettype none
`timescale 1ns/1ns

import cotm32_pkg::*;
import cotm32_test_pkg::assert_eq;

module tb_alu;

  logic [XLEN-1:0] a;
  logic [XLEN-1:0] b;
  alu_op_t op;
  wire [XLEN-1:0] o;

  alu dut(
    .i_a(a),
    .i_b(b),
    .i_op(op),
    .o(o)
  );

  always @* begin
    $display("time %2t, a = %d, b = %d, op = %4b, o = %d", $time, a, b, op, o);
  end

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_alu);
  end

  initial begin
    // 1 + 2 = 3
    a = 32'd1;
    b = 32'd2;
    op = ALU_ADD;

    #10 assert_eq(o, 32'd3);

    // 8 - 3 = 5
    a = 32'd8;
    b = 32'd3;
    op = ALU_SUB;

    #10 assert_eq(o, 32'd5);

    // b01101000 | b01010101 = b01111101
    a = 32'b01101000;
    b = 32'b01010101;
    op = ALU_OR;

    #10 assert_eq(o, 32'b01111101);

    // b01101000 & b01010101 = b01000000
    a = 32'b01101000;
    b = 32'b01010101;
    op = ALU_AND;

    #10 assert_eq(o, 32'b01000000);

    // b01101000 ^ b01010101 = b00111101
    a = 32'b01101000;
    b = 32'b01010101;
    op = ALU_XOR;

    #10 assert_eq(o, 32'b00111101);

    // -30 < 25
    a = -32'd30;
    b = 32'd25;
    op = ALU_SLT;

    #10 assert_eq(o, 32'b1);

    // -30 u< 25 (eq. 98 < 25)
    a = -32'd30;
    b = 32'd25;
    op = ALU_SLTU;

    #10 assert_eq(o, 32'b0);

    // 30 << 2 = 120
    a = 32'd30;
    b = 32'd2;
    op = ALU_SLL;

    #10 assert_eq(o, 32'd120);

    // hffffffff >> 8 = h00ffffff
    a = 32'hffffffff;
    b = 32'd8;
    op = ALU_SRL;

    #10 assert_eq(o, 32'h00ffffff);

    // -60 >> 2 = -15
    a = -32'd60;
    b = 32'd2;
    op = ALU_SRA;

    #10 assert_eq(o, -32'd15);

    $finish;
  end

endmodule