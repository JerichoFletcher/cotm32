import cotm32_pkg::NUM_REGS;
import cotm32_pipeline_pkg::forward_src_t;

// Forwarding unit
module fwd_unit (
  input logic [$clog2(NUM_REGS)-1:0] i_idex_rs1_addr,
  input logic [$clog2(NUM_REGS)-1:0] i_idex_rs2_addr,

  input logic [$clog2(NUM_REGS)-1:0] i_exmem_rd_addr,
  input logic i_exmem_regfile_we,
  input logic i_exmem_valid,

  input logic [$clog2(NUM_REGS)-1:0] i_memwb_rd_addr,
  input logic i_memwb_regfile_we,
  input logic i_memwb_valid,

  output forward_src_t o_forward_a,
  output forward_src_t o_forward_b
);

  import cotm32_pkg::*;
  import cotm32_pipeline_pkg::*;

  always_comb begin
    o_forward_a = PIPE_FWD_SRC_NONE;

    // RAW: EX/MEM RD == ID/EX RS1
    if (
      i_exmem_regfile_we &&
      i_exmem_valid &&
      i_exmem_rd_addr != '0 &&
      i_exmem_rd_addr == i_idex_rs1_addr
    ) begin
      o_forward_a = PIPE_FWD_SRC_EXMEM;
    end
    // RAW: MEM/WB RD == ID/EX RS1
    else if (
      i_memwb_regfile_we &&
      i_memwb_valid &&
      i_memwb_rd_addr != '0 &&
      i_memwb_rd_addr == i_idex_rs1_addr
    ) begin
      o_forward_a = PIPE_FWD_SRC_MEMWB;
    end
  end

  always_comb begin
    o_forward_b = PIPE_FWD_SRC_NONE;

    // RAW: EX/MEM RD == ID/EX RS2
    if (
      i_exmem_regfile_we &&
      i_exmem_valid &&
      i_exmem_rd_addr != '0 &&
      i_exmem_rd_addr == i_idex_rs2_addr
    ) begin
      o_forward_b = PIPE_FWD_SRC_EXMEM;
    end
    // RAW: MEM/WB RD == ID/EX RS2
    else if (
      i_memwb_regfile_we &&
      i_memwb_valid &&
      i_memwb_rd_addr != '0 &&
      i_memwb_rd_addr == i_idex_rs2_addr
    ) begin
      o_forward_b = PIPE_FWD_SRC_MEMWB;
    end
  end

endmodule
