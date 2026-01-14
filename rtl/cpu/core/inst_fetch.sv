import cotm32_pkg::XLEN;
import cotm32_priv_pkg::MXLEN;

// Instruction fetch unit
module inst_fetch #(
  parameter RESET_VECTOR = '0
) (
  input logic i_clk,
  input logic i_rst,
  input logic i_take_branch,
  input logic [XLEN-1:0] i_new_addr,

  input logic i_stall,

  input logic i_trap_mret,
  input logic i_trap_req,
  input logic [MXLEN-1:0] i_mtvec,
  input logic [MXLEN-1:0] i_mepc,

  output logic [XLEN-1:0] o_addr,
  output logic [XLEN-1:0] o_addr_4,

  output logic o_t_inst_addr_misaligned,
  output logic o_t_inst_access_fault
);

  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;

  ifu_pc_sel_t pc_sel;
  wire [XLEN-1:0] pc_mux_vals [0:IFU_PC_VALCOUNT-1];
  wire [XLEN-1:0] pc_mux_out;

  assign o_addr_4 = o_addr + 'd4;

  assign pc_mux_vals[IFU_PC_PC4] = o_addr_4;
  assign pc_mux_vals[IFU_PC_BRANCH] = i_new_addr;
  assign pc_mux_vals[IFU_PC_MTVEC] = i_mtvec;
  assign pc_mux_vals[IFU_PC_MEPC] = i_mepc;

  mux #(
    .N_OPTIONS(IFU_PC_VALCOUNT),
    .DATA_WIDTH(XLEN)
  ) m(
    .i_sel(pc_sel),
    .i_val(pc_mux_vals),
    .o_val(pc_mux_out)
  );

  initial begin
    o_addr = RESET_VECTOR;
  end

  always_comb begin
    pc_sel = IFU_PC_PC4;
    if (i_trap_req) begin
      pc_sel = IFU_PC_MTVEC;
    end else if (i_trap_mret) begin
      pc_sel = IFU_PC_MEPC;
    end else if (i_take_branch) begin
      pc_sel = IFU_PC_BRANCH;
    end
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      o_addr <= RESET_VECTOR;
    end else if (!i_stall) begin
      o_addr <= pc_mux_out;
    end
  end

  always_comb begin
    o_t_inst_addr_misaligned = '0;
    if (o_addr[1:0] != '0) begin
      o_t_inst_addr_misaligned = '1;
    end
  end

  always_comb begin
    o_t_inst_access_fault = '0;
    if ((BOOTROM_MEM_START > 0 && o_addr < BOOTROM_MEM_START) || o_addr > BOOTROM_MEM_END) begin
      o_t_inst_access_fault = '1;
    end
  end

endmodule
