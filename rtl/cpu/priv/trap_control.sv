// Trap control unit
module trap_control (
  input logic i_clk,
  input logic i_rst,
  input logic i_trap_mret,
  input logic i_trap_req,

  output logic o_trap_mode,
  output logic o_trap_mret
);

  import cotm32_pkg::*;

  initial begin
    o_trap_mode = '0;
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      o_trap_mode <= '0;
    end else if (i_trap_req) begin
      o_trap_mode <= '1;
    end else if (i_trap_mret) begin
      o_trap_mode <= '0;
    end
  end

  assign o_trap_mret = i_trap_mret;

endmodule
