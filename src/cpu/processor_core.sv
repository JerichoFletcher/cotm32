import cotm32_pkg::*;
import cotm32_priv_pkg::*;

module processor_core (
  input logic i_clk,
  input logic i_rst
);

  localparam REG_ADDR_WIDTH = $clog2(NUM_REGS);

  // Signals
  logic [XLEN-1:0] pc;
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

  logic [XLEN-1:0] rom_rdata;

  lsu_ls_t lsu_ls_op;
  logic [XLEN-1:0] lsu_addr;
  logic [XLEN-1:0] lsu_rdata;

  reg_wb_sel_t reg_wb_sel;
  wire [XLEN-1:0] reg_wb;

  logic [XLEN-1:0] reg_wb_vals [0:REG_WB_VALCOUNT-1];
  wire [XLEN-1:0] pc_4;

  always_comb begin
    rs_addr[0] = rs1_addr;
    rs_addr[1] = rs2_addr;
  end

  always_comb begin
    alu_a_vals[ALU_A_RS1] = rs1;
    alu_a_vals[ALU_A_PC] = pc;
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
    reg_wb_vals[REG_WB_CSR] = csr_rdata;
  end

  // Trap signals
  wire trap_mret;
  wire trap_req;
  trap_cause_t trap_cause;
  wire [MXLEN-1:0] trap_tval;

  wire cu_t_illegal_inst;
  wire csr_t_illegal_inst;

  wire t_illegal_inst = cu_t_illegal_inst | csr_t_illegal_inst;
  wire t_inst_addr_misaligned;
  wire t_ecall_m;
  wire t_ebreak;
  wire t_load_addr_misaligned;
  wire t_store_addr_misaligned;

  wire trap_mode;

  // CSR wires
  wire csr_we;
  zicsr_data_sel_t csr_data_sel;
  zicsr_csr_addr_t csr_addr;
  zicsr_csr_op_t csr_op;
  logic [MXLEN-1:0] csr_zimm;
  logic [MXLEN-1:0] csr_wdata;

  logic [MXLEN-1:0] csr_wdata_vals [0:ZICSR_DATA_VALCOUNT-1];

  logic [MXLEN-1:0] csr_rdata;
  wire [MXLEN-1:0] csr_mtvec;
  wire [MXLEN-1:0] csr_mepc;

  always_comb begin
    csr_wdata_vals[ZICSR_DATA_RS1] = rs1;
    csr_wdata_vals[ZICSR_DATA_IMM] = csr_zimm;
  end

  // Instruction fetch unit
  inst_fetch #(
    .RESET_VECTOR(PC_RESET_VECTOR)
  ) ifu(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_take_branch(take_branch),
    .i_new_addr(alu_out),

    .i_trap_mret(trap_mret),
    .i_trap_req(trap_req),
    .i_mtvec(csr_mtvec),
    .i_mepc(csr_mepc),

    .o_addr(pc),
    .o_addr_4(pc_4),
    .o_t_inst_addr_misaligned(t_inst_addr_misaligned)
  );

  // IMEM
  inst_mem im(
    .i_addr(pc),
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
    .i_trap_mode(trap_mode),

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

    .o_csr_we(csr_we),
    .o_csr_data_sel(csr_data_sel),
    .o_csr_addr(csr_addr),
    .o_csr_op(csr_op),
    .o_csr_zimm(csr_zimm),

    .o_t_illegal_inst(cu_t_illegal_inst),
    .o_t_ecall_m(t_ecall_m),
    .o_t_ebreak(t_ebreak),
    .o_trap_mret(trap_mret)
  );

  // DMEM
  data_mem #(
    .MEM_SIZE(DATA_MEM_SIZE),
    .DATA_WIDTH(XLEN)
  ) mem(
    .i_clk(i_clk),
    .i_we(dmem_we),
    .i_addr(lsu_addr),
    .i_wdata(dmem_wdata),
    .i_wstrb(dmem_wstrb),
    .o_rdata(dmem_rdata)
  );

  // ROM
  rodata_mem #(
    .DATA_WIDTH(XLEN)
  ) rom(
    .i_addr(lsu_addr),
    .o_rdata(rom_rdata)
  );

  // LSU
  lsu lsu(
    .i_op(lsu_ls_op),
    .i_addr(alu_out),
    .i_wdata(rs2),
    .i_rdata_dmem(dmem_rdata),
    .i_rdata_rom(rom_rdata),
    .i_trap_req(trap_req),
    .o_addr(lsu_addr),
    .o_wdata(dmem_wdata),
    .o_rdata(lsu_rdata),
    .o_wstrb(dmem_wstrb),
    .o_we_dmem(dmem_we),
    .o_t_load_addr_misaligned(t_load_addr_misaligned),
    .o_t_store_addr_misaligned(t_store_addr_misaligned)
  );

  // Register writeback mux
  mux #(
    .N_OPTIONS(REG_WB_VALCOUNT),
    .DATA_WIDTH(XLEN)
  ) mux_wb(
    .i_sel(reg_wb_sel),
    .i_val(reg_wb_vals),
    .o_val(reg_wb)
  );

  // Trap dispatch
  trap_dispatch td(
    .i_pc(pc),
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

  // Trap control unit
  trap_control tc(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_trap_mret(trap_mret),
    .i_trap_req(trap_req),
    .o_trap_mode(trap_mode)
  );

  // CSR write data mux
  mux #(
    .N_OPTIONS(ZICSR_DATA_VALCOUNT),
    .DATA_WIDTH(MXLEN)
  ) mux_csr(
    .i_sel(csr_data_sel),
    .i_val(csr_wdata_vals),
    .o_val(csr_wdata)
  );

  // CSR file
  csr_file #(
    .CSR_REG_WIDTH(MXLEN)
  ) csr(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_we(csr_we),
    .i_op(csr_op),
    .i_addr(csr_addr),
    .i_wdata(csr_wdata),
    .i_pc(pc),

    .i_trap_req(trap_req),
    .i_trap_cause(trap_cause),
    .i_trap_tval(trap_tval),

    .o_rdata(csr_rdata),
    .o_mtvec(csr_mtvec),
    .o_mepc(csr_mepc),

    .o_t_illegal_inst(csr_t_illegal_inst)
  );

endmodule