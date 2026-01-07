import cotm32_pkg::*;
import cotm32_priv_pkg::*;

// Control unit
module cu (
  input logic [INST_WIDTH-1:0] i_inst,
  input logic i_trap_mode,

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
  output reg_wb_sel_t o_reg_wb_sel,

  output logic o_csr_we,
  output zicsr_data_sel_t o_csr_data_sel,
  output zicsr_csr_addr_t o_csr_addr,
  output zicsr_csr_op_t o_csr_op,
  output logic [MXLEN-1:0] o_csr_zimm,

  output logic o_t_illegal_inst,
  output logic o_t_ecall_m,
  output logic o_t_ebreak,
  output logic o_trap_mret
);

  localparam REG_ADDR_WIDTH = $clog2(NUM_REGS);

  wire [INST_OPCODE_WIDTH-1:0] opcode = i_inst[0+:INST_OPCODE_WIDTH];
  wire [REG_ADDR_WIDTH-1:0] rd = i_inst[7+:REG_ADDR_WIDTH];
  wire [REG_ADDR_WIDTH-1:0] rs1 = i_inst[15+:REG_ADDR_WIDTH];
  wire [REG_ADDR_WIDTH-1:0] rs2 = i_inst[20+:REG_ADDR_WIDTH];
  wire [6:0] funct7 = i_inst[25+:7];
  wire [2:0] funct3 = i_inst[12+:3];

  inst_t inst_type;

  assign o_csr_addr = zicsr_csr_addr_t'(i_inst[20+:ZICSR_CSR_ADDR_WIDTH]);
  assign o_csr_zimm = {27'b0, i_inst[19:15]};

  always_comb begin
    o_bu_be = '0;
    o_regfile_we = '0;
    o_lsu_ls = LSU_NONE;
    o_csr_we = '0;
    o_csr_op = ZICSR_CSR_OP_NONE;

    o_t_illegal_inst = '0;
    o_t_ecall_m = '0;
    o_t_ebreak = '0;
    o_trap_mret = '0;

    unique case (opcode)
      OP_ALU    : begin
        // Read from RS1, RS2; write to RD
        o_rd_addr = rd;
        o_rs1_addr = rs1;
        o_rs2_addr = rs2;

        // ALU A = RS1, B = RS2
        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_RS2;
        o_alu_op = f7f3_to_alu_op(funct7, funct3);

        // Write back from ALU to register
        o_reg_wb_sel = REG_WB_ALU;
        o_regfile_we = '1;
      end
      OP_ALUI   : begin
        // Read from RS1; write to RD
        o_rd_addr = rd;
        o_rs1_addr = rs1;

        // ALU A = RS1, B = imm
        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_I;
        o_alu_op = f7f3_to_alui_op(funct7, funct3);

        // Write back from ALU to register
        o_reg_wb_sel = REG_WB_ALU;
        o_regfile_we = '1;
      end
      OP_JALR   : begin
        // Read from RS1; write to RD
        o_rd_addr = rd;
        o_rs1_addr = rs1;

        // ALU A = RS1, B = imm
        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_I;
        o_alu_op = ALU_ADD;

        // Write back from PC+4 to register
        o_reg_wb_sel = REG_WB_PC4;
        o_regfile_we = '1;
      end
      OP_LOAD   : begin
        // Read from RS1; write to RD
        o_rd_addr = rd;
        o_rs1_addr = rs1;

        // ALU A = RS1, B = imm
        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_I;
        o_alu_op = ALU_ADD;

        // Write back from LSU to register
        o_lsu_ls = f3_to_lsu_l(funct3);
        o_reg_wb_sel = REG_WB_LSU;
        o_regfile_we = '1;
      end
      OP_STORE  : begin
        // Read from RS1, RS2
        o_rs1_addr = rs1;
        o_rs2_addr = rs2;

        // ALU A = RS1, B = imm
        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_S;
        o_alu_op = ALU_ADD;

        // Write from RS2 to LSU
        o_lsu_ls = f3_to_lsu_s(funct3);
      end
      OP_BRANCH : begin
        // Read from RS1, RS2
        o_rs1_addr = rs1;
        o_rs2_addr = rs2;

        // ALU A = PC, B = imm
        o_alu_a_sel = ALU_A_PC;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_B;
        o_alu_op = ALU_ADD;

        // Enable branching
        o_bu_op = f3_to_bu_op(funct3);
        o_bu_be = '1;
      end
      OP_LUI    : begin
        // Read from zero; write to RD
        o_rd_addr = rd;
        o_rs1_addr = '0;

        // ALU A = zero, B = imm
        o_alu_a_sel = ALU_A_RS1;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_U;
        o_alu_op = ALU_ADD;

        // Write back from ALU to register
        o_reg_wb_sel = REG_WB_ALU;
        o_regfile_we = '1;
      end
      OP_AUIPC  : begin
        // Write to RD
        o_rd_addr = rd;

        // ALU A = PC, B = imm
        o_alu_a_sel = ALU_A_PC;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_U;
        o_alu_op = ALU_ADD;

        // Write back from ALU to register
        o_reg_wb_sel = REG_WB_ALU;
        o_regfile_we = '1;
      end
      OP_JAL    : begin
        // Write to RD
        o_rd_addr = rd;

        // ALU A = PC, B = imm
        o_alu_a_sel = ALU_A_PC;
        o_alu_b_sel = ALU_B_IMM;
        o_imm_sel = IMM_J;
        o_alu_op = ALU_ADD;

        // Always take branch
        o_bu_op = BU_ALWAYS;
        o_bu_be = '1;

        // Write back from PC+4 to register
        o_reg_wb_sel = REG_WB_PC4;
        o_regfile_we = '1;
      end
      OP_MSCMEM : begin
        // TODO: Implement
      end
      OP_SYSTEM : begin
        unique case (i_inst)
          INST_EXACT_ECALL: begin
            // Dispatch an ecall trap request
            o_t_ecall_m = '1;
          end
          INST_EXACT_EBREAK: begin
            // Dispatch an ebreak trap request
            o_t_ebreak = '1;
          end
          INST_EXACT_MRET: begin
            if (i_trap_mode) begin
              o_trap_mret = '1;
            end else begin
              o_t_illegal_inst = '1;
            end
          end
          default: begin
            // Write to RD
            o_rd_addr = rd;

            // Write back from CSR to register
            o_reg_wb_sel = REG_WB_CSR;
            o_regfile_we = '1;

            unique case (funct3)
              ZICSR_F3_CSRRW  : begin
                o_rs1_addr = rs1;
                o_csr_we = '1;
                o_csr_op = ZICSR_CSR_OP_RW;
                o_csr_data_sel = ZICSR_DATA_RS1;
              end
              ZICSR_F3_CSRRS  : begin
                o_rs1_addr = rs1;
                o_csr_we = rs1 != '0;
                o_csr_op = ZICSR_CSR_OP_RS;
                o_csr_data_sel = ZICSR_DATA_RS1;
              end
              ZICSR_F3_CSRRC  : begin
                o_rs1_addr = rs1;
                o_csr_we = rs1 != '0;
                o_csr_op = ZICSR_CSR_OP_RC;
                o_csr_data_sel = ZICSR_DATA_RS1;
              end
              ZICSR_F3_CSRRWI : begin
                o_csr_we = '1;
                o_csr_op = ZICSR_CSR_OP_RW;
                o_csr_data_sel = ZICSR_DATA_IMM;
              end
              ZICSR_F3_CSRRSI : begin
                o_csr_we = o_csr_zimm != '0;
                o_csr_op = ZICSR_CSR_OP_RS;
                o_csr_data_sel = ZICSR_DATA_IMM;
              end
              ZICSR_F3_CSRRCI : begin
                o_csr_we = o_csr_zimm != '0;
                o_csr_op = ZICSR_CSR_OP_RC;
                o_csr_data_sel = ZICSR_DATA_IMM;
              end
              default: begin
                o_csr_we = '0;
                o_regfile_we = '0;
                o_t_illegal_inst = '1;
              end
            endcase
          end
        endcase
      end
      default: begin
        o_t_illegal_inst = '1;
      end
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
      default       : begin
        o_t_illegal_inst = '1;
        f7f3_to_alu_op = ALU_ADD;
      end
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
          default           : begin
            o_t_illegal_inst = '1;
            f7f3_to_alui_op = ALU_SLL;
          end
        endcase
      end
      ALU_F7F3_SLT[2:0] : f7f3_to_alui_op = ALU_SLT;
      ALU_F7F3_SLTU[2:0]: f7f3_to_alui_op = ALU_SLTU;
      ALU_F7F3_XOR[2:0] : f7f3_to_alui_op = ALU_XOR;
      ALU_F7F3_SRL[2:0] : begin
        unique case (f7)
          ALU_F7F3_SRL[9:3] : f7f3_to_alui_op = ALU_SRL;
          ALU_F7F3_SRA[9:3] : f7f3_to_alui_op = ALU_SRA;
          default           : begin
            o_t_illegal_inst = '1;
            f7f3_to_alui_op = ALU_SRL;
          end
        endcase
      end
      ALU_F7F3_OR[2:0]  : f7f3_to_alui_op = ALU_OR;
      ALU_F7F3_AND[2:0] : f7f3_to_alui_op = ALU_AND;
      default           : begin
        o_t_illegal_inst = '1;
        f7f3_to_alui_op = ALU_ADD;
      end
    endcase
  endfunction

  function lsu_ls_t f3_to_lsu_l(input logic [2:0] f3);
    unique case (f3)
      LS_F3_B   : f3_to_lsu_l = LSU_LOAD_B;
      LS_F3_H   : f3_to_lsu_l = LSU_LOAD_H;
      LS_F3_W   : f3_to_lsu_l = LSU_LOAD_W;
      LS_F3_BU  : f3_to_lsu_l = LSU_LOAD_BU;
      LS_F3_HU  : f3_to_lsu_l = LSU_LOAD_HU;
      default   : begin
        o_t_illegal_inst = '1;
        f3_to_lsu_l = LSU_NONE;
      end
    endcase
  endfunction

  function lsu_ls_t f3_to_lsu_s(input logic [2:0] f3);
    unique case (f3)
      LS_F3_B   : f3_to_lsu_s = LSU_STORE_B;
      LS_F3_H   : f3_to_lsu_s = LSU_STORE_H;
      LS_F3_W   : f3_to_lsu_s = LSU_STORE_W;
      default   : begin
        o_t_illegal_inst = '1;
        f3_to_lsu_s = LSU_NONE;
      end
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
      default   : begin
        o_t_illegal_inst = '1;
        f3_to_bu_op = BU_NEVER;
      end
    endcase
  endfunction

endmodule