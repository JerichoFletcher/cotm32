// Interrupt dispatch
module interrupt_dispatch
  import cotm32_pkg::*;
  import cotm32_priv_pkg::*;
(
  input zicsr_val_mstatus_t i_mstatus,
  input zicsr_val_mie_t i_mie,
  input zicsr_val_mip_t i_mip,

  output logic o_interrupt_req,
  output trap_cause_t o_interrupt_cause
);

  logic eip;
  logic sip;
  logic tip;

  assign eip = i_mie.meie && i_mip.meip;
  assign sip = i_mie.msie && i_mip.msip;
  assign tip = i_mie.mtie && i_mip.mtip;

  always_comb begin
    o_interrupt_req = '0;
    o_interrupt_cause = trap_cause_t'('0);

    if (i_mstatus.mie) begin
      if (eip) begin
        o_interrupt_req = '1;
        o_interrupt_cause = TRAP_CAUSE_M_EXTERNAL_INTERRUPT;
      end else if (sip) begin
        o_interrupt_req = '1;
        o_interrupt_cause = TRAP_CAUSE_M_SOFTWARE_INTERRUPT;
      end else if (tip) begin
        o_interrupt_req = '1;
        o_interrupt_cause = TRAP_CAUSE_M_TIMER_INTERRUPT;
      end
    end
  end

endmodule
