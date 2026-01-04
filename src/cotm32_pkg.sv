package cotm32_pkg;

// Core parameters
parameter XLEN = 32;
parameter NUM_REGS = 32;

parameter BYTE_WIDTH = 8;
parameter INST_WIDTH = 32;

parameter OPCODE_WIDTH = 7;

// ALU operation selector
typedef enum logic [3:0] {
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
typedef enum logic [2:0] {
  BU_EQ,
  BU_NE,
  BU_LT,
  BU_GE,
  BU_LTU,
  BU_GEU
} bu_op_t;

// Immediate type
typedef enum logic [2:0] {
  IMM_I,
  IMM_S,
  IMM_B,
  IMM_U,
  IMM_J
} imm_t;

// Opcodes
typedef enum logic [OPCODE_WIDTH-1:0] {
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

endpackage : cotm32_pkg