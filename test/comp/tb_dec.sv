`default_nettype none
`timescale 1ns/1ns

module tb_dec;

  logic [2:0] i;
  logic [1:0] o1;
  logic [3:0] o2;

  dec #(.N_SEL_BITS(1)) d1(.i_sel(i[0]), .o(o1));
  dec #(.N_SEL_BITS(2)) d2(.i_sel(i[1:0]), .o(o2));

  initial begin
    $dumpfile("tb.vcd");
    $dumpvars(0, tb_dec);
  end

  initial begin
    i = 0;

    #10 i = 1;
    #10 i = 2;
    #10 i = 3;
    #10 i = 4;
    #10 i = 5;
    #10 i = 6;
    #10 i = 7;
    #10 $finish;
  end

endmodule