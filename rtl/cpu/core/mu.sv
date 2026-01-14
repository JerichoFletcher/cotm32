import cotm32_pkg::XLEN;
import cotm32_pkg::mu_op_t;

module mu (
  input logic i_clk,
  input logic i_rst,

  input mu_op_t i_op,
  input logic [XLEN-1:0] i_a,
  input logic [XLEN-1:0] i_b,

  output logic [XLEN-1:0] o,
  output logic o_busy,
  output logic o_done
);

  import cotm32_pkg::*;

  localparam int MU_MUL_LATENCY = 4;
  localparam int MU_DIV_LATENCY = 12;

  logic start;
  logic busy;
  int timer;

  logic [XLEN-1:0] a;
  logic [XLEN-1:0] b;

  bit div_special;
  logic [XLEN-1:0] v_special;

  assign start = (!busy && i_op != MU_NOP);
  assign o_busy = busy || start;
  assign o_done = busy && (timer == 1);

  always_comb begin
    div_special = 0;
    v_special = '0;

    unique case (i_op)
      MU_DIV: begin
        if (b == 0) begin
          div_special = 1;
          v_special = -1;
        end else if (a == {1'b1, {(XLEN-1){1'b0}}} && b == -1) begin
          div_special = 1;
          v_special = a;
        end
      end
      MU_REM: begin
        if (b == 0) begin
          div_special = 1;
          v_special = a;
        end else if (a == {1'b1, {(XLEN-1){1'b0}}} && b == -1) begin
          div_special = 1;
          v_special = 0;
        end
      end
      MU_DIVU: begin
        if (b == 0) begin
          div_special = 1;
          v_special = -1;
        end
      end
      MU_REMU: begin
        if (b == 0) begin
          div_special = 1;
          v_special = a;
        end
      end
      default: begin /* NOP */ end
    endcase
  end

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      busy <= '0;
      timer <= 0;
      a <= '0;
      b <= '0;
    end else if (start) begin
      busy <= 1'b1;
      timer <= mu_latency(i_op);
      a <= i_a;
      b <= i_b;
    end else if (busy) begin
      timer <= timer - 1;
      if (timer == 1) begin
        busy <= '0;
      end
    end
  end

  /// TODO: Replace fake operations with actual impls
  logic signed [2*XLEN-1:0] p_ss;
  logic signed [2*XLEN-1:0] p_su;
  logic unsigned [2*XLEN-1:0] p_uu;

  logic signed [XLEN-1:0] q_s;
  logic signed [XLEN-1:0] q_u;
  logic signed [XLEN-1:0] r_s;
  logic signed [XLEN-1:0] r_u;

  assign p_ss = $signed(a) * $signed(b);
  assign p_uu = $unsigned(a) * $unsigned(b);
  assign p_su = $signed(a) * $unsigned(b);

  assign q_s = $signed(a) / $signed(b);
  assign q_u = $unsigned(a) / $unsigned(b);
  assign r_s = $signed(a) % $signed(b);
  assign r_u = $unsigned(a) % $unsigned(b);

  always_comb begin
    unique case (i_op)
      MU_MUL    : o = p_ss[31:0];
      MU_MULH   : o = p_ss[63:32];
      MU_MULHU  : o = p_uu[63:32];
      MU_MULHSU : o = p_su[63:32];
      MU_DIV    : o = div_special ? v_special : q_s;
      MU_DIVU   : o = div_special ? v_special : q_u;
      MU_REM    : o = div_special ? v_special : r_s;
      MU_REMU   : o = div_special ? v_special : r_u;
      default   : o = '0;
    endcase
  end

  function automatic int mu_latency(input mu_op_t mu_op);
    unique case (mu_op)
      MU_MUL, MU_MULH, MU_MULHU, MU_MULHSU: mu_latency = MU_MUL_LATENCY;
      MU_DIV, MU_DIVU, MU_REM, MU_REMU: mu_latency = MU_DIV_LATENCY;
      default: mu_latency = 0;
    endcase
  endfunction

endmodule

