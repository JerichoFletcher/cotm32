import cotm32_pkg::XLEN;
import cotm32_pkg::alu_op_t;

// Arithmetic-logic unit
module alu (
  input logic [XLEN-1:0] i_a,
  input logic [XLEN-1:0] i_b,
  input alu_op_t i_op,
  output logic [XLEN-1:0] o
);

  import cotm32_pkg::*;

  always_comb begin
    unique case (i_op)
      ALU_ADD   : o = i_a + i_b;
      ALU_SUB   : o = i_a - i_b;
      ALU_OR    : o = i_a | i_b;
      ALU_AND   : o = i_a & i_b;
      ALU_XOR   : o = i_a ^ i_b;
      ALU_SLT   : o = ($signed(i_a) < $signed(i_b)) ? 32'b1 : 32'b0;
      ALU_SLTU  : o = i_a < i_b ? 32'b1 : 32'b0;
      ALU_SLL   : o = i_a << i_b[4:0];
      ALU_SRL   : o = i_a >> i_b[4:0];
      ALU_SRA   : o = $signed(i_a) >>> i_b[4:0];
      default   : o = '0;
    endcase
  end

endmodule
