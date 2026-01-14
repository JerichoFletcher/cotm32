import cotm32_pkg::NUM_REGS;
import cotm32_pkg::lsu_ls_t;

// Hazard detection unit
module hazard_unit (
  input logic [$clog2(NUM_REGS)-1:0] i_id_rs1_addr,
  input logic [$clog2(NUM_REGS)-1:0] i_id_rs2_addr,

  input logic [$clog2(NUM_REGS)-1:0] i_ex_rd_addr,
  input logic i_ex_regfile_we,
  input lsu_ls_t i_ex_lsu_ls_op,
  input logic i_ex_valid,

  input logic i_ex_take_branch,

  input logic i_ex_mu_busy,
  input logic i_ex_mu_done,

  output logic o_stall_ifid,
  output logic o_stall_idex,

  output logic o_flush_ifid,
  output logic o_flush_idex,
  output logic o_flush_exmem
);

  import cotm32_pkg::*;

  wire ex_is_load = (
    i_ex_lsu_ls_op == LSU_LOAD_B ||
    i_ex_lsu_ls_op == LSU_LOAD_H ||
    i_ex_lsu_ls_op == LSU_LOAD_W ||
    i_ex_lsu_ls_op == LSU_LOAD_BU ||
    i_ex_lsu_ls_op == LSU_LOAD_HU
  );
  logic hazard_lu;
  logic lock_ex;

  always_comb begin
    hazard_lu = '0;
    lock_ex = '0;

    // Load-use hazard when:
    // 1. EX current inst is LOAD to RD = RX
    // 2. RX is not reg zero
    // 3. ID inst read RS1 or RS2 matches RX
    if (
      ex_is_load &&
      i_ex_valid &&
      i_ex_regfile_we &&
      i_ex_rd_addr != '0
    ) begin
      if (
        i_ex_rd_addr == i_id_rs1_addr ||
        i_ex_rd_addr == i_id_rs2_addr
      ) begin
        hazard_lu = '1;
      end
    end

    // MU locks EX when busy and not done
    if (
      i_ex_valid &&
      i_ex_mu_busy &&
      !i_ex_mu_done
    ) begin
      lock_ex = '1;
    end
  end

  always_comb begin
    o_stall_ifid = '0;
    o_stall_idex = '0;

    o_flush_ifid = '0;
    o_flush_idex = '0;
    o_flush_exmem = '0;

    // On load-use hazard: stall IF/ID
    // Insert a bubble to prevent double execution
    if (hazard_lu) begin
      o_stall_ifid = '1;
      o_flush_idex = '1;
    end

    // On branch taken: flush invalidated inst in IF and ID
    if (i_ex_take_branch) begin
      o_flush_ifid = '1;
      o_flush_idex = '1;
    end

    // On EX lock: stall all stages before MEM, insert a bubble in MEM
    if (lock_ex) begin
      o_stall_ifid = '1;
      o_stall_idex = '1;
      o_flush_exmem = '1;
    end
  end

endmodule
