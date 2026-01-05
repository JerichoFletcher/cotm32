import cotm32_pkg::*;
import cotm32_priv_pkg::*;

module processor_core (
  input logic i_clk,
  input logic i_rst
);

  localparam REG_ADDR_WIDTH = $clog2(NUM_REGS);

  // Signals
  logic [XLEN-1:0] pc_out;
  logic [INST_WIDTH-1:0] inst;

  imm_t imm_sel;
  logic [XLEN-1:0] imm;
  wire [XLEN-1:0] rs [0:1];
  wire [XLEN-1:0] rs1 = rs[0];
  wire [XLEN-1:0] rs2 = rs[1];

  logic regfile_we;
  logic [REG_ADDR_WIDTH-1:0] rd_addr;
  logic [REG_ADDR_WIDTH-1:0] rs1_addr;
  logic [REG_ADDR_WIDTH-1:0] rs2_addr;
  logic [REG_ADDR_WIDTH-1:0] rs_addr [0:1];

  alu_a_sel_t alu_a_sel;
  alu_b_sel_t alu_b_sel;

  logic [XLEN-1:0] alu_a_vals [0:1];
  logic [XLEN-1:0] alu_b_vals [0:1];

  wire [XLEN-1:0] alu_a_in;
  wire [XLEN-1:0] alu_b_in;
  alu_op_t alu_op;
  logic [XLEN-1:0] alu_out;

  logic bu_be;
  bu_op_t bu_op;
  logic take_branch;

  logic dmem_we;
  logic [XLEN-1:0] dmem_wdata;
  logic [XLEN/BYTE_WIDTH-1:0] dmem_wstrb;
  logic [XLEN-1:0] dmem_rdata;

  lsu_ls_t lsu_ls_op;
  logic [XLEN-1:0] lsu_rdata;

  reg_wb_sel_t reg_wb_sel;
  logic [XLEN-1:0] pc_wb;
  logic [XLEN-1:0] reg_wb;

  logic [XLEN-1:0] reg_wb_vals [0:3];
  logic [XLEN-1:0] pc_wb_vals [0:1];

  wire [XLEN-1:0] pc_4 = pc_out + 4;

  always_comb begin
    rs_addr[0] = rs1_addr;
    rs_addr[1] = rs2_addr;
  end

  always_comb begin
    alu_a_vals[ALU_A_RS1] = rs1;
    alu_a_vals[ALU_A_PC] = pc_out;
  end

  always_comb begin
    alu_b_vals[ALU_B_RS2] = rs2;
    alu_b_vals[ALU_B_IMM] = imm;
  end

  always_comb begin
    reg_wb_vals[REG_WB_ZERO] = '0;
    reg_wb_vals[REG_WB_ALU] = alu_out;
    reg_wb_vals[REG_WB_PC4] = pc_4;
    reg_wb_vals[REG_WB_LSU] = lsu_rdata;
  end

  always_comb begin
    pc_wb_vals[0] = pc_4;
    pc_wb_vals[1] = alu_out;
  end

  // Trap signals
  wire trap_req;
  trap_cause_t trap_cause;
  wire [MXLEN-1:0] trap_tval;

  wire t_illegal_inst;
  wire t_inst_addr_misaligned;
  wire t_ecall_m;
  wire t_ebreak;
  wire t_load_addr_misaligned;
  wire t_store_addr_misaligned;

  // Program counter
  prog_ctr #(
    .RESET_VECTOR(PC_RESET_VECTOR)
  ) pc(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_addr(pc_wb),
    .o_addr(pc_out),
    .o_t_inst_addr_misaligned(t_inst_addr_misaligned)
  );

  // IMEM
  inst_mem #(
    .MEM_SIZE(INST_MEM_SIZE)
  ) im(
    .i_addr(pc_out),
    .o_inst(inst)
  );

  // Immediate sign extender
  sign_ext sext(
    .i_inst(inst),
    .i_sel(imm_sel),
    .o_imm(imm)
  );

  // Register file
  register_file #(
    .N_RPORTS(2),
    .N_REGS(NUM_REGS)
  ) rf(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_we(regfile_we),
    .i_wdata(reg_wb),
    .i_waddr(rd_addr),
    .i_raddr(rs_addr),
    .i_trap_req(trap_req),
    .o_rdata(rs)
  );

  // ALU port A mux
  mux #(
    .N_OPTIONS(2),
    .DATA_WIDTH(XLEN)
  ) mux_alu_a(
    .i_sel(alu_a_sel),
    .i_val(alu_a_vals),
    .o_val(alu_a_in)
  );

  // ALU port B mux
  mux #(
    .N_OPTIONS(2),
    .DATA_WIDTH(XLEN)
  ) mux_alu_b(
    .i_sel(alu_b_sel),
    .i_val(alu_b_vals),
    .o_val(alu_b_in)
  );

  // ALU
  alu alu(
    .i_a(alu_a_in),
    .i_b(alu_b_in),
    .i_op(alu_op),
    .o(alu_out)
  );

  // BU
  bu bu(
    .i_be(bu_be),
    .i_a(rs1),
    .i_b(rs2),
    .i_op(bu_op),
    .o_take(take_branch)
  );

  // CU
  cu cu(
    .i_inst(inst),
    .o_alu_op(alu_op),
    .o_alu_a_sel(alu_a_sel),
    .o_alu_b_sel(alu_b_sel),
    .o_bu_be(bu_be),
    .o_bu_op(bu_op),
    .o_regfile_we(regfile_we),
    .o_rd_addr(rd_addr),
    .o_rs1_addr(rs1_addr),
    .o_rs2_addr(rs2_addr),
    .o_imm_sel(imm_sel),
    .o_lsu_ls(lsu_ls_op),
    .o_reg_wb_sel(reg_wb_sel),
    .o_t_illegal_inst(t_illegal_inst),
    .o_t_ecall_m(t_ecall_m),
    .o_t_ebreak(t_ebreak)
  );

  // DMEM
  data_mem #(
    .MEM_SIZE(DATA_MEM_SIZE),
    .DATA_WIDTH(XLEN)
  ) mem(
    .i_clk(i_clk),
    .i_we(dmem_we),
    .i_addr(alu_out),
    .i_wdata(dmem_wdata),
    .i_wstrb(dmem_wstrb),
    .o_rdata(dmem_rdata)
  );

  // LSU
  lsu lsu(
    .i_op(lsu_ls_op),
    .i_addr(alu_out),
    .i_wdata(rs2),
    .i_rdata(dmem_rdata),
    .i_trap_req(trap_req),
    .o_wdata(dmem_wdata),
    .o_rdata(lsu_rdata),
    .o_wstrb(dmem_wstrb),
    .o_we(dmem_we),
    .o_t_load_addr_misaligned(t_load_addr_misaligned),
    .o_t_store_addr_misaligned(t_store_addr_misaligned)
  );

  // Register writeback mux
  mux #(
    .N_OPTIONS(4),
    .DATA_WIDTH(XLEN)
  ) mux_wb(
    .i_sel(reg_wb_sel),
    .i_val(reg_wb_vals),
    .o_val(reg_wb)
  );

  // PC writeback mux
  mux #(
    .N_OPTIONS(2),
    .DATA_WIDTH(XLEN)
  ) mux_pc(
    .i_sel(take_branch),
    .i_val(pc_wb_vals),
    .o_val(pc_wb)
  );

  // Trap dispatch
  trap_dispatch td(
    .i_pc(pc_out),
    .i_inst(inst),
    .i_ls_addr(alu_out),
    .i_illegal_inst(t_illegal_inst),
    .i_inst_addr_misaligned(t_inst_addr_misaligned),
    .i_ecall_m(t_ecall_m),
    .i_ebreak(t_ebreak),
    .i_load_addr_misaligned(t_load_addr_misaligned),
    .i_store_addr_misaligned(t_store_addr_misaligned),
    .o_trap_req(trap_req),
    .o_trap_cause(trap_cause),
    .o_trap_tval(trap_tval)
  );

endmodule