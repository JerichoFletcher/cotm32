`ifndef DEFS_RV32I_SVH
`define DEFS_RV32I_SVH

// Opcodes
`define OPCODE_WIDTH 7
`define OP_ALU    7'b0110011
`define OP_ALUI   7'b0010011
`define OP_JALR   7'b1100111
`define OP_LOAD   7'b0000011
`define OP_STORE  7'b0100011
`define OP_BRANCH 7'b1100011
`define OP_LUI    7'b0110111
`define OP_AUIPC  7'b0010111
`define OP_JAL    7'b1101111

`endif