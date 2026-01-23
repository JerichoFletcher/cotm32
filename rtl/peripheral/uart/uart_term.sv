// Terminal UART
module uart_term
  import cotm32_pkg::*;
(
  input logic i_clk,
  input logic i_rst,

  input logic i_re,
  input logic i_we,
  input logic [$clog2(UART_MEM_SIZE)-1:0] i_addr,
  input logic [XLEN-1:0] i_wdata,

  output logic o_meip,
  output logic [XLEN-1:0] o_rdata
);

  localparam ADDR_WIDTH = $clog2(UART_MEM_SIZE);

  localparam ADDR_TXDATA = normalize_addr(UART_MMR_ADDR_TERM_TXDATA);
  localparam ADDR_RXDATA = normalize_addr(UART_MMR_ADDR_TERM_RXDATA);
  localparam ADDR_STATUS = normalize_addr(UART_MMR_ADDR_TERM_STATUS);

  logic [BYTE_WIDTH-1:0] tx_data /* verilator public */;
  logic tx_valid /* verilator public */;

  logic [BYTE_WIDTH-1:0] rx_data /* verilator public */;
  logic rx_valid /* verilator public */;

  logic re;
  logic we;
  assign re = i_re && i_addr inside {ADDR_TXDATA, ADDR_RXDATA, ADDR_STATUS};
  assign we = i_we && i_addr inside {ADDR_TXDATA, ADDR_RXDATA, ADDR_STATUS};

  assign o_meip = rx_valid;

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      tx_valid <= '0;
    end else if (we && i_addr == ADDR_TXDATA) begin
      tx_data <= i_wdata[BYTE_WIDTH-1:0];
      tx_valid <= '1;
    end else begin
      tx_valid <= '0;
    end
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      rx_valid <= '0;
    end else if (re && i_addr == ADDR_RXDATA) begin
      // RX valid flag is cleared on read
      // Data and flag expected to be set from C++
      rx_valid <= '0;
    end
  end

  always_comb begin
    case (i_addr)
      ADDR_RXDATA : o_rdata = {{(XLEN-1-BYTE_WIDTH){1'b0}}, rx_valid, rx_data};
      ADDR_STATUS : o_rdata = {{(XLEN-2){1'b0}}, rx_valid, 1'b0};
      default     : o_rdata = '0;
    endcase
  end

  function logic [ADDR_WIDTH-1:0] normalize_addr(input logic [XLEN-1:0] addr);
    logic [XLEN-1:0] t;
    t = (addr - UART_MEM_START);
    normalize_addr = t[ADDR_WIDTH-1:0];
  endfunction

endmodule
