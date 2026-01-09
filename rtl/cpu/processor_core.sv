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
  logic [XLEN-1:0] if_pc /* verilator public */;
  logic [XLEN-1:0] id_pc /* verilator public */;
  logic [XLEN-1:0] ex_pc /* verilator public */;
  wire [XLEN-1:0] if_pc_4 /* verilator public */;
  wire [XLEN-1:0] id_pc_4 /* verilator public */;
  wire [XLEN-1:0] ex_pc_4 /* verilator public */;

  imm_t id_imm_sel /* verilator public */;
  logic [XLEN-1:0] id_imm /* verilator public */;
  logic [XLEN-1:0] ex_imm /* verilator public */;
  wire [XLEN-1:0] id_rs1 /* verilator public */;
  wire [XLEN-1:0] ex_rs1 /* verilator public */;
  wire [XLEN-1:0] id_rs2 /* verilator public */;
  wire [XLEN-1:0] ex_rs2 /* verilator public */;

  logic id_regfile_we /* verilator public */;
  logic ex_regfile_we /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] id_rd_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] id_rs1_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] id_rs2_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] ex_rd_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] ex_rs1_addr /* verilator public */;
  logic [REG_ADDR_WIDTH-1:0] ex_rs2_addr /* verilator public */;

  alu_a_sel_t id_alu_a_sel /* verilator public */;
  alu_b_sel_t id_alu_b_sel /* verilator public */;
  alu_a_sel_t ex_alu_a_sel /* verilator public */;
  alu_b_sel_t ex_alu_b_sel /* verilator public */;

  logic [XLEN-1:0] alu_a_vals [0:1];
  logic [XLEN-1:0] alu_b_vals [0:1];

  alu_op_t id_alu_op /* verilator public */;
  alu_op_t ex_alu_op /* verilator public */;
  wire [XLEN-1:0] alu_a_in;
  wire [XLEN-1:0] alu_b_in;
  logic [XLEN-1:0] alu_out;

  logic id_bu_be /* verilator public */;
  logic ex_bu_be /* verilator public */;
  bu_op_t id_bu_op /* verilator public */;
  bu_op_t ex_bu_op /* verilator public */;
  logic take_branch;

  logic dmem_we;
  logic [XLEN-1:0] dmem_wdata;
  logic [XLEN/BYTE_WIDTH-1:0] dmem_wstrb;
  logic [XLEN-1:0] dmem_rdata;

  logic [XLEN-1:0] rom_rdata;

  lsu_ls_t id_lsu_ls_op /* verilator public */;
  lsu_ls_t ex_lsu_ls_op /* verilator public */;
  logic [XLEN-1:0] lsu_addr;
  logic [XLEN-1:0] lsu_rdata;

  reg_wb_sel_t id_reg_wb_sel /* verilator public */;
  reg_wb_sel_t ex_reg_wb_sel /* verilator public */;
  wire [XLEN-1:0] reg_wb;

  logic [XLEN-1:0] reg_wb_vals [0:REG_WB_VALCOUNT-1];

  always_comb begin
    alu_a_vals[ALU_A_RS1] = id_rs1;
    alu_a_vals[ALU_A_PC] = id_pc;
  end

  always_comb begin
    alu_b_vals[ALU_B_RS2] = id_rs2;
    alu_b_vals[ALU_B_IMM] = id_imm;
  end

  always_comb begin
    reg_wb_vals[REG_WB_ZERO] = '0;
    reg_wb_vals[REG_WB_ALU] = alu_out;
    reg_wb_vals[REG_WB_PC4] = if_pc_4;
    reg_wb_vals[REG_WB_LSU] = lsu_rdata;
    reg_wb_vals[REG_WB_CSR] = csr_rdata;
  end

  // Trap signals
  wire trap_mret /* verilator public */;
  wire trap_req /* verilator public */;
  trap_cause_t trap_cause;
  wire [MXLEN-1:0] trap_tval;

  wire cu_t_illegal_inst;
  wire csr_t_illegal_inst;

  wire t_inst_addr_misaligned;
  wire t_inst_access_fault;
  wire t_illegal_inst = cu_t_illegal_inst | csr_t_illegal_inst;
  wire t_ebreak;
  wire t_load_addr_misaligned;
  wire t_load_access_fault;
  wire t_store_addr_misaligned;
  wire t_store_access_fault;
  wire t_ecall_m;

  wire trap_mode /* verilator public */;

  // CSR wires
  wire id_csr_we /* verilator public */;
  wire ex_csr_we /* verilator public */;
  zicsr_data_sel_t id_csr_data_sel /* verilator public */;
  zicsr_data_sel_t ex_csr_data_sel /* verilator public */;
  zicsr_csr_addr_t id_csr_addr /* verilator public */;
  zicsr_csr_addr_t ex_csr_addr /* verilator public */;
  zicsr_csr_op_t id_csr_op /* verilator public */;
  zicsr_csr_op_t ex_csr_op /* verilator public */;
  logic [MXLEN-1:0] id_csr_zimm /* verilator public */;
  logic [MXLEN-1:0] ex_csr_zimm /* verilator public */;
  logic [MXLEN-1:0] csr_wdata;

  logic [MXLEN-1:0] csr_wdata_vals [0:ZICSR_DATA_VALCOUNT-1];

  logic [MXLEN-1:0] csr_rdata;
  wire [MXLEN-1:0] csr_mtvec;
  wire [MXLEN-1:0] csr_mepc;

  always_comb begin
    csr_wdata_vals[ZICSR_DATA_RS1] = id_rs1;
    csr_wdata_vals[ZICSR_DATA_IMM] = id_csr_zimm;
  end

  // Pipeline wires
  wire ifid_stall /* verilator public */;
  wire ifid_flush /* verilator public */;
  wire ifid_valid /* verilator public */;

  wire idex_stall /* verilator public */;
  wire idex_flush /* verilator public */;
  wire idex_valid /* verilator public */;

  /////////////// IF      ///////////////
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

    .o_addr(if_pc),
    .o_addr_4(if_pc_4),
    .o_t_inst_addr_misaligned(t_inst_addr_misaligned),
    .o_t_inst_access_fault(t_inst_access_fault)
  );

  // IMEM
  inst_mem im(
    .i_addr(if_pc),
    .o_inst(if_inst)
  );

  /////////////// IF/ID   ///////////////
  ifid_reg ifid(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_stall(ifid_stall),
    .i_flush(ifid_flush),
    .i_data('{if_pc, if_pc_4, if_inst}),
    .o_data('{id_pc, id_pc_4, id_inst}),
    .o_valid(ifid_valid)
  );

  /////////////// ID      ///////////////
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

    .o_csr_we(id_csr_we),
    .o_csr_data_sel(id_csr_data_sel),
    .o_csr_addr(id_csr_addr),
    .o_csr_op(id_csr_op),
    .o_csr_zimm(id_csr_zimm),

    .o_t_illegal_inst(cu_t_illegal_inst),
    .o_t_ecall_m(t_ecall_m),
    .o_t_ebreak(t_ebreak),
    .o_trap_mret(trap_mret)
  );

  // Immediate sign extender
  sign_ext sext(
    .i_inst(id_inst),
    .i_sel(id_imm_sel),
    .o_imm(id_imm)
  );

  // Register file
  register_file #(
    .N_RPORTS(2),
    .N_REGS(NUM_REGS)
  ) rf(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_we(id_regfile_we),
    .i_wdata(reg_wb),
    .i_waddr(id_rd_addr),
    .i_raddr('{id_rs1_addr, id_rs2_addr}),
    .i_trap_req(trap_req),
    .o_rdata('{id_rs1, id_rs2})
  );

  /////////////// ID/EX   ///////////////
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
      id_csr_we,
      id_csr_data_sel,
      id_csr_addr,
      id_csr_op,
      id_csr_zimm,
      id_rs1,
      id_rs2,
      id_imm,
      id_pc,
      id_pc_4
    }),
    .i_valid(ifid_valid),
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
      ex_csr_we,
      ex_csr_data_sel,
      ex_csr_addr,
      ex_csr_op,
      ex_csr_zimm,
      ex_rs1,
      ex_rs2,
      ex_imm,
      ex_pc,
      ex_pc_4
    }),
    .o_valid(idex_valid)
  );

  /////////////// EX      ///////////////
  // ALU port A mux
  mux #(
    .N_OPTIONS(2),
    .DATA_WIDTH(XLEN)
  ) mux_alu_a(
    .i_sel(id_alu_a_sel),
    .i_val(alu_a_vals),
    .o_val(alu_a_in)
  );

  // ALU port B mux
  mux #(
    .N_OPTIONS(2),
    .DATA_WIDTH(XLEN)
  ) mux_alu_b(
    .i_sel(id_alu_b_sel),
    .i_val(alu_b_vals),
    .o_val(alu_b_in)
  );

  // ALU
  alu alu(
    .i_a(alu_a_in),
    .i_b(alu_b_in),
    .i_op(id_alu_op),
    .o(alu_out)
  );

  // BU
  bu bu(
    .i_be(id_bu_be),
    .i_a(id_rs1),
    .i_b(id_rs2),
    .i_op(id_bu_op),
    .o_take(take_branch)
  );

  /////////////// MEM     ///////////////
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
    .i_op(id_lsu_ls_op),
    .i_addr(alu_out),
    .i_wdata(id_rs2),
    .i_rdata_dmem(dmem_rdata),
    .i_rdata_rom(rom_rdata),
    .i_trap_req(trap_req),
    .o_addr(lsu_addr),
    .o_wdata(dmem_wdata),
    .o_rdata(lsu_rdata),
    .o_wstrb(dmem_wstrb),
    .o_we_dmem(dmem_we),
    .o_t_load_addr_misaligned(t_load_addr_misaligned),
    .o_t_load_access_fault(t_load_access_fault),
    .o_t_store_addr_misaligned(t_store_addr_misaligned),
    .o_t_store_access_fault(t_store_access_fault)
  );

  /////////////// WB      ///////////////
  // Register writeback mux
  mux #(
    .N_OPTIONS(REG_WB_VALCOUNT),
    .DATA_WIDTH(XLEN)
  ) mux_wb(
    .i_sel(id_reg_wb_sel),
    .i_val(reg_wb_vals),
    .o_val(reg_wb)
  );

  // Trap dispatch
  trap_dispatch td(
    .i_pc(id_pc),
    .i_inst(id_inst),
    .i_ls_addr(alu_out),
    .i_inst_addr_misaligned(t_inst_addr_misaligned),
    .i_inst_access_fault(t_inst_access_fault),
    .i_illegal_inst(t_illegal_inst),
    .i_ebreak(t_ebreak),
    .i_load_addr_misaligned(t_load_addr_misaligned),
    .i_load_access_fault(t_load_access_fault),
    .i_store_addr_misaligned(t_store_addr_misaligned),
    .i_store_access_fault(t_store_access_fault),
    .i_ecall_m(t_ecall_m),
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
    .i_sel(id_csr_data_sel),
    .i_val(csr_wdata_vals),
    .o_val(csr_wdata)
  );

  // CSR file
  csr_file csr(
    .i_clk(i_clk),
    .i_rst(i_rst),
    .i_we(id_csr_we),
    .i_op(id_csr_op),
    .i_addr(id_csr_addr),
    .i_wdata(csr_wdata),
    .i_pc(id_pc),

    .i_trap_req(trap_req),
    .i_trap_cause(trap_cause),
    .i_trap_tval(trap_tval),

    .o_rdata(csr_rdata),
    .o_mtvec(csr_mtvec),
    .o_mepc(csr_mepc),

    .o_t_illegal_inst(csr_t_illegal_inst)
  );

endmodule
