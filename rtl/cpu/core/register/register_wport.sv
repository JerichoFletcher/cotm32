import cotm32_pkg::NUM_REGS;

// Register write port
module register_wport #(
  parameter N_REGS = NUM_REGS
) (
  input logic i_we,
  input logic [$clog2(N_REGS)-1:0] i_waddr,
  output logic [N_REGS-1:0] o_we
);

  import cotm32_pkg::*;

  logic [N_REGS-1:0] we;

  dec #(
    .N_SEL_BITS($clog2(N_REGS))
  ) d(
    .i_sel(i_waddr),
    .o(we)
  );

  assign o_we = (i_we && (i_waddr != '0)) ? we : '0;

endmodule
