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

  input logic i_trap_req,
  input trap_cause_t i_trap_cause,
  input logic [MXLEN-1:0] i_trap_tval,

  output logic [MXLEN-1:0] o_rdata,
  output zicsr_val_mtvec_t o_mtvec,
  output logic [MXLEN-1:0] o_mepc,

  output logic o_t_illegal_inst
);

  logic [MXLEN-1:0] mem [zicsr_csr_addr_t] /* verilator public */;

  assign o_rdata  = mem[i_addr];
  assign o_mtvec  = zicsr_val_mtvec_t'(mem[ZICSR_CSR_MTVEC]);
  assign o_mepc   = mem[ZICSR_CSR_MEPC];

  always_comb begin
    o_t_illegal_inst = '0;

    if (i_op != ZICSR_CSR_OP_NONE) begin
      if (mem.exists(i_addr) == 0) begin
        o_t_illegal_inst = '1;
      end
    end
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      mem[ZICSR_CSR_MSTATUS]  <= '0;
      mem[ZICSR_CSR_MIE]      <= '0;
      mem[ZICSR_CSR_MTVEC]    <= '0;
      mem[ZICSR_CSR_MEPC]     <= '0;
      mem[ZICSR_CSR_MCAUSE]   <= '0;
      mem[ZICSR_CSR_MTVAL]    <= '0;
      mem[ZICSR_CSR_MIP]      <= '0;
    end else if (i_trap_req) begin
      mem[ZICSR_CSR_MEPC]     <= i_pc;
      mem[ZICSR_CSR_MCAUSE]   <= i_trap_cause;
      mem[ZICSR_CSR_MTVAL]    <= i_trap_tval;
    end else if (i_we && mem.exists(i_addr) == 1) begin
      case (i_op)
        ZICSR_CSR_OP_RW: mem[i_addr] <= i_wdata;
        ZICSR_CSR_OP_RS: mem[i_addr] <= mem[i_addr] | i_wdata;
        ZICSR_CSR_OP_RC: mem[i_addr] <= mem[i_addr] & ~i_wdata;
        default: begin /* NOP */ end
      endcase
    end
  end

endmodule
