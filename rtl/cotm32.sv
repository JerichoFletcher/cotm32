`timescale 1ns/1ns

// Top module
module cotm32
  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;
(
  input logic i_clk,
  input logic i_rst
);

  logic [$clog2(CLINT_MEM_SIZE)-1:0] clint_mmr_addr /* verilator public */;
  logic clint_we /* verilator public */;
  logic [XLEN-1:0] clint_wdata /* verilator public */;

  wire [XLEN-1:0] clint_rdata /* verilator public */;
  wire clint_mtip /* verilator public */;

  // Cores
  processor_core core(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_clint_rdata(clint_rdata),
    .i_clint_mtip(clint_mtip),
    .o_clint_addr(clint_mmr_addr),
    .o_clint_we(clint_we),
    .o_clint_wdata(clint_wdata)
  );

  // CLINT
  clint clint(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_addr(clint_mmr_addr),
    .i_we(clint_we),
    .i_wdata(clint_wdata),
    .o_rdata(clint_rdata),
    .o_mtip(clint_mtip)
  );

endmodule
