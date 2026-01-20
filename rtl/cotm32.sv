`timescale 1ns/1ns

// Top module
module cotm32
  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;
(
  input logic i_clk,
  input logic i_rst
);

  logic clint_we /* verilator public */;
  logic [$clog2(CLINT_MEM_SIZE)-1:0] clint_addr /* verilator public */;
  logic [XLEN-1:0] clint_wdata /* verilator public */;
  logic [XLEN-1:0] clint_rdata /* verilator public */;
  logic clint_mtip /* verilator public */;

  logic uart_re /* verilator public */;
  logic uart_we /* verilator public */;
  logic [$clog2(UART_MEM_SIZE)-1:0] uart_addr /* verilator public */;
  logic [XLEN-1:0] uart_wdata /* verilator public */;
  logic [XLEN-1:0] uart_rdata /* verilator public */;

  // Cores
  processor_core core(
    .i_clk(i_clk),
    .i_rst(i_rst),
    
    .i_clint_rdata(clint_rdata),
    .i_clint_mtip(clint_mtip),
    .o_clint_addr(clint_addr),
    .o_clint_we(clint_we),
    .o_clint_wdata(clint_wdata),

    .i_uart_rdata(uart_rdata),
    .o_uart_re(uart_re),
    .o_uart_we(uart_we),
    .o_uart_addr(uart_addr),
    .o_uart_wdata(uart_wdata)
  );

  // CLINT
  clint clint(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_addr(clint_addr),
    .i_we(clint_we),
    .i_wdata(clint_wdata),
    .o_rdata(clint_rdata),
    .o_mtip(clint_mtip)
  );

  // Terminal UART
  uart_term uart_term(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_re(uart_re),
    .i_we(uart_we),
    .i_addr(uart_addr),
    .i_wdata(uart_wdata),
    .o_rdata(uart_rdata)
  );

endmodule
