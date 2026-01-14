module processor_core (
  input logic i_clk,
  input logic i_rst
);

  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;
  import cotm32_pipeline_pkg::*;

  localparam REG_ADDR_WIDTH = $clog2(NUM_REGS);

  // Signals
  logic [INST_WIDTH-1:0] if_inst /* verilator public */;
  logic [INST_WIDTH-1:0] id_inst /* verilator public */;
  logic [INST_WIDTH-1:0] ex_inst /* verilator public */;
  logic [INST_WIDTH-1:0] mem_inst /* verilator public */;
  logic [INST_WIDTH-1:0] wb_inst /* verilator public */;
  logic [XLEN-1:0] if_pc /* verilator public */;
  logic [XLEN-1:0] id_pc /* verilator public */;
  logic [XLEN-1:0] ex_pc /* verilator public */;
  logic [XLEN-1:0] mem_pc /* verilator public */;
  logic [XLEN-1:0] wb_pc /* verilator public */;
  wire [XLEN-1:0] if_pc_4 /* verilator public */;
  wire [XLEN-1:0] id_pc_4 /* verilator public */;
  wire [XLEN-1:0] ex_pc_4 /* verilator public */;
  wire [XLEN-1:0] mem_pc_4 /* verilator public */;
  wire [XLEN-1:0] wb_pc_4 /* verilator public */;

  imm_t id_imm_sel /* verilator public */;
  logic [XLEN-1:0] id_imm /* verilator public */;
  logic [XLEN-1:0] ex_imm /* verilator public */;
  wire [XLEN-1:0] id_rs1 /* verilator public */;
  wire [XLEN-1:0] ex_rs1 /* verilator public */;
  wire [XLEN-1:0] mem_rs1 /* verilator public */;
  wire [XLEN-1:0] id_rs2 /* verilator public */;
  wire [XLEN-1:0] ex_rs2 /* verilator public */;
  wire [XLEN-1:0] mem_rs2 /* verilator public */;

  logic id_regfile_we /* verilator public */;
  logic ex_regfile_we /* verilator public */;
  logic mem_regfile_we /* verilator public */;
  logic wb_regfile_we /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] id_rd_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] id_rs1_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] id_rs2_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] ex_rd_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] ex_rs1_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] ex_rs2_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] mem_rd_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] wb_rd_addr /* verilator public */;

  alu_a_sel_t id_alu_a_sel /* verilator public */;
  alu_b_sel_t id_alu_b_sel /* verilator public */;
  alu_a_sel_t ex_alu_a_sel /* verilator public */;
  alu_b_sel_t ex_alu_b_sel /* verilator public */;

  logic [XLEN-1:0] ex_alu_a_vals [0:1];
  logic [XLEN-1:0] ex_alu_b_vals [0:1];

  alu_op_t id_alu_op /* verilator public */;
  alu_op_t ex_alu_op /* verilator public */;
  wire [XLEN-1:0] ex_alu_a_in /* verilator public */;
  wire [XLEN-1:0] ex_alu_b_in /* verilator public */;
  logic [XLEN-1:0] ex_alu_out /* verilator public */;
  logic [XLEN-1:0] mem_alu_out /* verilator public */;
  logic [XLEN-1:0] wb_alu_out /* verilator public */;

  logic id_bu_be /* verilator public */;
  logic ex_bu_be /* verilator public */;
  bu_op_t id_bu_op /* verilator public */;
  bu_op_t ex_bu_op /* verilator public */;
  logic ex_take_branch /* verilator public */;

  mu_op_t id_mu_op /* verilator public */;
  mu_op_t ex_mu_op /* verilator public */;
  wire ex_mu_busy /* verilator public */;
  wire ex_mu_done /* verilator public */;
  logic [XLEN-1:0] ex_mu_out /* verilator public */;
  logic [XLEN-1:0] mem_mu_out /* verilator public */;
  logic [XLEN-1:0] wb_mu_out /* verilator public */;

  logic mem_dmem_we /* verilator public */;
  logic [XLEN-1:0] mem_dmem_wdata /* verilator public */;
  logic [XLEN/BYTE_WIDTH-1:0] mem_dmem_wstrb /* verilator public */;
  logic [XLEN-1:0] mem_dmem_rdata /* verilator public */;

  logic [XLEN-1:0] mem_rom_rdata /* verilator public */;

  lsu_ls_t id_lsu_ls_op /* verilator public */;
  lsu_ls_t ex_lsu_ls_op /* verilator public */;
  lsu_ls_t mem_lsu_ls_op /* verilator public */;
  logic [XLEN-1:0] mem_lsu_addr /* verilator public */;
  logic [XLEN-1:0] mem_lsu_rdata /* verilator public */;
  logic [XLEN-1:0] wb_lsu_rdata /* verilator public */;

  reg_wb_sel_t id_reg_wb_sel /* verilator public */;
  reg_wb_sel_t ex_reg_wb_sel /* verilator public */;
  reg_wb_sel_t mem_reg_wb_sel /* verilator public */;
  reg_wb_sel_t wb_reg_wb_sel /* verilator public */;
  wire [XLEN-1:0] mem_reg_wb /* verilator public */;
  wire [XLEN-1:0] wb_reg_wb /* verilator public */;

  logic [XLEN-1:0] mem_reg_wb_vals [0:REG_WB_VALCOUNT-1];
  logic [XLEN-1:0] wb_reg_wb_vals [0:REG_WB_VALCOUNT-1];

  always_comb begin
    ex_alu_a_vals[ALU_A_RS1] = ex_rs1_fwd;
    ex_alu_a_vals[ALU_A_PC] = ex_pc;

    ex_alu_b_vals[ALU_B_RS2] = ex_rs2_fwd;
    ex_alu_b_vals[ALU_B_IMM] = ex_imm;

    mem_reg_wb_vals[REG_WB_ZERO] = '0;
    mem_reg_wb_vals[REG_WB_ALU] = mem_alu_out;
    mem_reg_wb_vals[REG_WB_MU] = mem_mu_out;
    mem_reg_wb_vals[REG_WB_PC4] = mem_pc_4;
    mem_reg_wb_vals[REG_WB_LSU] = mem_lsu_rdata;
    mem_reg_wb_vals[REG_WB_CSR] = mem_csr_rdata;

    wb_reg_wb_vals[REG_WB_ZERO] = '0;
    wb_reg_wb_vals[REG_WB_ALU] = wb_alu_out;
    wb_reg_wb_vals[REG_WB_MU] = wb_mu_out;
    wb_reg_wb_vals[REG_WB_PC4] = wb_pc_4;
    wb_reg_wb_vals[REG_WB_LSU] = wb_lsu_rdata;
    wb_reg_wb_vals[REG_WB_CSR] = wb_csr_rdata;
  end

  // Trap signals
  trap_cause_t trap_cause;
  wire [MXLEN-1:0] trap_tval;

  wire trap_mret /* verilator public */;
  wire trap_req /* verilator public */;

  wire id_trap_mret;
  wire ex_trap_mret;
  wire mem_trap_mret;

  wire if_t_inst_addr_misaligned;
  wire if_t_inst_access_fault;
  
  wire id_t_inst_addr_misaligned;
  wire id_t_inst_access_fault;
  wire id_t_illegal_inst;
  wire id_t_ebreak;
  wire id_t_ecall_m;

  wire ex_t_inst_addr_misaligned;
  wire ex_t_inst_access_fault;
  wire ex_t_illegal_inst;
  wire ex_t_ebreak;
  wire ex_t_ecall_m;

  wire mem_t_inst_addr_misaligned;
  wire mem_t_inst_access_fault;
  wire mem_t_illegal_inst;
  wire mem_t_illegal_inst_csr;
  wire mem_t_ebreak;
  wire mem_t_load_addr_misaligned;
  wire mem_t_load_access_fault;
  wire mem_t_store_addr_misaligned;
  wire mem_t_store_access_fault;
  wire mem_t_ecall_m;

  wire trap_mode /* verilator public */;

  // CSR wires
  wire id_csr_we /* verilator public */;
  wire ex_csr_we /* verilator public */;
  wire mem_csr_we /* verilator public */;
  zicsr_data_sel_t id_csr_data_sel /* verilator public */;
  zicsr_data_sel_t ex_csr_data_sel /* verilator public */;
  zicsr_data_sel_t mem_csr_data_sel /* verilator public */;
  zicsr_csr_addr_t id_csr_addr /* verilator public */;
  zicsr_csr_addr_t ex_csr_addr /* verilator public */;
  zicsr_csr_addr_t mem_csr_addr /* verilator public */;
  zicsr_csr_op_t id_csr_op /* verilator public */;
  zicsr_csr_op_t ex_csr_op /* verilator public */;
  zicsr_csr_op_t mem_csr_op /* verilator public */;
  logic [MXLEN-1:0] id_csr_zimm /* verilator public */;
  logic [MXLEN-1:0] ex_csr_zimm /* verilator public */;
  logic [MXLEN-1:0] mem_csr_zimm /* verilator public */;
  logic [MXLEN-1:0] mem_csr_wdata;

  logic [MXLEN-1:0] mem_csr_wdata_vals [0:ZICSR_DATA_VALCOUNT-1];

  logic [MXLEN-1:0] mem_csr_rdata /* verilator public */;
  logic [MXLEN-1:0] wb_csr_rdata /* verilator public */;
  wire [MXLEN-1:0] mem_csr_mtvec;
  wire [MXLEN-1:0] mem_csr_mepc;

  always_comb begin
    mem_csr_wdata_vals[ZICSR_DATA_RS1] = mem_rs1;
    mem_csr_wdata_vals[ZICSR_DATA_IMM] = mem_csr_zimm;
  end

  // Pipeline wires
  wire ifid_valid /* verilator public */;
  wire ifid_stall /* verilator public */;
  wire ifid_flush /* verilator public */;

  wire idex_valid /* verilator public */;
  wire idex_stall /* verilator public */;
  wire idex_flush /* verilator public */;

  wire exmem_valid /* verilator public */;
  wire exmem_stall /* verilator public */;
  wire exmem_flush /* verilator public */;

  wire memwb_valid /* verilator public */;
  wire memwb_stall /* verilator public */;
  wire memwb_flush /* verilator public */;

  wire ifid_stall_hu;
  wire idex_stall_hu;

  wire ifid_flush_hu;
  wire idex_flush_hu;
  wire exmem_flush_hu;

  assign ifid_stall = ifid_stall_hu;
  assign idex_stall = idex_stall_hu;

  assign ifid_flush = ifid_flush_hu || trap_req || trap_mret;
  assign idex_flush = idex_flush_hu || trap_req || trap_mret;
  assign exmem_flush = exmem_flush_hu || trap_req || trap_mret;

  forward_src_t forward_a /* verilator public */;
  forward_src_t forward_b /* verilator public */;
  
  logic [XLEN-1:0] forward_a_vals [0:2];
  logic [XLEN-1:0] forward_b_vals [0:2];
  wire [XLEN-1:0] ex_rs1_fwd /* verilator public */;
  wire [XLEN-1:0] ex_rs2_fwd /* verilator public */;

  always_comb begin
    forward_a_vals[PIPE_FWD_SRC_NONE] = ex_rs1;
    forward_a_vals[PIPE_FWD_SRC_EXMEM] = mem_reg_wb;
    forward_a_vals[PIPE_FWD_SRC_MEMWB] = wb_reg_wb;

    forward_b_vals[PIPE_FWD_SRC_NONE] = ex_rs2;
    forward_b_vals[PIPE_FWD_SRC_EXMEM] = mem_reg_wb;
    forward_b_vals[PIPE_FWD_SRC_MEMWB] = wb_reg_wb;
  end

  //////////////////////////////// IF     ////////////////////////////////
  // Instruction fetch unit
  inst_fetch #(
    .RESET_VECTOR(PC_RESET_VECTOR)
  ) ifu(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_take_branch(ex_take_branch && idex_valid),
    .i_new_addr(ex_alu_out),

    .i_stall(ifid_stall),

    .i_trap_mret(trap_mret),
    .i_trap_req(trap_req),
    .i_mtvec(mem_csr_mtvec),
    .i_mepc(mem_csr_mepc),

    .o_addr(if_pc),
    .o_addr_4(if_pc_4),
    .o_t_inst_addr_misaligned(if_t_inst_addr_misaligned),
    .o_t_inst_access_fault(if_t_inst_access_fault)
  );

  // IMEM
  inst_mem im(
    .i_addr(if_pc),
    .o_inst(if_inst)
  );

  //////////////////////////////// IF/ID  ////////////////////////////////
  ifid_reg ifid(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_stall(ifid_stall),
    .i_flush(ifid_flush),

    .i_data('{if_pc, if_pc_4, if_inst}),
    .i_t_inst_addr_misaligned(if_t_inst_addr_misaligned),
    .i_t_inst_access_fault(if_t_inst_access_fault),

    .o_data('{id_pc, id_pc_4, id_inst}),
    .o_valid(ifid_valid),
    .o_t_inst_addr_misaligned(id_t_inst_addr_misaligned),
    .o_t_inst_access_fault(id_t_inst_access_fault)
  );

  //////////////////////////////// ID     ////////////////////////////////
  // CU
  cu cu(
    .i_inst(id_inst),
    .i_trap_mode(trap_mode),

    .o_alu_op(id_alu_op),
    .o_alu_a_sel(id_alu_a_sel),
    .o_alu_b_sel(id_alu_b_sel),
    .o_bu_be(id_bu_be),
    .o_bu_op(id_bu_op),
    .o_regfile_we(id_regfile_we),
    .o_rd_addr(id_rd_addr),
    .o_rs1_addr(id_rs1_addr),
    .o_rs2_addr(id_rs2_addr),
    .o_imm_sel(id_imm_sel),
    .o_lsu_ls(id_lsu_ls_op),
    .o_reg_wb_sel(id_reg_wb_sel),

    .o_mu_op(id_mu_op),

    .o_csr_we(id_csr_we),
    .o_csr_data_sel(id_csr_data_sel),
    .o_csr_addr(id_csr_addr),
    .o_csr_op(id_csr_op),
    .o_csr_zimm(id_csr_zimm),

    .o_t_illegal_inst(id_t_illegal_inst),
    .o_t_ebreak(id_t_ebreak),
    .o_t_ecall_m(id_t_ecall_m),
    .o_trap_mret(id_trap_mret)
  );

  // Immediate sign extender
  sign_ext sext(
    .i_inst(id_inst),
    .i_sel(id_imm_sel),
    .o_imm(id_imm)
  );

  // Register file
  register_file rf(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_we(wb_regfile_we && memwb_valid),
    .i_wdata(wb_reg_wb),
    .i_waddr(wb_rd_addr),
    .i_rs1_addr(id_rs1_addr),
    .i_rs2_addr(id_rs2_addr),
    .o_rs1(id_rs1),
    .o_rs2(id_rs2)
  );

  //////////////////////////////// ID/EX  ////////////////////////////////
  idex_reg idex(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_stall(idex_stall),
    .i_flush(idex_flush),
    .i_data('{
      id_alu_op,
      id_alu_a_sel,
      id_alu_b_sel,
      id_bu_be,
      id_bu_op,
      id_regfile_we,
      id_rd_addr,
      id_rs1_addr,
      id_rs2_addr,
      id_lsu_ls_op,
      id_reg_wb_sel,
      id_mu_op,
      id_csr_we,
      id_csr_data_sel,
      id_csr_addr,
      id_csr_op,
      id_csr_zimm,
      id_rs1,
      id_rs2,
      id_imm,
      id_pc,
      id_pc_4,
      id_inst
    }),
    .i_valid(ifid_valid),
    .i_t_inst_addr_misaligned(id_t_inst_addr_misaligned),
    .i_t_inst_access_fault(id_t_inst_access_fault),
    .i_t_illegal_inst(id_t_illegal_inst),
    .i_t_ebreak(id_t_ebreak),
    .i_t_ecall_m(id_t_ecall_m),
    .i_trap_mret(id_trap_mret),
    .o_data('{
      ex_alu_op,
      ex_alu_a_sel,
      ex_alu_b_sel,
      ex_bu_be,
      ex_bu_op,
      ex_regfile_we,
      ex_rd_addr,
      ex_rs1_addr,
      ex_rs2_addr,
      ex_lsu_ls_op,
      ex_reg_wb_sel,
      ex_mu_op,
      ex_csr_we,
      ex_csr_data_sel,
      ex_csr_addr,
      ex_csr_op,
      ex_csr_zimm,
      ex_rs1,
      ex_rs2,
      ex_imm,
      ex_pc,
      ex_pc_4,
      ex_inst
    }),
    .o_valid(idex_valid),
    .o_t_inst_addr_misaligned(ex_t_inst_addr_misaligned),
    .o_t_inst_access_fault(ex_t_inst_access_fault),
    .o_t_illegal_inst(ex_t_illegal_inst),
    .o_t_ebreak(ex_t_ebreak),
    .o_t_ecall_m(ex_t_ecall_m),
    .o_trap_mret(ex_trap_mret)
  );

  //////////////////////////////// EX     ////////////////////////////////
  // RS1 forward mux
  mux #(
    .N_OPTIONS(3),
    .DATA_WIDTH(XLEN)
  ) mux_rs1_fwd(
    .i_sel(forward_a),
    .i_val(forward_a_vals),
    .o_val(ex_rs1_fwd)
  );

  // RS2 forward mux
  mux #(
    .N_OPTIONS(3),
    .DATA_WIDTH(XLEN)
  ) mux_rs2_fwd(
    .i_sel(forward_b),
    .i_val(forward_b_vals),
    .o_val(ex_rs2_fwd)
  );

  // ALU port A mux
  mux #(
    .N_OPTIONS(2),
    .DATA_WIDTH(XLEN)
  ) mux_alu_a(
    .i_sel(ex_alu_a_sel),
    .i_val(ex_alu_a_vals),
    .o_val(ex_alu_a_in)
  );

  // ALU port B mux
  mux #(
    .N_OPTIONS(2),
    .DATA_WIDTH(XLEN)
  ) mux_alu_b(
    .i_sel(ex_alu_b_sel),
    .i_val(ex_alu_b_vals),
    .o_val(ex_alu_b_in)
  );

  // ALU
  alu alu(
    .i_a(ex_alu_a_in),
    .i_b(ex_alu_b_in),
    .i_op(ex_alu_op),
    .o(ex_alu_out)
  );

  // BU
  bu bu(
    .i_be(ex_bu_be),
    .i_a(ex_rs1_fwd),
    .i_b(ex_rs2_fwd),
    .i_op(ex_bu_op),
    .o_take(ex_take_branch)
  );

  // MU
  mu mu(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_op(ex_mu_op),
    .i_a(ex_rs1_fwd),
    .i_b(ex_rs2_fwd),
    .o(ex_mu_out),
    .o_busy(ex_mu_busy),
    .o_done(ex_mu_done)
  );

  //////////////////////////////// EX/MEM ////////////////////////////////
  exmem_reg exmem(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_stall(exmem_stall),
    .i_flush(exmem_flush),
    .i_data('{
      ex_alu_out,
      ex_rs1_fwd,
      ex_rs2_fwd,
      ex_regfile_we,
      ex_rd_addr,
      ex_lsu_ls_op,
      ex_reg_wb_sel,
      ex_mu_out,
      ex_csr_we,
      ex_csr_data_sel,
      ex_csr_addr,
      ex_csr_op,
      ex_csr_zimm,
      ex_pc,
      ex_pc_4,
      ex_inst
    }),
    .i_valid(idex_valid),
    .i_t_inst_addr_misaligned(ex_t_inst_addr_misaligned),
    .i_t_inst_access_fault(ex_t_inst_access_fault),
    .i_t_illegal_inst(ex_t_illegal_inst),
    .i_t_ebreak(ex_t_ebreak),
    .i_t_ecall_m(ex_t_ecall_m),
    .i_trap_mret(ex_trap_mret),
    .o_data('{
      mem_alu_out,
      mem_rs1,
      mem_rs2,
      mem_regfile_we,
      mem_rd_addr,
      mem_lsu_ls_op,
      mem_reg_wb_sel,
      mem_mu_out,
      mem_csr_we,
      mem_csr_data_sel,
      mem_csr_addr,
      mem_csr_op,
      mem_csr_zimm,
      mem_pc,
      mem_pc_4,
      mem_inst
    }),
    .o_valid(exmem_valid),
    .o_t_inst_addr_misaligned(mem_t_inst_addr_misaligned),
    .o_t_inst_access_fault(mem_t_inst_access_fault),
    .o_t_illegal_inst(mem_t_illegal_inst),
    .o_t_ebreak(mem_t_ebreak),
    .o_t_ecall_m(mem_t_ecall_m),
    .o_trap_mret(mem_trap_mret)
  );

  //////////////////////////////// MEM    ////////////////////////////////
  // DMEM
  data_mem #(
    .MEM_SIZE(DATA_MEM_SIZE),
    .DATA_WIDTH(XLEN)
  ) dmem(
    .i_clk(i_clk),
    .i_we(mem_dmem_we && exmem_valid && !trap_req),
    .i_addr(mem_lsu_addr),
    .i_wdata(mem_dmem_wdata),
    .i_wstrb(mem_dmem_wstrb),
    .o_rdata(mem_dmem_rdata)
  );

  // ROM
  rodata_mem #(
    .DATA_WIDTH(XLEN)
  ) rom(
    .i_addr(mem_lsu_addr),
    .o_rdata(mem_rom_rdata)
  );

  // LSU
  lsu lsu(
    .i_op(mem_lsu_ls_op),
    .i_addr(mem_alu_out),
    .i_wdata(mem_rs2),
    .i_rdata_dmem(mem_dmem_rdata),
    .i_rdata_rom(mem_rom_rdata),
    .i_trap_req(trap_req),
    .o_addr(mem_lsu_addr),
    .o_wdata(mem_dmem_wdata),
    .o_rdata(mem_lsu_rdata),
    .o_wstrb(mem_dmem_wstrb),
    .o_we_dmem(mem_dmem_we),
    .o_t_load_addr_misaligned(mem_t_load_addr_misaligned),
    .o_t_load_access_fault(mem_t_load_access_fault),
    .o_t_store_addr_misaligned(mem_t_store_addr_misaligned),
    .o_t_store_access_fault(mem_t_store_access_fault)
  );

  // CSR write data mux
  mux #(
    .N_OPTIONS(ZICSR_DATA_VALCOUNT),
    .DATA_WIDTH(MXLEN)
  ) mux_csr(
    .i_sel(mem_csr_data_sel),
    .i_val(mem_csr_wdata_vals),
    .o_val(mem_csr_wdata)
  );

  // CSR file
  csr_file csr(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_we(mem_csr_we && exmem_valid),
    .i_op(mem_csr_op),
    .i_addr(mem_csr_addr),
    .i_wdata(mem_csr_wdata),
    .i_pc(mem_pc),

    .i_trap_req(trap_req),
    .i_trap_cause(trap_cause),
    .i_trap_tval(trap_tval),

    .o_rdata(mem_csr_rdata),
    .o_mtvec(mem_csr_mtvec),
    .o_mepc(mem_csr_mepc),

    .o_t_illegal_inst(mem_t_illegal_inst_csr)
  );

  // Early writeback mux (only for forwarding)
  mux #(
    .N_OPTIONS(REG_WB_VALCOUNT),
    .DATA_WIDTH(XLEN)
  ) mem_mux_wb(
    .i_sel(mem_reg_wb_sel),
    .i_val(mem_reg_wb_vals),
    .o_val(mem_reg_wb)
  );

  // Trap dispatch
  trap_dispatch td(
    .i_pc(mem_pc),
    .i_inst(mem_inst),
    .i_ls_addr(mem_alu_out),
    .i_inst_addr_misaligned(mem_t_inst_addr_misaligned),
    .i_inst_access_fault(mem_t_inst_access_fault),
    .i_illegal_inst(mem_t_illegal_inst || mem_t_illegal_inst_csr),
    .i_ebreak(mem_t_ebreak),
    .i_load_addr_misaligned(mem_t_load_addr_misaligned),
    .i_load_access_fault(mem_t_load_access_fault),
    .i_store_addr_misaligned(mem_t_store_addr_misaligned),
    .i_store_access_fault(mem_t_store_access_fault),
    .i_ecall_m(mem_t_ecall_m),
    .o_trap_req(trap_req),
    .o_trap_cause(trap_cause),
    .o_trap_tval(trap_tval)
  );

  // Trap control unit
  trap_control tc(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_trap_mret(mem_trap_mret),
    .i_trap_req(trap_req),
    .o_trap_mode(trap_mode),
    .o_trap_mret(trap_mret)
  );

  //////////////////////////////// MEM/WB ////////////////////////////////
  memwb_reg memwb(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_stall(memwb_stall),
    .i_flush(memwb_flush),
    .i_data('{
      mem_lsu_rdata,
      mem_alu_out,
      mem_csr_rdata,
      mem_regfile_we,
      mem_rd_addr,
      mem_reg_wb_sel,
      mem_mu_out,
      mem_pc,
      mem_pc_4,
      mem_inst
    }),
    .i_valid(exmem_valid && ~trap_req),
    .o_data('{
      wb_lsu_rdata,
      wb_alu_out,
      wb_csr_rdata,
      wb_regfile_we,
      wb_rd_addr,
      wb_reg_wb_sel,
      wb_mu_out,
      wb_pc,
      wb_pc_4,
      wb_inst
    }),
    .o_valid(memwb_valid)
  );

  //////////////////////////////// WB     ////////////////////////////////
  // Register writeback mux
  mux #(
    .N_OPTIONS(REG_WB_VALCOUNT),
    .DATA_WIDTH(XLEN)
  ) wb_mux_wb(
    .i_sel(wb_reg_wb_sel),
    .i_val(wb_reg_wb_vals),
    .o_val(wb_reg_wb)
  );

  //////////////////////////////// PIPE   ////////////////////////////////
  // Forwarding unit
  fwd_unit fu(
    .i_idex_rs1_addr(ex_rs1_addr),
    .i_idex_rs2_addr(ex_rs2_addr),
    .i_exmem_rd_addr(mem_rd_addr),
    .i_exmem_regfile_we(mem_regfile_we),
    .i_exmem_valid(exmem_valid),
    .i_memwb_rd_addr(wb_rd_addr),
    .i_memwb_regfile_we(wb_regfile_we),
    .i_memwb_valid(memwb_valid),
    .o_forward_a(forward_a),
    .o_forward_b(forward_b)
  );

  // Hazard detection unit
  hazard_unit hu(
    .i_id_rs1_addr(id_rs1_addr),
    .i_id_rs2_addr(id_rs2_addr),
    .i_ex_rd_addr(ex_rd_addr),
    .i_ex_regfile_we(ex_regfile_we),
    .i_ex_lsu_ls_op(ex_lsu_ls_op),
    .i_ex_valid(idex_valid),
    .i_ex_take_branch(ex_take_branch && idex_valid),
    .i_ex_mu_busy(ex_mu_busy),
    .i_ex_mu_done(ex_mu_done),
    .o_stall_ifid(ifid_stall_hu),
    .o_stall_idex(idex_stall_hu),
    .o_flush_ifid(ifid_flush_hu),
    .o_flush_idex(idex_flush_hu),
    .o_flush_exmem(exmem_flush_hu)
  );

endmodule
