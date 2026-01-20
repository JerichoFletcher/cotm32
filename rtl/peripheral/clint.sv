// Core local interrupt
module clint
  import cotm32_pkg::*;
(
  input logic i_clk,
  input logic i_rst,

  input logic i_we,
  input logic [$clog2(CLINT_MEM_SIZE)-1:0] i_addr,
  input logic [XLEN-1:0] i_wdata,

  output logic [XLEN-1:0] o_rdata,
  output logic o_mtip
);

  localparam ADDR_WIDTH = $clog2(CLINT_MEM_SIZE);
  localparam TIMER_PRESCALER = 100;

  localparam ADDR_MSIP      = normalize_addr(CLINT_MMR_ADDR_MSIP);
  localparam ADDR_MTIME     = normalize_addr(CLINT_MMR_ADDR_MTIME);
  localparam ADDR_MTIMEH    = normalize_addr(CLINT_MMR_ADDR_MTIME) + 4;
  localparam ADDR_MTIMECMP  = normalize_addr(CLINT_MMR_ADDR_MTIMECMP);
  localparam ADDR_MTIMECMPH = normalize_addr(CLINT_MMR_ADDR_MTIMECMP) + 4;

  logic [$clog2(TIMER_PRESCALER)-1:0] prescaler_cnt /* verilator public */;

  logic msip /* verilator public */;
  logic [63:0] mtime /* verilator public */;
  logic [63:0] mtimecmp /* verilator public */;

  logic [XLEN-1:0] mem [logic [ADDR_WIDTH-1:0]] /* verilator public */;

  assign mem[ADDR_MSIP]       = {31'b0, msip};
  assign mem[ADDR_MTIME]      = mtime[31:0];
  assign mem[ADDR_MTIMEH]     = mtime[63:32];
  assign mem[ADDR_MTIMECMP]   = mtimecmp[31:0];
  assign mem[ADDR_MTIMECMPH]  = mtimecmp[63:32];

  assign o_mtip = mtime >= mtimecmp;

  always_comb begin
    o_rdata = '0;
    if (mem.exists(i_addr) == 1) begin
      o_rdata = mem[i_addr];
    end
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      msip      <= '0;
      mtime     <= '0;
      mtimecmp  <= '1;
      prescaler_cnt <= TIMER_PRESCALER;
    end else begin
      if (i_we && mem.exists(i_addr) == 1) begin
        unique case (i_addr)
          ADDR_MSIP: begin
            msip <= i_wdata[0];
          end
          ADDR_MTIME: begin
            mtime[31:0] <= i_wdata;
          end
          ADDR_MTIMEH: begin
            mtime[63:32] <= i_wdata;
          end
          ADDR_MTIMECMP: begin
            mtimecmp[31:0] <= i_wdata;
          end
          ADDR_MTIMECMPH: begin
            mtimecmp[63:32] <= i_wdata;
          end
          default: begin /* NOP */ end
        endcase
      end

      if (0 < prescaler_cnt) begin
        prescaler_cnt <= prescaler_cnt - 1;
      end else begin
        mtime <= mtime + 64'd1;
        prescaler_cnt <= TIMER_PRESCALER;
      end
    end
  end

  function logic [ADDR_WIDTH-1:0] normalize_addr(input logic [XLEN-1:0] addr);
    logic [XLEN-1:0] t;
    t = (addr - CLINT_MEM_START);
    normalize_addr = t[ADDR_WIDTH-1:0];
  endfunction

endmodule
