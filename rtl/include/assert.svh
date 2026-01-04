`ifndef ASSERT_SVH
`define ASSERT_SVH

`define assert(sig, val) \
if (sig !== val) begin \
  $display("ASSERTION FAILED at %2d in %m: sig != val", $time); \
  $finish; \
end

`endif