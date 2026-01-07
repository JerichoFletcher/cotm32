package cotm32_pkg;

// Sizes
parameter BYTE_WIDTH = 8;
parameter INST_WIDTH = 32;

parameter INST_OPCODE_WIDTH = 7;

// Core parameters
parameter XLEN = 32;
parameter NUM_REGS = 32;

parameter PC_RESET_VECTOR = '0;
parameter INST_MEM_SIZE = 1024;
parameter DATA_MEM_SIZE = 4096;

// ALU operation selector
typedef enum logic [$clog2(10)-1:0] {
  ALU_ADD,
  ALU_SUB,
  ALU_OR,
  ALU_AND,
  ALU_XOR,
  ALU_SLT,
  ALU_SLTU,
  ALU_SLL,
  ALU_SRL,
  ALU_SRA
} alu_op_t;

// BU operation selector
typedef enum logic [$clog2(8)-1:0] {
  BU_ALWAYS,
  BU_NEVER,
  BU_EQ,
  BU_NE,
  BU_LT,
  BU_GE,
  BU_LTU,
  BU_GEU
} bu_op_t;

// Instruction type
typedef enum logic [$clog2(6)-1:0] {
  INST_I,
  INST_S,
  INST_B,
  INST_U,
  INST_J,
  INST_R
} inst_t;

// Immediate type
typedef enum logic [$clog2(5)-1:0] {
  IMM_I,
  IMM_S,
  IMM_B,
  IMM_U,
  IMM_J
} imm_t;

// ALU port A mux selector
typedef enum logic {
  ALU_A_RS1,
  ALU_A_PC
} alu_a_sel_t;

// ALU port B mux selector
typedef enum logic {
  ALU_B_RS2,
  ALU_B_IMM
} alu_b_sel_t;

// IFU PC selector
parameter IFU_PC_VALCOUNT = 4;
typedef enum logic [$clog2(IFU_PC_VALCOUNT)-1:0] {
  IFU_PC_PC4,
  IFU_PC_BRANCH,
  IFU_PC_MTVEC,
  IFU_PC_MEPC
} ifu_pc_sel_t;

// LSU load-store selectors
typedef enum logic [$clog2(9)-1:0] {
  LSU_NONE,
  LSU_LOAD_B,
  LSU_LOAD_H,
  LSU_LOAD_W,
  LSU_LOAD_BU,
  LSU_LOAD_HU,
  LSU_STORE_B,
  LSU_STORE_H,
  LSU_STORE_W
} lsu_ls_t;

// Register writeback selectors
parameter REG_WB_VALCOUNT = 5;
typedef enum logic [$clog2(REG_WB_VALCOUNT)-1:0] {
  REG_WB_ZERO,
  REG_WB_ALU,
  REG_WB_PC4,
  REG_WB_LSU,
  REG_WB_CSR
} reg_wb_sel_t;

// Opcodes
typedef enum logic [INST_OPCODE_WIDTH-1:0] {
  OP_ALU    = 7'b0110011,
  OP_ALUI   = 7'b0010011,
  OP_JALR   = 7'b1100111,
  OP_LOAD   = 7'b0000011,
  OP_STORE  = 7'b0100011,
  OP_BRANCH = 7'b1100011,
  OP_LUI    = 7'b0110111,
  OP_AUIPC  = 7'b0010111,
  OP_JAL    = 7'b1101111,
  OP_MSCMEM = 7'b0001111,
  OP_SYSTEM = 7'b1110011
} opcode_t;

// Exact instructions
typedef enum logic [INST_WIDTH-1:0] {
  INST_EXACT_ECALL  = 32'h00000073,
  INST_EXACT_EBREAK = 32'h00100073,
  INST_EXACT_MRET   = 32'h30200073
} inst_exact_t;

// ALU operations funct7_funct3
typedef enum logic [9:0] {
  ALU_F7F3_ADD  = 10'b0000000_000,
  ALU_F7F3_SUB  = 10'b0100000_000,
  ALU_F7F3_SLL  = 10'b0000000_001,
  ALU_F7F3_SLT  = 10'b0000000_010,
  ALU_F7F3_SLTU = 10'b0000000_011,
  ALU_F7F3_XOR  = 10'b0000000_100,
  ALU_F7F3_SRL  = 10'b0000000_101,
  ALU_F7F3_SRA  = 10'b0100000_101,
  ALU_F7F3_OR   = 10'b0000000_110,
  ALU_F7F3_AND  = 10'b0000000_111
} alu_f7_f3_t;

// LS operations funct3
typedef enum logic [2:0] {
  LS_F3_B   = 3'b000,
  LS_F3_H   = 3'b001,
  LS_F3_W   = 3'b010,
  LS_F3_BU  = 3'b100,
  LS_F3_HU  = 3'b101
} ls_f3_t;

// BU operations funct3
typedef enum logic [2:0] {
  BU_F3_EQ  = 3'b000,
  BU_F3_NE  = 3'b001,
  BU_F3_LT  = 3'b100,
  BU_F3_GE  = 3'b101,
  BU_F3_LTU = 3'b110,
  BU_F3_GEU = 3'b111
} bu_f3_t;

endpackage : cotm32_pkg