import cotm32_pkg::*;

// N-bit multiplexer
module mux #(
  parameter N_OPTIONS = 2,
  parameter DATA_WIDTH = XLEN
) (
  input logic [$clog2(N_OPTIONS)-1:0] i_sel,
  input logic [DATA_WIDTH-1:0] i_val [0:N_OPTIONS-1],
  output logic [DATA_WIDTH-1:0] o_val
);
  
  genvar i;
  generate
    for (i = 0; i < N_OPTIONS; i++) begin : val
      wire [DATA_WIDTH-1:0] v = i_val[i];
    end
  endgenerate

  assign o_val = i_val[i_sel];

endmodule