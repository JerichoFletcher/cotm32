// Module for managing core sleep state on WFI
module wfi (
  input logic i_clk,
  input logic i_rst,

  input logic i_wfi_req,
  input logic i_trap_req,
  input logic i_interrupt_req,
  
  output logic o_sleep
);

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      o_sleep <= '0;
    end else if (!o_sleep && i_wfi_req && !i_trap_req) begin
      o_sleep <= '1;
    end else if (o_sleep && i_interrupt_req) begin
      o_sleep <= '0;
    end
  end

endmodule
