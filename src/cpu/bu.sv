`include "defs.svh"

typedef enum logic [2:0] {
  BU_EQ,
  BU_NE,
  BU_LT,
  BU_GE,
  BU_LTU,
  BU_GEU
} bu_op_t;

module bu (
  input logic [`XLEN-1:0] i_a,
  input logic [`XLEN-1:0] i_b,
  input bu_op_t i_op,
  output logic o_take
);

  always_comb begin
    case (i_op)
      BU_EQ:  o_take = i_a == i_b;
      BU_NE:  o_take = i_a != i_b;
      BU_LT:  o_take = $signed(i_a) < $signed(i_b);
      BU_GE:  o_take = $signed(i_a) >= $signed(i_b);
      BU_LTU: o_take = i_a < i_b;
      BU_GEU: o_take = i_a >= i_b;
      default: o_take = '0;
    endcase
  end

endmodule