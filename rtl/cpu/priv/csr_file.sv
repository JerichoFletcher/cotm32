module csr_file
  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;
(
  input logic i_clk,
  input logic i_rst,
  input logic i_we,
  input zicsr_csr_op_t i_op,
  input zicsr_csr_addr_t i_addr,
  input logic [MXLEN-1:0] i_wdata,
  input logic [MXLEN-1:0] i_pc,
  input priv_mode_t i_priv_mode,

  input logic i_trap_mret,
  input logic i_trap_req,
  input trap_cause_t i_trap_cause,
  input logic [MXLEN-1:0] i_trap_tval,

  input logic i_mtip,
  input logic i_meip,

  output logic [MXLEN-1:0] o_rdata,
  output zicsr_val_mstatus_t o_mstatus,
  output zicsr_val_mie_t o_mie,
  output zicsr_val_mtvec_t o_mtvec,
  output logic [MXLEN-1:0] o_mepc,
  output zicsr_val_mip_t o_mip,

  output logic o_t_illegal_inst
);

  zicsr_val_mstatus_t mstatus;
  zicsr_val_mie_t mie;
  zicsr_val_mtvec_t mtvec;
  logic [MXLEN-1:0] mscratch;
  logic [MXLEN-1:0] mepc;
  logic [MXLEN-1:0] mcause;
  logic [MXLEN-1:0] mtval;
  zicsr_val_mip_t mip;

  logic [MXLEN-1:0] mem [zicsr_csr_addr_t] /* verilator public */;
  logic [MXLEN-1:0] wval;

  assign mem[ZICSR_CSR_MSTATUS]   = mstatus;
  assign mem[ZICSR_CSR_MIE]       = mie;
  assign mem[ZICSR_CSR_MTVEC]     = mtvec;
  assign mem[ZICSR_CSR_MSCRATCH]  = mscratch;
  assign mem[ZICSR_CSR_MEPC]      = mepc;
  assign mem[ZICSR_CSR_MCAUSE]    = mcause;
  assign mem[ZICSR_CSR_MTVAL]     = mtval;
  assign mem[ZICSR_CSR_MIP]       = mip;

  assign o_rdata    = mem[i_addr];
  assign o_mstatus  = mstatus;
  assign o_mie      = mie;
  assign o_mtvec    = mtvec;
  assign o_mepc     = mepc;
  assign o_mip      = mip;

  always_comb begin
    o_t_illegal_inst = '0;

    if (i_op != ZICSR_CSR_OP_NONE) begin
      if (!csr_accessible(i_addr, i_priv_mode, i_we)) begin
        o_t_illegal_inst = '1;
      end
    end
  end

  always_comb begin
    case (i_op)
      ZICSR_CSR_OP_RW: wval = i_wdata;
      ZICSR_CSR_OP_RS: wval = mem[i_addr] | i_wdata;
      ZICSR_CSR_OP_RC: wval = mem[i_addr] & ~i_wdata;
      default: wval = '0;
    endcase
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      mstatus   <= '0;
      mie       <= '0;
      mtvec     <= '0;
      mscratch  <= '0;
      mepc      <= '0;
      mcause    <= '0;
      mtval     <= '0;
      mip       <= '0;
    end else begin
      if (i_trap_req) begin
        mepc    <= i_pc;
        mcause  <= i_trap_cause;
        mtval   <= i_trap_tval;
        
        mstatus.mpie  <= mstatus.mie;
        mstatus.mie   <= '0;
        mstatus.mpp   <= i_priv_mode;
      end else if (i_trap_mret) begin
        mstatus.mie   <= mstatus.mpie;
        mstatus.mpie  <= '1;
        mstatus.mpp   <= PRIV_U;
      end else if (i_we && csr_accessible(i_addr, i_priv_mode, i_we)) begin
        unique case (i_addr)
          ZICSR_CSR_MSTATUS : mstatus   <= wval;
          ZICSR_CSR_MIE     : mie       <= wval;
          ZICSR_CSR_MTVEC   : mtvec     <= wval;
          ZICSR_CSR_MSCRATCH: mscratch  <= wval;
          ZICSR_CSR_MEPC    : mepc      <= wval;
          ZICSR_CSR_MCAUSE  : mcause    <= wval;
          ZICSR_CSR_MTVAL   : mtval     <= wval;
          ZICSR_CSR_MIP     : mip       <= wval;
          default: begin /* NOP */ end
        endcase
      end

      mip.mtip <= i_mtip;
      mip.meip <= i_meip;
    end
  end

  function automatic logic csr_accessible(
    input zicsr_csr_addr_t addr,
    priv_mode_t priv_mode,
    logic write
  );
    if (mem.exists(addr) == 0) return '0;
    if (addr[9:8] > priv_mode) return '0;
    if (write && addr[11:10] == 2'b11) return '0;
    return '1;
  endfunction

endmodule
