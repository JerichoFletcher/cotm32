module priv_control
  import cotm32_priv_pkg::*;
(
  input logic i_clk,
  input logic i_rst,

  input logic i_trap_req,
  input logic i_trap_mret,
  input zicsr_val_mstatus_t i_mstatus,
  output priv_mode_t o_priv_mode
);

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      o_priv_mode <= PRIV_M;
    end else if (i_trap_req) begin
      o_priv_mode <= PRIV_M;
    end else if (i_trap_mret) begin
      o_priv_mode <= i_mstatus.mpp;
    end
  end

endmodule
