import cotm32_pkg::NUM_REGS;
import cotm32_pkg::XLEN;

// Register read port
module register_rport #(
  parameter N_REGS = NUM_REGS
) (
  input logic [$clog2(N_REGS)-1:0] i_raddr,
  input logic [XLEN-1:0] i_qbus [0:N_REGS-1],
  output logic [XLEN-1:0] o_rdata
);

  import cotm32_pkg::*;

  mux #(
    .N_OPTIONS(N_REGS),
    .DATA_WIDTH(XLEN)
  ) m(
    .i_sel(i_raddr),
    .i_val(i_qbus),
    .o_val(o_rdata)
  );

endmodule
