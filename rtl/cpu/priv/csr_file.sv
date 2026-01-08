import cotm32_priv_pkg::MXLEN;
import cotm32_priv_pkg::zicsr_csr_op_t;
import cotm32_priv_pkg::zicsr_csr_addr_t;
import cotm32_priv_pkg::trap_cause_t;

module csr_file #(
  parameter CSR_REG_WIDTH = MXLEN
) (
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
  output logic [MXLEN-1:0] o_mtvec,
  output logic [MXLEN-1:0] o_mepc,

  output logic o_t_illegal_inst
);

  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;

  localparam CSR_N = 4;
  localparam CSR_MTVEC_IDX  = 0;
  localparam CSR_MEPC_IDX   = 1;
  localparam CSR_MCAUSE_IDX = 2;
  localparam CSR_MTVAL_IDX  = 3;

  logic [MXLEN-1:0] mem [0:CSR_N-1] /* verilator public */;
  logic [$clog2(CSR_N)-1:0] csr_idx;

  assign o_rdata = mem[csr_idx];
  assign o_mtvec = mem[CSR_MTVEC_IDX];
  assign o_mepc = mem[CSR_MEPC_IDX];

  always_comb begin
    csr_idx = 0;
    o_t_illegal_inst = '0;

    if (i_op != ZICSR_CSR_OP_NONE) begin
      unique case (i_addr)
        ZICSR_CSR_MTVEC : csr_idx = CSR_MTVEC_IDX;
        ZICSR_CSR_MEPC  : csr_idx = CSR_MEPC_IDX;
        ZICSR_CSR_MCAUSE: csr_idx = CSR_MCAUSE_IDX;
        ZICSR_CSR_MTVAL : csr_idx = CSR_MTVAL_IDX;
        default: o_t_illegal_inst = '1;
      endcase
    end
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      for (int i = 0; i < CSR_N; i++) begin
        mem[i] <= '0;
      end
    end else if (i_trap_req) begin
      mem[CSR_MEPC_IDX] <= i_pc;
      mem[CSR_MCAUSE_IDX] <= i_trap_cause;
      mem[CSR_MTVAL_IDX] <= i_trap_tval;
    end else if (i_we) begin
      case (i_op)
        ZICSR_CSR_OP_RW: mem[csr_idx] <= i_wdata;
        ZICSR_CSR_OP_RS: mem[csr_idx] <= mem[csr_idx] | i_wdata;
        ZICSR_CSR_OP_RC: mem[csr_idx] <= mem[csr_idx] & ~i_wdata;
      endcase
    end
  end

endmodule
