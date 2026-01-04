`include "defs.svh"

// N-bit multiplexer
module mux #(
  parameter NUM_OPTIONS = 2,
  parameter DATA_WIDTH = `XLEN
) (
  input logic [$clog2(NUM_OPTIONS)-1:0] i_sel,
  input logic [DATA_WIDTH-1:0] i_val [0:NUM_OPTIONS-1],
  output logic [DATA_WIDTH-1:0] o_val
);
  
  assign o_val = i_val[i_sel];

endmodule