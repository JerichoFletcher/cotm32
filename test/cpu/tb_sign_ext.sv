`default_nettype none
`timescale 1ns/1ns

import cotm32_pkg::*;
import cotm32_test_pkg::assert_eq;

module tb_sign_ext;

  logic [XLEN-1:0] inst;
  imm_t sel;
  wire [XLEN-1:0] imm;

  sign_ext dut(
    .i_inst(inst),
    .i_sel(sel),
    .o_imm(imm)
  );

  always @* begin
    $display("time %2t, inst = %8h, sel = %3b, imm = %8h", $time, inst, sel, imm);
  end

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_sign_ext);
  end

  initial begin
    // addi s2, zero, 144
    inst = 32'h09000913;
    sel = IMM_I;
    #10 assert_eq(imm, 32'd144);

    // sw s3, -4(sp)
    #10;
    inst = 32'hff312e23;
    sel = IMM_S;
    #10 assert_eq(imm, -32'd4);

    // beq s2, s3, -8
    #10;
    inst = 32'hff390ce3;
    sel = IMM_B;
    #10 assert_eq(imm, -32'd8);

    // lui s4, 0xabc
    #10;
    inst = 32'h00abca37;
    sel = IMM_U;
    #10 assert_eq(imm, 32'habc000);

    // jal zero, -4
    #10;
    inst = 32'hffdff06f;
    sel = IMM_J;
    #10; assert_eq(imm, -32'd4);

    #10 $finish;
  end

endmodule