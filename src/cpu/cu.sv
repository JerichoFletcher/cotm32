import cotm32_pkg::*;

// Control unit
module cu (
  input logic [INST_WIDTH-1:0] i_inst,
  output alu_op_t o_alu_op,
  output alu_a_sel_t o_alu_a_sel,
  output alu_b_sel_t o_alu_b_sel,
  output logic o_bu_be,
  output bu_op_t o_bu_op,
  output logic o_regfile_we,
  output logic [$clog2(NUM_REGS)-1:0] o_rd_addr,
  output logic [$clog2(NUM_REGS)-1:0] o_rs1_addr,
  output logic [$clog2(NUM_REGS)-1:0] o_rs2_addr,
  output imm_t o_imm_sel,
  output lsu_ls_t o_lsu_ls,
  output reg_wb_sel_t o_reg_wb_sel
);

  localparam REG_ADDR_WIDTH = $clog2(NUM_REGS);

  wire [6:0] opcode = i_inst[6:0];
  wire [REG_ADDR_WIDTH-1:0] rd = i_inst[7+:REG_ADDR_WIDTH];
  wire [REG_ADDR_WIDTH-1:0] rs1 = i_inst[15+:REG_ADDR_WIDTH];
  wire [REG_ADDR_WIDTH-1:0] rs2 = i_inst[20+:REG_ADDR_WIDTH];
  wire [6:0] funct7 = i_inst[31:25];
  wire [2:0] funct3 = i_inst[14:12];
  
  inst_t inst_type;

  always_comb begin
    o_bu_be = '0;
    o_regfile_we = '0;
    o_lsu_ls = LSU_NONE;

    unique case (opcode)
      OP_ALU    : begin
        o_rd_addr = rd;
        o_rs1_addr = rs1;
        o_rs2_addr = rs2;

        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_RS2;

        o_alu_op = f7f3_to_alu_op(funct7, funct3);
        o_reg_wb_sel = REG_WB_ALU;
        o_regfile_we = '1;
      end
      OP_ALUI   : begin
        o_rd_addr = rd;
        o_rs1_addr = rs1;

        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_I;

        o_alu_op = f7f3_to_alui_op(funct7, funct3);
        o_reg_wb_sel = REG_WB_ALU;
        o_regfile_we = '1;
      end
      OP_JALR   : begin
        o_rd_addr = rd;
        o_rs1_addr = rs1;

        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_I;

        o_alu_op = ALU_ADD;
        o_reg_wb_sel = REG_WB_PC4;
        o_regfile_we = '1;
      end
      OP_LOAD   : begin
        o_rd_addr = rd;
        o_rs1_addr = rs1;

        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_I;

        o_alu_op = ALU_ADD;
        o_lsu_ls = f3_to_lsu_l(funct3);
        o_reg_wb_sel = REG_WB_LSU;
        o_regfile_we = '1;
      end
      OP_STORE  : begin
        o_rs1_addr = rs1;
        o_rs2_addr = rs2;

        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_S;

        o_alu_op = ALU_ADD;
        o_lsu_ls = f3_to_lsu_s(funct3);
      end
      OP_BRANCH : begin
        o_rs1_addr = rs1;
        o_rs2_addr = rs2;

        o_alu_a_sel = ALU_A_PC;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_B;

        o_alu_op = ALU_ADD;
        o_bu_op = f3_to_bu_op(funct3);
        o_bu_be = '1;
      end
      OP_LUI    : begin
        o_rd_addr = rd;
        o_rs1_addr = '0;

        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_U;

        o_alu_op = ALU_ADD;
        o_reg_wb_sel = REG_WB_ALU;
        o_regfile_we = '1;
      end
      OP_AUIPC  : begin
        o_rd_addr = rd;

        o_alu_a_sel = ALU_A_PC;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_U;

        o_alu_op = ALU_ADD;
        o_reg_wb_sel = REG_WB_ALU;
        o_regfile_we = '1;
      end
      OP_JAL    : begin
        o_rd_addr = rd;
        o_alu_a_sel = ALU_A_PC;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_J;

        o_alu_op = ALU_ADD;
        o_bu_op = BU_ALWAYS;
        o_bu_be = '1;

        o_reg_wb_sel = REG_WB_PC4;
        o_regfile_we = '1;
      end
      // OP_MSCMEM :
      // OP_SYSTEM :
    endcase
  end

  function alu_op_t f7f3_to_alu_op(
    input logic [6:0] f7,
    input logic [2:0] f3
  );
    unique case (alu_f7_f3_t'({f7, f3}))
      ALU_F7F3_ADD  : f7f3_to_alu_op = ALU_ADD;
      ALU_F7F3_SUB  : f7f3_to_alu_op = ALU_SUB;
      ALU_F7F3_SLL  : f7f3_to_alu_op = ALU_SLL;
      ALU_F7F3_SLT  : f7f3_to_alu_op = ALU_SLT;
      ALU_F7F3_SLTU : f7f3_to_alu_op = ALU_SLTU;
      ALU_F7F3_XOR  : f7f3_to_alu_op = ALU_XOR;
      ALU_F7F3_SRL  : f7f3_to_alu_op = ALU_SRL;
      ALU_F7F3_SRA  : f7f3_to_alu_op = ALU_SRA;
      ALU_F7F3_OR   : f7f3_to_alu_op = ALU_OR;
      ALU_F7F3_AND  : f7f3_to_alu_op = ALU_AND;
      default   : f7f3_to_alu_op = ALU_ADD;
    endcase
  endfunction

  function alu_op_t f7f3_to_alui_op(
    input logic [6:0] f7,
    input logic [2:0] f3
  );
    unique case (f3)
      ALU_F7F3_ADD[2:0] : f7f3_to_alui_op = ALU_ADD;
      ALU_F7F3_SLL[2:0] : begin
        unique case (f7)
          ALU_F7F3_SLL[9:3] : f7f3_to_alui_op = ALU_SLL;
          default       : f7f3_to_alui_op = ALU_SLL;
        endcase
      end
      ALU_F7F3_SLT[2:0] : f7f3_to_alui_op = ALU_SLT;
      ALU_F7F3_SLTU[2:0]: f7f3_to_alui_op = ALU_SLTU;
      ALU_F7F3_XOR[2:0] : f7f3_to_alui_op = ALU_XOR;
      ALU_F7F3_SRL[2:0] : begin
        unique case (f7)
          ALU_F7F3_SRL[9:3] : f7f3_to_alui_op = ALU_SRL;
          ALU_F7F3_SRA[9:3] : f7f3_to_alui_op = ALU_SRA;
          default       : f7f3_to_alui_op = ALU_SRL;
        endcase
      end
      ALU_F7F3_OR[2:0]  : f7f3_to_alui_op = ALU_OR;
      ALU_F7F3_AND[2:0] : f7f3_to_alui_op = ALU_AND;
      default       : f7f3_to_alui_op = ALU_ADD;
    endcase
  endfunction

  function lsu_ls_t f3_to_lsu_l(input logic [2:0] f3);
    unique case (f3)
      LS_F3_B   : f3_to_lsu_l = LSU_LOAD_B;
      LS_F3_H   : f3_to_lsu_l = LSU_LOAD_H;
      LS_F3_W   : f3_to_lsu_l = LSU_LOAD_W;
      LS_F3_BU  : f3_to_lsu_l = LSU_LOAD_BU;
      LS_F3_HU  : f3_to_lsu_l = LSU_LOAD_HU;
      default   : f3_to_lsu_l = LSU_NONE;
    endcase
  endfunction

  function lsu_ls_t f3_to_lsu_s(input logic [2:0] f3);
    unique case (f3)
      LS_F3_B   : f3_to_lsu_s = LSU_STORE_B;
      LS_F3_H   : f3_to_lsu_s = LSU_STORE_H;
      LS_F3_W   : f3_to_lsu_s = LSU_STORE_W;
      default   : f3_to_lsu_s = LSU_NONE;
    endcase
  endfunction

  function bu_op_t f3_to_bu_op(input logic [2:0] f3);
    unique case (f3)
      BU_F3_EQ  : f3_to_bu_op = BU_EQ;
      BU_F3_NE  : f3_to_bu_op = BU_NE;
      BU_F3_LT  : f3_to_bu_op = BU_LT;
      BU_F3_GE  : f3_to_bu_op = BU_GE;
      BU_F3_LTU : f3_to_bu_op = BU_LTU;
      BU_F3_GEU : f3_to_bu_op = BU_GEU;
    endcase
  endfunction

endmodule