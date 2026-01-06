import cotm32_pkg::*;

// Branch unit
module bu (
  input logic i_be,
  input logic [XLEN-1:0] i_a,
  input logic [XLEN-1:0] i_b,
  input bu_op_t i_op,
  output logic o_take
);

  always_comb begin
    if (i_be) begin
      unique case (i_op)
        BU_ALWAYS : o_take = '1;
        BU_NEVER  : o_take = '0;
        BU_EQ     : o_take = i_a == i_b;
        BU_NE     : o_take = i_a != i_b;
        BU_LT     : o_take = $signed(i_a) < $signed(i_b);
        BU_GE     : o_take = $signed(i_a) >= $signed(i_b);
        BU_LTU    : o_take = i_a < i_b;
        BU_GEU    : o_take = i_a >= i_b;
        default   : o_take = '0;
      endcase
    end else o_take = '0;
  end

endmodule