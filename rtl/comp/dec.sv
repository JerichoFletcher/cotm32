// N-bit decoder
module dec #(
  parameter N_SEL_BITS = 1
) (
  input logic [N_SEL_BITS-1:0] i_sel,
  output logic [(1<<N_SEL_BITS)-1:0] o
);

  always_comb begin
    o = '0;
    o[i_sel] = 1'b1;
  end

endmodule
