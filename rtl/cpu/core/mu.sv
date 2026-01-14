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

  localparam int MU_LATENCY = 4;
  localparam int MU_LATENCY_BITW = $clog2(MU_LATENCY+1);

  logic start;
  logic busy;
  logic [MU_LATENCY_BITW-1:0] timer;

  logic [XLEN-1:0] a;
  logic [XLEN-1:0] b;

  assign start  = (!busy && i_op != MU_NOP);
  assign o_busy = busy || start;
  assign o_done = busy && (timer == 1);

  always_ff @(posedge i_clk) begin
    if (i_rst) begin
      busy <= '0;
      timer <= '0;
      a <= '0;
      b <= '0;
    end else begin
      if (start) begin
        busy <= 1'b1;
        timer <= (MU_LATENCY_BITW)'(MU_LATENCY);
        a <= i_a;
        b <= i_b;
      end else if (busy) begin
        timer <= timer - '1;
        if (timer == 1) begin
          busy <= '0;
        end
      end
    end
  end

  /// TODO: Replace fake multiplier with radix-4 Booth
  logic signed [2*XLEN-1:0] p_ss;
  logic signed [2*XLEN-1:0] p_su;
  logic unsigned [2*XLEN-1:0] p_uu;

  assign p_ss = $signed(a) * $signed(b);
  assign p_uu = $unsigned(a) * $unsigned(b);
  assign p_su = $signed(a) * $unsigned(b);

  always_comb begin
    unique case (i_op)
      MU_MUL    : o = p_ss[31:0];
      MU_MULH   : o = p_ss[63:32];
      MU_MULHU  : o = p_uu[63:32];
      MU_MULHSU : o = p_su[63:32];
      default   : o = '0;
    endcase
  end

endmodule

