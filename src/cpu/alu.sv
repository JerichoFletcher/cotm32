`include "defs.svh"

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

module alu (
  input logic [`XLEN-1:0] i_a,
  input logic [`XLEN-1:0] i_b,
  input alu_op_t i_op,
  output logic [`XLEN-1:0] o
);

  always_comb begin
    case (i_op)
      ALU_ADD:  o = i_a + i_b;
      ALU_SUB:  o = i_a - i_b;
      ALU_OR:   o = i_a | i_b;
      ALU_AND:  o = i_a & i_b;
      ALU_XOR:  o = i_a ^ i_b;
      ALU_SLT:  o = ($signed(i_a) < $signed(i_b)) ? 32'b1 : 32'b0;
      ALU_SLTU: o = i_a < i_b ? 32'b1 : 32'b0;
      ALU_SLL:  o = i_a << i_b[4:0];
      ALU_SRL:  o = i_a >> i_b[4:0];
      ALU_SRA:  o = $signed(i_a) >>> i_b[4:0];
      default:  o = '0;
    endcase
  end

endmodule